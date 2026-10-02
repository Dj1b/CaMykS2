<?php
/**
 * @brief HTTPRequest library.
 * <br /> Object to execute HTTP requests and retrieve contents and statuses.
 * @details Engine / Object Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre1
 * @date Creation: Oct 2026
 * @date Modification: Oct 2026
 * @copyright 2026 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2;

/**
 * HTTPRequest class.
 */
final class HTTPRequest {
    /**
     * @var array $params
     * @brief Request parameters.
     */
    private array $params = array();

    /**
     * @var mixed $responseBody
     * @brief Last response body.
     */
    private mixed $responseBody = null;

    /**
     * @var array $responseHeaders
     * @brief Last response headers.
     */
    private array $responseHeaders = array();

    /**
     * @var int|false $responseCode
     * @brief Last response HTTP code.
     */
    private int|false $responseCode = false;

    /**
     * Class constructor.
     * @param string $url
     * @param array $params
     * @return void
     */
    public function __construct(string $url, array $params=array()) {
        global $CaMykS;

        $defaultParams = array(
            'Engine'            => 'php', // php | curl
            'Method'            => 'GET', // GET | POST
            'Content'           => array(), // fields
            'SSLVerifyPeer'     => true,
            'UserAgent'         => $CaMykS->get_configValue('Engine_UserAgent'),
            'Referer'           => '',
            'Timeout'           => 60,
            'Headers'           => array('Content-type' => 'application/x-www-form-urlencoded'),
            'SSL'               => array(),
        );

        $this->params = array_merge($defaultParams, $params, array('URL'=>$url));
    }

    /* Mark: Parameters methods */

    /**
     * Set a parameter.
     * @param string $name
     * @param mixed $value
     * @return void
     */
    public function set_param(string $name, mixed $value): void {
        $this->params[$name] = $value;
    }

    /* Mark: Public action methods */

    /**
     * Execute the request.
     * @return mixed
     */
    public function execute(): mixed {
        /* Check cURL engine */
        if ($this->params['Engine'] === 'curl' and !extension_loaded('curl'))
            $this->params['Engine'] = 'php';

        /* Execute request */
        if ($this->params['Engine'] === 'curl')
            $this->responseBody = $this->execute_usingCURL();
        else
            $this->responseBody = $this->execute_usingPHP();

        return $this->responseBody;
    }

    /**
     * Return response body.
     * @return mixed
     */
    public function get_responseBody(): mixed {
        return $this->responseBody;
    }

    /**
     * Return response headers.
     * @return array
     */
    public function get_responseHeaders(): array {
        return $this->responseHeaders;
    }

    /**
     * Return response code.
     * @return int|false
     */
    public function get_responseCode(): int|false {
        return $this->responseCode;
    }

    /**
     * Return response code name.
     * @return string|false
     */
    public function get_responseCodeName(): int|false {
        if ($this->responseCode === false)
            return false;
        return http_getResponseCodeName($this->responseCode);
    }

    /* Mark: Private methods */

    /**
     * Execute request using PHP engine.
     * @return mixed
     */
    private function execute_usingPHP(): mixed {
        /* Move referer to headers */
        if ($this->params['Referer'] !== '') {
            if (is_array($this->params['Headers'])) {
                $this->params['Headers']['Referer'] = $this->params['Referer'];
            } elseif (is_string($this->params['Headers'])) {
                $this->params['Headers'] .= "\r\nReferer: ".$this->params['Referer'];
            }
        }

        /* Build header */
        $header = '';
        if (is_string($this->params['Headers'])) {
            $header = $this->params['Headers'];
        } elseif (is_array($this->params['Headers'])) {
            foreach ($this->params['Headers'] as $name => $value) {
                $header .= $name.":".$value."\r\n";
            }
        }

        /* Build context */
        $opts = array(
            'http' => array(
                'header'        => $header,
                'method'        => $this->params['Method'],
                'user_agent'    => $this->params['UserAgent'],
                'timeout'       => $this->params['Timeout'],
                'ignore_errors' => true,
            ),
        );

        /* Check SSL peer verification */
        if ($this->params['SSLVerifyPeer'] === false) {
            $opts['ssl'] = array(
                'verify_peer'       => false,
                'verify_peer_name'  => false,
            );
        }

        /* Check other SSL parameters */
        if (is_array($this->params['SSL']) and count($this->params['SSL']) > 0) {
            if (isset($opts['ssl']))
                $opts['ssl'] = array_merge($opts['ssl'], $this->params['SSL']);
            else
                $opts['ssl'] = $this->params['SSL'];
        }

        /* Add content to context */
        if (is_array($this->params['Content']) and count($this->params['Content']) > 0) {
            $data = http_build_query($this->params['Content']);
            $opts['http']['content'] = $data;
        } elseif (is_string($this->params['Content']) and $this->params['Content'] !== '') {
            $opts['http']['content'] = $this->params['Content'];
        }

        /* Create context */
        $context = stream_context_create($opts);

        /* Execute request */
        $result = @file_get_contents($this->params['URL'], false, $context);

        /* Read headers and HTTP code */
        if (isset($http_response_header)) {
            $this->responseHeaders = $http_response_header;
            $this->responseCode = $this->extract_codeFromPHPHeaders($http_response_header);
        } else {
            $this->responseHeaders = array();
            $this->responseCode = false;
        }

        return $result;
    }

