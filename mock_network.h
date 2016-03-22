#ifndef MOCK_NETWORK_H_
#define MOCK_NETWORK_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/socket.h> //ssize_t

ssize_t ut_send(int socket, const void *buffer, size_t length, int flags);

#ifndef DISABLE_MOCKS
#    define send ut_send
#endif


#ifdef __cplusplus
}
#endif



#endif // MOCK_NETWORK_H_