<?php
/**
 * @brief Push Notification library
 * @details Engine / Object Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0
 * @date Creation: Nov 2019
 * @date Modification: Nov 2019
 * @copyright 2019 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * @warning This library is not loaded with CaMykS initialisation.
 * @warning Windows phone method currently not tested
 * @todo Add sendings and failures to CaMykS logs.
 * @details Device plateforms
 * <br /> 0 : Unknown
 * <br /> 1 : Android
 * <br /> 2 : iOS
 * <br /> 3 : Windows Phone
 * <br />
 * <br /> Error codes
 * <br /> 1 : Unknown platform
 * <br /> 2 : Undefined identifier
 * <br /> 3 : Server connection failed
 * <br /> 4 : Message refused
 */

namespace CaMykS2;

/**
 * PushNotification class.
 */
final class PushNotification extends CaMykSLibrary {
    /**
     * @var array $params
     * @brief Notification parameters.
     */
    protected $params = array(
        /* Device data */
        'devicePlatform' => 0,
        'deviceIdentifier' => '',

        /* Notification params */
        'title' => '',
        'message' => '',
        'badge' => 0,
        'sound' => 'default',
        'vibrate' => 1,
        'delay' => 2,
        'customActionButton' => '', // customised action button for iOS
        'privateAppData' => array(), // private data to send to the app iOS / Android

        /* Android security key */
        'APIAccessKey' => '',

        /* iOS certificate */
        'mode' => '', // sandbox or empty
        'certificateLocation' => '',
        'certificatePassphrase' => '',
    );

    /**
     * Class constructor.
     * @param array $params
     * @return void
     */
    public function __construct($params=array()) {
        parent::__construct($params);
    }

    /* Override CaMykSLibrary methods */

    /**
     * Define error code list.
     * @return void
     */
    protected function define_errorCodes() {
        $this->errorCodes = array(
            1 => 'UnknownPlatform',
            2 => 'UndefinedIdentifier',
            3 => 'ServerConnectionFailed',
            4 => 'MessageRefused',
        );
    }

    /* Specific public methods */

    /**
     * Send notification.
     * @param array $params
     * @return boolean result
     */
    public function send($params=array()) {
        /* Update params */
        $this->update_params($params);

        /* Auto detect platform */
        /* @todo */

        /* Send notification through the correct engine */
        switch ($this->params['devicePlatform']) {
            case 1 :
                return $this->send_toAndroidDevice();
            case 2 :
                return $this->send_toIOsDevice();
            case 3 :
                return $this->send_toWindowsPhoneDevice();
        }

        /* No platform found, return as error */
        return  $this->add_error(1);
    }

    /**
     * Send notification to iOS device.
     * @param array $params
     * @return boolean result
     */
    public function send_toIOsDevice($params=array()) {
        /* Compile params */
        $this->params = array_merge($this->params, $params);

        /* Create stream context with SSL and Apple certificate */
        $context = stream_context_create();
        stream_context_set_option($context, 'ssl', 'local_cert', $this->params['certificateLocation']);
        stream_context_set_option($context, 'ssl', 'passphrase', $this->params['certificatePassphrase']);

        /* Open a connection to the APNS server */
        $connection = stream_socket_client(
            $this->params['mode'] == 'sandbox'?'ssl://gateway.sandbox.push.apple.com:2195':'ssl://gateway.push.apple.com:2195',
            $err,
            $errstr,
            60,
            STREAM_CLIENT_CONNECT|STREAM_CLIENT_PERSISTENT,
            $context
        );

        /* Check connection error */
        if (!$connection)
            return $this->add_error(3, "$err $errstr");

        /* Build payload body */
        $payload = array_merge(array(
            'aps' => array(
                'alert' => array(
                    'title' => $this->params['title'],
                    'body'  => $this->params['message'],
                ),
                'badge' => $this->params['badge'],
                'sound' => $this->params['sound'],
            ),
        ), $this->params['privateAppData']);

        if ($this->params['customActionButton'] != '')
            $payload['aps']['alert']['action-loc-key'] = $this->params['customActionButton'];
        $payload = json_encode($payload);

        /* Build binary notification */
        $notification = chr(0) . pack('n', 32) . pack('H*', $this->params['deviceIdentifier']) . pack('n', strlen($payload)) . $payload;

        /* Send notification to the server */
        $success = fwrite($connection, $notification, strlen($notification));

        /* Close connection */
        fclose($connection);

        /* Return result */
        return $success;
    }

