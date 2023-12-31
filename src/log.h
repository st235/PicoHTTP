#ifndef __LOG_H__
#define __LOG_H__

#ifndef PLOGD
#define PLOGD(...) printf("[%s:%d] [%s]: ", __FILE__, __LINE__, __func__), printf(__VA_ARGS__), printf("\n")
#endif

#endif // __LOG_H__
