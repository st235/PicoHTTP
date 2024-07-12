#ifndef __HTTP_STATUS_CODE_H__
#define __HTTP_STATUS_CODE_H__

namespace http {

enum class StatusCode {
    OK,
    BAD_REQUEST,
    UNAUTHORIZED,
    FORBIDDEN,
    NOT_FOUND,
    INTERNAL_SERVER_ERROR,
};

} // namespace http

#endif // __HTTP_STATUS_CODE_H__
