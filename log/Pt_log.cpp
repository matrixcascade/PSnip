
#include "./Pt_log.h"
std::vector<pt_string> __pt_error_vlog,__pt_warning_vlog,pt_vlog;


pt_void ___fpt_log_error(pt_string str)
{
	if(__pt_error_vlog.size()<LOG_ERROR_MAX_COUNT)
	__pt_error_vlog.push_back(str);
}


pt_void ___fpt_log_warning(pt_string str)
{
	if(__pt_error_vlog.size()<LOG_WARNING_MAX_COUNT)
	__pt_warning_vlog.push_back(str);
}

pt_void ___fpt_log(pt_string str)
{
	if(__pt_error_vlog.size()<LOG_MAX_COUNT)
	pt_vlog.push_back(str);
}

