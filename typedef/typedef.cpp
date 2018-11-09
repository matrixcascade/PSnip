#include "typedef.h"

pt_char * __PT_STRCPY(pt_char *dest,const pt_char *Source)
{
	return strcpy(dest,Source);
}

pt_char * __PT_STRCAT(pt_char *dest,const pt_char *Source)
{
	return strcat(dest,Source);
}

pt_char * __PT_ITOA(_In_ pt_int _Val, _Pre_notnull_ _Post_z_ pt_char * _DstBuf, _In_ pt_int _Radix)
{
	return itoa(_Val,_DstBuf,_Radix);
}
