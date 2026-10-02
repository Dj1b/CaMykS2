<?php
/**
 * @brief HTTP specific methods.
 * @details Engine / Tool Library
 * @author CaMykS Team <CaMykS.contact@gmail.com>
 * @version 1.0pre6
 * @date Creation: Nov 2019
 * @date Modification: Oct 2026
 * @copyright 2019 - 2026 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

/**
 * Return string value from header request.
 * @param string $name
 * @param string $default
 * @return string
 */
function http_getStringRequest(string $name, string $default=''): string {
    return (isset($_REQUEST[$name]) ? $_REQUEST[$name] : $default);
}

/**
 * Return integer value from header request.
 * @param string $name
 * @param integer $default
 * @return integer
 */
function http_getIntRequest(string $name, int $default=0): int {
    return (int)http_getStringRequest($name, $default);
}

/**
 * Return float value from header request.
 * @param string $name
 * @param float $default
 * @return integer
 */
function http_getFloatRequest(string $name, float $default=0): int {
    return (float)http_getStringRequest($name, $default);
}

/**
 * Return HTTP response code list.
 * @return array
 */
function http_getResponseCodeList(): array {
    return array(
        /* 1xx - Information */
        100 => 'Continue',
        101 => 'Switching Protocols',
        102 => 'Processing',
        103 => 'Early Hints',

        /* 2xx - Success */
        200 => 'OK',
        201 => 'Created',
        202 => 'Accepted',
        203 => 'Non-Authoritative Information',
        204 => 'No Content',
        205 => 'Reset Content',
        206 => 'Partial Content',
        207 => 'Multi-Status',
        208 => 'Already Reported',
        210 => 'Content Different',
        226 => 'IM Used',

        /* 3xx - Redirection */
        300 => 'Multiple Choices',
        301 => 'Moved Permanently',
        302 => 'Found',
        303 => 'See Other',
        304 => 'Not Modified',
        305 => 'Use Proxy',
        306 => 'Switch Proxy',
        307 => 'Temporary Redirect',
        308 => 'Permanent Redirect',
        310 => 'Too many Redirects',

        /* 4xx - Request error */
        400 => 'Bad Request',
        401 => 'Unauthorized',
        402 => 'Payment Required',
        403 => 'Forbidden',
        404 => 'Not Found',
        405 => 'Method Not Allowed',
        406 => 'Not Acceptable',
        407 => 'Proxy Authentication Required',
        408 => 'Request Time-out',
        409 => 'Conflict',
        410 => 'Gone',
        411 => 'Length Required',
        412 => 'Precondition Failed',
        413 => 'Request Entity Too Large',
        414 => 'Request-URI Too Long',
        415 => 'Unsupported Media Type',
        416 => 'Requested range unsatisfiable',
        417 => 'Expectation failed',
        418 => 'I’m a teapot',
        421 => 'Bad mapping / Misdirected Request',
        422 => 'Unprocessable entity',
        423 => 'Locked',
        424 => 'Method failure',
        425 => 'Unordered Collection',
        426 => 'Upgrade Required',
        428 => 'Precondition Required',
        429 => 'Too Many Requests',
        431 => 'Request Header Fields Too Large',
        449 => 'Retry With',
        450 => 'Blocked by Windows Parental Controls',
        451 => 'Unavailable For Legal Reasons',
        456 => 'Unrecoverable Error',

        /* 5xx - Server/Application error */
        500 => 'Internal Server Error',
        501 => 'Not Implemented',
        502 => 'Bad Gateway ou Proxy Error',
        503 => 'Service Unavailable',
        504 => 'Gateway Time-out',
        505 => 'HTTP Version not supported',
        506 => 'Variant Also Negotiates',
        507 => 'Insufficient storage',
        508 => 'Loop detected',
        509 => 'Bandwidth Limit Exceeded',
        510 => 'Not extended',
        511 => 'Network authentication required',
    );
}

/**
 * Return HTTP response code name.
 * @param integer $code
 * @return string
 */
function http_getResponseCodeName(int $code=200): string {
    $list = http_getResponseCodeList();
    if (isset($list[$code]))
        return $list[$code];
    return 'Unknown';
}

?>
