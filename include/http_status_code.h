#ifndef __HTTP_STATUS_CODE_H__
#define __HTTP_STATUS_CODE_H__

namespace http {

// Http Response Status codes, see mdn docs for more info:
// https://developer.mozilla.org/en-US/docs/Web/HTTP/Status.
enum class StatusCode {
    // Information responses.
    kContinue = 100,
    kSwitchingProtocols = 101,
    kProcessing = 102,
    kEarlyHints = 103,

    // Successful responses.
    kOk = 200,
    kCreated = 201,
    kAccepted = 202,
    kNonAuthoritativeInformation = 203,
    kNoContent = 204,
    kResetContent = 205,
    kPartialContent = 206,
    kMultiStatus = 207,
    kAlreadyReported = 208,
    kIMUsed = 226,

    // Redirection messages.
    kMultipleChoices = 300,
    kMovedPermanently = 301,
    kFound = 302,
    kSeeOther = 303,
    kNotModified = 304,
    //  Deprecated.
    kUseProxy = 305,
    //  306 is unused.
    kTemporaryRedirect = 307,
    kPermanentRedirect = 308,

    // Client error responses.
    kBadRequest = 400,
    kUnauthorized = 401,
    //  Experimental.
    kPaymentRequired = 402,
    kForbidden = 403,
    kNotFound = 404,
    kMethodNotAllowed = 405,
    kNotAcceptable = 406,
    kProxyAuthenticationRequired = 407,
    kRequestTimeout = 408,
    kConflict = 409,
    kGone = 410,
    kLengthRequired = 411,
    kPreconditionFailed = 412,
    kPayloadTooLarge = 413,
    kUriTooLong = 414,
    kUnsupportedMediaType = 415,
    kRangeNotSatisfiable = 416,
    kExpectationFailed = 417,
    kImATeapot = 418,
    kMisdirectedRequest = 421,
    kUnprocessableContent = 422,
    kLocked = 423,
    kFailedDependency = 424,
    //  Experimental.
    kTooEarly = 425,
    kUpgradeRequired = 426,
    kPreconditionRequired = 428,
    kTooManyRequests = 429,
    kRequestHeaderFieldsTooLarge = 431,
    kUnavailableForLegalReasons = 451,

    // Server error responses.
    kInternalServerError = 500,
    kNotImplemented = 501,
    kBadGateway = 502,
    kServiceUnavailable = 503,
    kGatewayTimeout = 504,
    kHttpVersionNotSupported = 505,
    kVariantAlsoNegotiates = 506,
    kInsufficientStorage = 507,
    kLoopDetected = 508,
    kNotExtended = 510,
    kNetworkAuthenticationRequired = 511,
};

} // namespace http

#endif // __HTTP_STATUS_CODE_H__
