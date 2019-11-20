<?php
/**
 * @brief HTTP specific methods.
 * @details Engine / Tool Library
 * @author CaMykS Team <CaMykS.contact@gmail.com>
 * @version 1.0.0
 * @date Creation: Nov 2019
 * @date Modification: Nov 2019
 * @copyright 2019 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

/**
 * Return HTTP response code list.
 * @return array
 */
function http_getResponseCodeList() {
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
function http_getResponseCodeName($code=200) {
    $list = http_getResponseCodeList();
    if (isset($list[$code]))
        return $list[$code];
    return 'Unknown';
}

/**
 * Return contents over HTTP.
 * @param array $params
 * @parblock - Available parameters are <br />
 * \e string url <br />
 * \e string engine - \b php / \b curl <br />
 * \e string method - \b GET / \b POST <br />
 * \e array values <br />
 * \e boolean SSL_VerifyPeer <br />
 * \e string user_agent <br />
 * \e string referer <br />
 * \e integer timeout <br />
 * @endparblock
 * @return string
 */
function http_getContents($params=array()) {
    global $CaMykS;

    /* build params list */
    $defaultParams = array(
        'URL'               => '',
        'Engine'            => 'php', // php | curl
        'Method'            => 'GET', // GET | POST
        'Values'            => array(), // fields
        'SSLVerifyPeer'    => true,
        'UserAgent'        => $CaMykS->get_configValue('Engine_UserAgent'),
        'Referer'           => '',
        'Timeout'           => 60,
    );
    $params = array_merge($defaultParams, $params);

    /* check curl engine */
    if ($params['Engine'] == 'curl' and !function_exists('curl_init'))
        $params['Engine'] = 'php';

    /* get context using the indicated engine */
    if ($params['Engine'] == 'curl')
        return http_getContentsUsingCURL($params);
    return http_getContentsUsingPHP($params);
}

/**
 * Return contents over HTTP using php functions.
 * @param array $params
 * @parblock - Available parameters are <br />
 * \e string url <br />
 * \e string method - \b GET / \b POST <br />
 * \e array values <br />
 * \e string user_agent <br />
 * \e string referer <br />
 * \e integer timeout <br />
 * @endparblock
 * @return false on failure, string on success
 */
function http_getContentsUsingPHP($params=array()) {
    global $CaMykS;

    /* build params list */
    $defaultParams = array(
        'URL'           => '',
        'Method'        => 'GET', // GET | POST
        'Values'        => array(), // fields
        'SSLVerifyPeer'=> true,
        'UserAgent'    => $CaMykS->get_configValue('Engine_UserAgent'),
        'Referer'       => '',
        'Timeout'       => 60,
    );
    $params = array_merge($defaultParams, $params);

    /* build context */
    $opts = array(
        'http' => array(
            'header'        => 'Content-type: application/x-www-form-urlencoded',
            'method'        => $params['Method'],
            'user_agent'    => $params['UserAgent'],
            'referer'       => $params['Referer'],
            'timeout'       => $params['Timeout'],
        ),
    );

    /* check SSL peer verification */
    if ($params['SSLVerifyPeer']) {
        $opts['ssl'] = array(
            'verify_peer'       => false,
            'verify_peer_name'  => false,
        );
    }

    /* add fields values to context */
    if (count($params['Values']) > 0) {
        $data = http_build_query($params['Values']);
        $opts['http']['content'] = $data;
    }

    /* get contents */
    $context  = stream_context_create($opts);
    return @file_get_contents($params['URL'], false, $context);
}

/**
 * Return contents over HTTP using CURL functions.
 * @param array $params
 * @parblock - Available parameters are <br />
 * \e string url <br />
 * \e string method - \b GET / \b POST <br />
 * \e array values <br />
 * \e boolean SSL_VerifyPeer <br />
 * \e string user_agent <br />
 * \e string referer <br />
 * \e integer timeout <br />
 * @endparblock
 * @return false on failure, string on success
 */
function http_getContentsUsingCURL($params=array()) {
    global $CaMykS;

    /* build params list */
    $defaultParams = array(
        'URL'               => '',
        'Method'            => 'GET', // GET | POST
        'Values'            => array(), // fields
        'SSLVerifyPeer'     => true, // curl
        'UserAgent'         => $CaMykS->get_configValue('Engine_UserAgent'),
        'Referer'           => '',
        'Timeout'           => 60,
    );
    $params = array_merge($defaultParams, $params);

    /* build request */
    $request = curl_init();
    curl_setopt($request, CURLOPT_URL, $params['URL']);
    curl_setopt($request, CURLOPT_RETURNTRANSFER, true);
    curl_setopt($request, CURLOPT_FOLLOWLOCATION, true);
    curl_setopt($request, CURLOPT_POST, $params['Method']=='POST' ? 1 : 0);
    curl_setopt($request, CURLOPT_POSTFIELDS, $params['Values']);
    curl_setopt($request, CURLOPT_SSL_VERIFYPEER, $params['SSLVerifyPeer']);
    if ($params['Timeout'] > 0)
        curl_setopt($request, CURLOPT_TIMEOUT, $params['Timeout']);
    if ($params['UserAgent'] != '')
        curl_setopt($request, CURLOPT_USERAGENT, $params['UserAgent']);
    if ($params['Referer'] === true)
        curl_setopt($request, CURLOPT_AUTOREFERER, true);
    elseif ($params['Referer'] != '')
        curl_setopt($request, CURLOPT_REFERER, $params['Referer']);

    /* execute request */
    return @curl_exec($request);
}
?>