    /**
     * Execute request using cURL engine.
     * @return mixed
     */
    private function execute_usingCURL(): mixed {
        /* Build header */
        $header = array();
        if (is_string($this->params['Headers'])) {
            $header[] = $this->params['Headers'];
        } elseif (is_array($this->params['Headers'])) {
            foreach ($this->params['Headers'] as $name => $value) {
                $header[] = $name.":".$value;
            }
        }

        /* Build request */
        $request = curl_init();
        curl_setopt($request, CURLOPT_URL, $this->params['URL']);
        curl_setopt($request, CURLOPT_RETURNTRANSFER, true);
        curl_setopt($request, CURLOPT_FOLLOWLOCATION, true);
        curl_setopt($request, CURLOPT_HEADER, true); // We need headers to extract code and headers
        curl_setopt($request, CURLOPT_POST, $this->params['Method'] === 'POST' ? 1 : 0);

        if (is_array($this->params['Content']) and count($this->params['Content']) > 0) {
            curl_setopt($request, CURLOPT_POSTFIELDS, http_build_query($this->params['Content']));
        } elseif (is_string($this->params['Content']) and $this->params['Content'] !== '') {
            curl_setopt($request, CURLOPT_POSTFIELDS, $this->params['Content']);
        }

        curl_setopt($request, CURLOPT_SSL_VERIFYPEER, $this->params['SSLVerifyPeer']);
        if (isset($this->params['SSL']['ciphers']))
            curl_setopt($request, CURLOPT_SSL_CIPHER_LIST, $this->params['SSL']['ciphers']);
        if ($this->params['Timeout'] > 0)
            curl_setopt($request, CURLOPT_TIMEOUT, $this->params['Timeout']);
        if ($this->params['UserAgent'] !== '')
            curl_setopt($request, CURLOPT_USERAGENT, $this->params['UserAgent']);
        if ($this->params['Referer'] === true)
            curl_setopt($request, CURLOPT_AUTOREFERER, true);
        elseif ($this->params['Referer'] !== '')
            curl_setopt($request, CURLOPT_REFERER, $this->params['Referer']);
        if (count($header) > 0)
            curl_setopt($request, CURLOPT_HTTPHEADER, $header);

        /* Execute request */
        $response = @curl_exec($request);

        if ($response === false) {
            $this->responseCode = false;
            $this->responseHeaders = array();
            curl_close($request);
            return false;
        }

        $this->responseCode = curl_getinfo($request, CURLINFO_HTTP_CODE);
        $headerSize = curl_getinfo($request, CURLINFO_HEADER_SIZE);
        $headerStr = substr($response, 0, $headerSize);
        $body = substr($response, $headerSize);

        $this->responseHeaders = explode("\r\n", $headerStr);

        curl_close($request);
        return $body;
    }

    /**
     * Extract HTTP code from PHP headers array.
     * @param array $headers
     * @return int|false
     */
    private function extract_codeFromPHPHeaders(array $headers): int|false {
        foreach ($headers as $header) {
            if (preg_match('{HTTP\/\S*\s(\d{3})}', $header, $matches)) {
                return (int)$matches[1];
            }
        }
        return false;
    }
}
?>
