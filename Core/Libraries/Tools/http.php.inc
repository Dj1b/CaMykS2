<?php
/**
 * @brief HTTP specific methods.
 * @details Engine / Tool Library
 * @author CaMykS Team <CaMykS.contact@gmail.com>
 * @version 1.0pre3
 * @date Creation: Nov 2019
 * @date Modification: Feb 2022
 * @copyright 2019 - 2022 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

/**
 * Return string value from header request.
 * @param string $name
 * @param string $default
 * @return string
 */
function http_getStringRequest($name, $default='') {
    return (isset($_REQUEST[$name]) ? $_REQUEST[$name] : $default);
}

/**
 * Return integer value from header request.
 * @param string $name
 * @param integer $default
 * @return integer
 */
function http_getIntRequest($name, $default=0) {
    return (int)http_getStringRequest($name, $default);
}

/**
 * Return float value from header request.
 * @param string $name
 * @param integer $default
 * @return integer
 */
function http_getFloatRequest($name, $default=0) {
    return (float)http_getStringRequest($name, $default);
}

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
 * \e string URL <br />
 * \e string Engine - \b php / \b curl <br />
 * \e string Method - \b GET / \b POST <br />
 * \e mixed Content <br />
 * \e boolean SSLVerifyPeer <br />
 * \e string UserAgent <br />
 * \e string Referer <br />
 * \e integer Timeout <br />
 * \e mixed Headers <br />
 * \e array SSL <br />
 * @endparblock
 * @return string
 */
function http_getContents($params=array()) {
    global $CaMykS;

    /* Build params list */
    $defaultParams = array(
        'URL'               => '',
        'Engine'            => 'php', // php | curl
        'Method'            => 'GET', // GET | POST
        'Content'           => null, // fields
        'SSLVerifyPeer'     => true,
        'UserAgent'         => $CaMykS->get_configValue('Engine_UserAgent'),
        'Referer'           => '',
        'Timeout'           => 60,
        'Headers'           => array('Content-type' => 'application/x-www-form-urlencoded'),
        'SSL'               => array(),
    );
    $params = array_merge($defaultParams, $params);

    /* Check cURL engine */
    if ($params['Engine'] == 'curl' and !extension_loaded('curl'))
        $params['Engine'] = 'php';

    /* Check engine to use */
    if ($params['Engine'] == 'curl')
        return http_getContentsUsingCURL($params);
    return http_getContentsUsingPHP($params);
}

/**
 * Return contents over HTTP using php functions.
 * @param array $params
 * @parblock - Available parameters are <br />
 * \e string URL <br />
 * \e string Method - \b GET / \b POST <br />
 * \e mixed Content <br />
 * \e boolean SSLVerifyPeer <br />
 * \e string UserAgent <br />
 * \e string referer <br />
 * \e integer timeout <br />
 * \e mixed Headers <br />
 * \e array SSL <br />
 * @endparblock
 * @return false on failure, string on success
 */
