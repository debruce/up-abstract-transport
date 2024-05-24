#include "UpAbstractTransport.hpp"
#include "Impl_zenoh.hpp"
#include <iostream>
#include <mutex>
#include <condition_variable>
#include <deque>
#include <thread>

namespace Impl_zenoh {

using namespace UpAbstractTransport;
using namespace std;

// thread_local std::shared_ptr<Subscriber::Info> Subscriber::info;

struct SInfoImpl {
    string source;
    Message message;

    SInfoImpl(const zenohc::Sample& sample)
    {
        source = sample.get_keyexpr().as_string_view();
        message.payload = sample.get_payload().as_string_view();
        message.attributes = sample.get_attachment().get("attributes").as_string_view();
    }
};

struct SubscriberImpl : public SubscriberApi {
    shared_ptr<TransportImpl> trans_impl;
    std::unique_ptr<zenohc::Subscriber> handle;
    string listening_topic;
    zenohc::KeyExprView expr;
    std::mutex  mtx;
    std::condition_variable cv;
    std::deque<std::shared_ptr<SInfoImpl>> queue;
    bool die;
    SubscriberCallback callback;

    std::vector<std::thread> thread_pool;

    void handler(const zenohc::Sample& sample)
    {
        auto wq = std::make_shared<SInfoImpl>(sample);
        std::unique_lock<std::mutex> lock(mtx);
        queue.push_front(wq);
        cv.notify_one();
    }

    void worker()
    {
        using namespace std;

        while (true) {
            shared_ptr<SInfoImpl> ptr = nullptr;
            {
                unique_lock<mutex> lock(mtx);
                cv.wait(lock, [&](){ return !queue.empty() && !die; });
                if (die) break;
                ptr = queue.back();
                queue.pop_back();
            }
            callback(ptr->source, listening_topic, ptr->message);
        }
    }

    SubscriberImpl(Transport transport, const std::string& topic, SubscriberCallback _callback)
     : expr(topic)
    {
        cout << __PRETTY_FUNCTION__ << " topic=" << topic << endl;
        trans_impl = any_cast<shared_ptr<TransportImpl>>(transport.pImpl->impl);
        listening_topic = topic;
        callback = _callback;
        handle = std::make_unique<zenohc::Subscriber>(
            zenohc::expect<zenohc::Subscriber>(
                trans_impl->session.declare_subscriber(
                    expr,
                    [&](const zenohc::Sample& arg) { this->handler(arg); } )));

        die = false;
        const size_t thread_count = 4;
        thread_pool.reserve(thread_count);
        for (size_t i = 0; i < 10; i++) {
            thread_pool.emplace_back([&]() { worker(); });
        }
    }

    ~SubscriberImpl()
    {
        {
            std::unique_lock<std::mutex> lock(mtx);
            die = true;
        }
        for (auto& thr : thread_pool) thr.join();
        thread_pool.clear();        
    }
};

std::shared_ptr<SubscriberApi> subscriber_getter(Transport transport, const std::string& topic, SubscriberCallback callback)
{
    return make_shared<SubscriberImpl>(transport, topic, callback);
}

}; // Impl_zenoh