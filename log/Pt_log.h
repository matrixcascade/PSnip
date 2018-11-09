#pragma once
#include "../typedef/typedef.h"
#include <vector>
#include <map>
#define LOG_MAX_COUNT			1024
#define LOG_ERROR_MAX_COUNT		256
#define LOG_WARNING_MAX_COUNT	256
pt_void ___fpt_log_error(pt_string str);
pt_void ___fpt_log(pt_string str);
pt_void ___fpt_log_warning(pt_string str);
#define PT_ERROR(x) ___fpt_log_error(x)
#define PT_WARNING(x) ___fpt_log_warning(x)
#define PT_LOG(x) ___fpt_log(x)