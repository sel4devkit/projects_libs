/* Wrappers around U-Boot print string routines */

#include <utils/zf_log.h>
#include <stdio.h>

#define debug(...)          ZF_LOGD(__VA_ARGS__)

#define debug_cond(A, ...)  ZF_LOGD(__VA_ARGS__)

#define pr_debug(...)       ZF_LOGD(__VA_ARGS__)

#define log(A, B, ...)      ZF_LOGD(__VA_ARGS__)

#define log_debug(...)      ZF_LOGD(__VA_ARGS__)

#define log_warning(...)    ZF_LOGW(__VA_ARGS__)

#define printk(...)         printf(__VA_ARGS__)

#define puts(...)           printf(__VA_ARGS__)

#define putc(CHAR)          putc(CHAR, stdout)

#define pr_info(...)        ZF_LOGI(__VA_ARGS__)

#define pr_err(...)         ZF_LOGE(__VA_ARGS__)

#define pr_warn(...)        ZF_LOGW(__VA_ARGS__)

#define assert_noisy(x) \
	({ bool _val = (x); \
	if (!_val) \
		panic("assert_noisy triggered"); \
	_val; \
	})

#define log_ret(_ret) ({ \
	int __ret = (_ret); \
	if (__ret < 0) \
		ZF_LOGE("returning err=%d\n", __ret); \
	__ret; \
	})

#define log_msg_ret(_msg, _ret) ({ \
	int __ret = (_ret); \
	if (__ret < 0) \
		ZF_LOGE("%s: returning err=%d\n", _msg, __ret); \
	__ret; \
	})