function http_getContentsUsingPHP($params=array()) {
    global $CaMykS;

    /* Build params list */
    $defaultParams = array(
        'URL'           => '',
        'Method'        => 'GET', // GET | POST
        'Content'       => array(), // fields
        'SSLVerifyPeer' => true,
        'UserAgent'     => $CaMykS->get_configValue('Engine_UserAgent'),
        'Referer'       => '',
        'Timeout'       => 60,
        'Headers'       => array('Content-type' => 'application/x-www-form-urlencoded'),
        'SSL'               => array(),
    );
    $params = array_merge($defaultParams, $params);

    /* Move referer to headers */
    if ($params['Referer'] !== '')
        $params['headers']['Referer'] = $params['Referer'];

    /* Build header */
    if (is_string($params['Headers']))
        $header = $params['Headers'];
    elseif (is_array($params['Headers'])) {
        $header = '';
        foreach ($params['Headers'] as $name => $value)
            $header = $name.":".$value."\r\n";
    }

    /* Build context */
    $opts = array(
        'http' => array(
            'header'        => $header,
            'method'        => $params['Method'],
            'user_agent'    => $params['UserAgent'],
            'timeout'       => $params['Timeout'],
        ),
    );

    /* Check SSL peer verification */
    if ($params['SSLVerifyPeer']) {
        $opts['ssl'] = array(
            'verify_peer'       => false,
            'verify_peer_name'  => false,
        );
    }

    /* Check other SSL parameters */
    if (count($params['SSL']) > 0) {
        if (isset($opts['SSL']))
            $opts['ssl'] = array_merge($opts['ssl'], $params['SSL']);
        else
            $opts['ssl'] = $params['SSL'];
    }

    /* Add content to context */
    if (is_array($params['Content']) and count($params['Content']) > 0) {
        $data = http_build_query($params['Values']);
        $opts['http']['content'] = $data;
    } elseif (is_string($params['Content']) and $params['Content'] !== '')
        $opts['http']['content'] = $params['Content'];

    /* Create context */
    $context  = stream_context_create($opts);

    /* Execute request */
    return @file_get_contents($params['URL'], false, $context);
}

/**
 * Return contents over HTTP using CURL functions.
 * @param array $params
 * @parblock - Available parameters are <br />
 * \e string URL <br />
 * \e string Method - \b GET / \b POST <br />
 * \e mixed Content <br />
 * \e boolean SSLVerifyPeer <br />
 * \e string UserAgent <br />
 * \e string Referer <br />
 * \e integer Timeout <br />
 * \e mixed Headers <br />
 * \e array SSL <br />
 * @endparblock
 * @return false on failure, string on success
 */
function http_getContentsUsingCURL($params=array()) {
    global $CaMykS;

    /* Build params list */
    $defaultParams = array(
        'URL'           => '',
        'Method'        => 'GET', // GET | POST
        'Content'        => array(), // fields
        'SSLVerifyPeer' => true, // curl
        'UserAgent'     => $CaMykS->get_configValue('Engine_UserAgent'),
        'Referer'       => '',
        'Timeout'       => 60,
        'Headers'       => array('Content-type' => 'application/x-www-form-urlencoded'),
        'SSL'               => array(),
    );
    $params = array_merge($defaultParams, $params);

    /* Build header */
    $header = array();
    if (is_string($params['Headers']))
        $header[] = array($params['Headers']);
    elseif (is_array($params['Headers'])) {
        foreach ($params['Headers'] as $name => $value)
            $header[] = $name.":".$value;
    }

    /* Build request */
    $request = curl_init();
    curl_setopt($request, CURLOPT_URL, $params['URL']);
    curl_setopt($request, CURLOPT_RETURNTRANSFER, true);
    curl_setopt($request, CURLOPT_FOLLOWLOCATION, true);
    curl_setopt($request, CURLOPT_POST, $params['Method']=='POST' ? 1 : 0);
    curl_setopt($request, CURLOPT_POSTFIELDS, $params['Values']);
    curl_setopt($request, CURLOPT_SSL_VERIFYPEER, $params['SSLVerifyPeer']);
    if (isset($params['SSL']['ciphers']))
        curl_setopt($request, CURLOPT_SSL_CIPHER_LIST, $params['SSL']['ciphers']);
    if ($params['Timeout'] > 0)
        curl_setopt($request, CURLOPT_TIMEOUT, $params['Timeout']);
    if ($params['UserAgent'] != '')
        curl_setopt($request, CURLOPT_USERAGENT, $params['UserAgent']);
    if ($params['Referer'] === true)
        curl_setopt($request, CURLOPT_AUTOREFERER, true);
    elseif ($params['Referer'] != '')
        curl_setopt($request, CURLOPT_REFERER, $params['Referer']);
    if (count($header) > 0)
        curl_setopt($request, CURLOPT_HTTPHEADER, $header);

    /* Execute request */
    return @curl_exec($request);
}
?>
