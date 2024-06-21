#pragma once

#include <memory>
#include <set>
#include <string>

class DllHandle {
public:
	using WhiteList = std::set<std::string>;
	DllHandle(const std::string& path,
	          const WhiteList& white_list = WhiteList());
	void* getSymbol(const std::string&);
	std::string getPath() const;
	std::string getMD5() const;

private:
	struct Impl;
	std::shared_ptr<Impl> pImpl;
};