    /**
     * Send notification to Android device.
     * @param array $params
     * @return boolean result
     */
    public function send_toAndroidDevice($params=array()) {
        /* Compile params */
        $this->params = array_merge($this->params, $params);

        /* Create android connection URL */
        $url = 'https://android.googleapis.com/gcm/send';

        /* Build message */
        $message = array(
            'title'         => $this->params['title'],
            'message'       => $this->params['message'],
            'subtitle'      => '',
            'tickerText'    => '',
            'msgcnt'        => 1,
            'vibrate'       => $this->params['vibrate'],
        );

        /* Build requet header */
        $headers = array(
            'Authorization: key=' .$this->params['APIAccessKey'],
            'Content-Type: application/json',
        );

        /* Build request fields */
        $fields = array(
            'to'    => $this->params['deviceIdentifier'],
        );

        if (count($this->params['privateAppData']) == 0) {
            $fields['data'] = $message;
        } else {
            $fields['notification'] = $message;
            $fields['data'] = $this->params['privateAppData'];
        }

        /* Send request */
        $result = $this->send_usingCurl($url, $headers, json_encode($fields));

        /* Check result */
        $result = json_decode($result);
        if ($result->success > 0)
            return true;

        /* Set error */
        if (isset($result->results[0]->error))
             return $this->add_error(4, $result->results[0]->error);
        return $this->add_error(4, 'Unknown error');
    }

    /**
     * send notification to Windows Phone device
     * @param array $params
     * @return boolean result
     */
    public function send_toWindowsPhoneDevice($params=array()) {
        /* compile params */
        $this->params = array_merge($this->params, $params);

        /* build message xml */
        $msg =  "<"."?xml version=\"1.0\" encoding=\"utf-8\"?".">" .
            "<wp:Notification xmlns:wp=\"WPNotification\">" .
                "<wp:Toast>" .
                    "<wp:Text1>".htmlspecialchars($this->params['title'])."</wp:Text1>" .
                    "<wp:Text2>".htmlspecialchars($this->params['message'])."</wp:Text2>" .
                "</wp:Toast>" .
            "</wp:Notification>";

        /* build headers */
        $headers =  array(
            'Content-Type: text/xml',
            'Accept: application/*',
            'X-WindowsPhone-Target: toast',
            'X-NotificationClass: '.$this->params['delay'],
        );

        /* get response from request */
        $response = $this->send_usingCurl($this->params['deviceIdentifier'], $headers, $msg);

        /* compile result */
        $result = array();
        foreach (explode("\n", $response) as $line) {
            $tab = explode(":", $line, 2);
            if (count($tab) == 2)
                $result[$tab[0]] = trim($tab[1]);
        }

        /* send result */
        return $result;
    }

    /* Private methods */

    /**
     * Send notification through curl
     * @param string $url
     * @param array $headers
     * @param array $fields
     * @return boolean result
     */
    private function send_usingCurl($url, $headers, $fields) {
        /* open connection */
        $ch = curl_init();
        if ($url) {
            /* define curl data */
            curl_setopt($ch, CURLOPT_URL, $url);
            curl_setopt($ch, CURLOPT_POST, true);
            curl_setopt($ch, CURLOPT_HTTPHEADER, $headers);
            curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);

            /*  Disabling SSL Certificate support temporarly */
            curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, false);
            if ($fields) {
                curl_setopt($ch, CURLOPT_POSTFIELDS, $fields);
            }

            /* Execute post */
            $result = curl_exec($ch);
            if ($result === false)
                return $this->add_error(3, curl_error($ch));

            /* Close connection */
            curl_close($ch);

            /* Return result */
            return $result;
        }

        /* Return failure */
        return $this->add_error(3, 'No URL defined');
    }
}
?>
