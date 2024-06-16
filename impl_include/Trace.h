#pragma once

#include <sys/sdt.h>

#define LINENO() __LINE__
#define TRACE(obj, desc)                                 \
	{                                                    \
		char buf[64];                                    \
		(obj)->trace(buf, __FUNCTION__, __LINE__, desc); \
		DTRACE_PROBE1(tracehook, LINENO(), buf);         \
	}

struct TraceBase {
	std::string trace_name;

	TraceBase(const std::string& trace_name) : trace_name(trace_name) {}

	void trace(char buf[64], const char* fn, int line_no,
	           const std::string& desc) {
		memset(buf, 0, 64);
		snprintf(buf, 64, "%s:%s:%d:%s", trace_name.c_str(), fn, line_no,
		         desc.c_str());
	}
};
