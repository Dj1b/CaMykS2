<?php
/**
 * @brief HTML Page.
 * @details Engine / Object Library
 * @author CaMykS Team <CaMykS.contact@gmail.com>
 * @version 1.0.4
 * @date Creation: Feb 2020
 * @date Modification: Mar 2020
 * @copyright 2020 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2;

/**
 * HTMLPage class.
 */
final class HTMLPage {
    /**
     * @var string $step
     * @brief Page current step, values are init, body, done.
     */
    private $step = 'init';

    /**
     * @var integer $status
     * @brief Page status.
     */
    private $status = 200;

    /**
     * @var header $headers
     * @brief Page headers.
     */
    private $headers = array(
        'Content-Type' => 'text/html',
    );

    /**
     * @var string $content
     * @brief Page content.
     */
    private $content = '';

    /**
     * @var integer $HTMLVersion
     * @brief HTML version the page has to display.
     */
    private $HTMLVersion = 5;

    /**
     * @var boolean $requiresBaseURL
     * @brief Does the page requires the HTML base url for the internal links.
     */
    private $requiresBaseURL = false;

    /**
     * @var string $title
     * @brief Page title for page header.
     */
    private $title = '';

    /**
     * @var string $keywords
     * @brief Keywords for page header.
     */
    private $keywords = '';

    /**
     * @var string $description
     * @brief Description for page header.
     */
    private $description = '';

    /**
     * @var string $language
     * @brief Page current language.
     */
    private $language = '';

    /**
     * @var boolean $isHome
     * @brief Is page the homepage of website.
     */
    private $isHome = false;

    /**
     * @var array $robotsDirectives
     * @brief Page robots index and follow directives.
     */
    private $robotsDirectives = array(
        /**< Index directive value */
        'Index'     => true,
        /**< Follow directive value */
        'Follow'    => true,
    );

    /**
     * @var array $metas
     * @brief Page metas.
     */
    private $metas = array();

    /**
     * @var array $links
     * @brief Page links.
     */
    private $links = array();

    /**
     * @var array $JSFiles
     * @brief Client side script files to load.
     */
    private $JSFiles = array();

    /**
     * @var array $JSScripts
     * @brief Client side scripts to execute.
     */
    private $JSScripts = array(
        /**< Scripts displayed before JS files loading. */
        'pre'   => array(),
        /**< Main scripts. */
        'main'  => array(),
        /**< Scripts displayed in on page load event. */
        'load'  => array(),
        /**< Other page events can be inserted. */
    );

    /**
     * @var array $JSStructuredData
     * @brief JSON structured data for search engines.
     */
    private $JSStructuredData = array();

    /**
     * @var array $styleFiles
     * @brief Style files to load for media and constraints.
     */
    private $styleFiles = array();

    /**
     * @var array $styleIncludedFiles
     * @brief Style files to insert in header.
     */
    private $styleIncludedFiles = array();

    /**
     * @var array $styleScripts
     * @brief Style scripts to insert in header.
     */
    private $styleScripts = array();

    /**
     * @var string $headExtras
     * @brief HTML head extras.
     */
    private $headExtras = '';

    /**
     * @var string $bodyClass
     * @brief HTML body tag to apply.
     */
    private $bodyClass = '';

    /**
     * Class constructor.
     * @return void
     */
    public function __construct() {
        global $CaMykS;

        /* Add charset meta as first meta */
        $this->add_meta('charset', $CaMykS->get_configValue('Website_Encoding', 'UTF-8'), 'charset');

        /* Add generator meta */
        if ($CaMykS->get_configValue('Website_DisplayCMSSignature', true))
            $this->add_meta('generator', $CaMykS->name.' '.$CaMykS->version.' ('.$CaMykS->CMSWebsiteURL.')');

        /* Add author meta */
        if (($author = $CaMykS->get_configValue('Website_Author')) != '')
            $this->add_meta('author', $author);
    }

    /* Headers related methods */

    /**
     * Define page status.
     * @param integer $status
     * @return void
     */
    public function set_status($status) {
        $this->status = $status;
    }

    /**
     * Insert page header.
     * @param string $name
     * @param string $value
     * @return void
     */
    public function add_header($name, $value) {
        $this->headers[$name] = $value;
    }

    /* HTML data insertion methods */

    /**
     * Define HTML version.
     * @param mixed $version
     * @return void
     */
    public function set_HTMLVersion($version=5) {
        $this->HTMLVersion = $version;
    }

    /**
     * Indicate the page to require the base URL information
     * @return void
     */
    public function require_baseURL() {
        $this->requiresBaseURL = true;
    }

    /**
     * Define some page information.
     * @param string $title
     * @param string $description
     * @param string $keywords
     * @param string $language
     * @param string $isHome
     * @return void
     */
    public function set_pageInformation($title=null, $description=null, $keywords=null, $language=null, $isHome=null) {
        if (!is_null($title)) $this->title = $title;
        if (!is_null($description)) $this->description = $description;
        if (!is_null($keywords)) $this->keywords = $keywords;
        if (!is_null($language)) $this->language = $language;
        if (!is_null($isHome)) $this->isHome = $isHome;
    }

    /**
     * Define charset.
     * @param string $charset
     * @return void
     */
    public function set_charset($charset='UTF-8') {
        if (!is_null($charset)) $this->charset = $charset;
    }

    /**
     * Define robots directives.
     * @param boolean $index
     * @param boolean $follow
     * @return void
     */
    public function set_robotsDirectives($index=null, $follow=null) {
        if (!is_null($index)) $this->robotsDirectives['index'] = $index;
        if (!is_null($follow)) $this->robotsDirectives['follow'] = $follow;
    }

    /**
     * Insert a meta.
     * @param string $key the meta name
     * @param string $value the meta value
     * @param string $method the meta displaying method
     * @return void
     */
    public function add_meta($key, $value, $method='name') {
        if ($method == 'link:rel')
            return $this->add_link($key, $value);
        $this->metas[$key] = array('Value' => $value, 'Method' => $method);
    }

    /**
     * Insert a link.
     * @param string $rel the rel value
     * @param string $href the href value
     * @param array $params the other params
     * @return void
     */
    public function add_link($rel, $href, $params=array()) {
        if ($rel == null)
            $this->links[] = array_merge($params, array('Href' => $href));
        else
            $this->links[] = array_merge(array('Rel' => $rel, 'Href' => $href), $params);
    }

    /**
     * Attach a Javascript file.
     * @param string $file
     * @param array $params
     * @return void
     */
    public function add_JSFile($file='', $params=array()) {
        /* Check file */
        if ($file != '' and !isset($this->JSFiles[$file]))
            $this->JSFiles[$file] = $params;
    }

    /**
     * Insert a new Javascript script.
     * @param string $script
     * @param boolean $unique
     * @param string $event
     * @return void
     */
    public function add_JSScript($script='', $unique=false, $event='main') {
        /* Trim script */
        $script = trim($script);

        /* Check script is empty */
        if ($script == '')
            return;

        /* Check event exists */
        $event = strtolower($event);
        if (!isset($this->JSScripts[$event]))
            $this->JSScripts[$event] = array();

        /* Check unique */
        if ($unique and in_array($script, $this->JSScripts[$event]))
            return;

        /* Add script to list */
        $this->JSScripts[$event][] = $script;
    }

    /**
     * Insert JSON structured data to page.
     * @param array $data
     * @return void
     */
    public function add_JSONStructuredData($data) {
        $this->JSStructuredData[] = $data;
    }

    /**
     * Attach CSS style file.
     * @param string $file
     * @param string $condition
     * @return void
     */
    public function add_styleFile($file='', $condition='') {
        /* Check file */
        if ($file == '')
            return;

        /* Check condition exists */
        if (!isset($this->styleFiles[$condition]))
            $this->styleFiles[$condition] = array();

        /* Add file to list */
        $this->styleFiles[$condition][] = $file;
    }

    /**
     * Insert CSS style script.
     * @param string $script
     * @param string $condition
     * @return void
     */
    public function add_styleScript($script='', $condition='') {
        /* Trim script */
        $script = trim($script);

        /* Check script is empty */
        if ($script == '')
            return;

        /* Check condition exists */
        if (!isset($this->styleScripts[$condition]))
            $this->styleScripts[$condition] = array();

        /* Add script to list */
        $this->styleScripts[$condition][] = $script;
    }

    /**
     * Add CSS style file to be inserted as script.
     * @param string $file
     * @param string $condition
     * @return void
     */
    public function add_styleIncludedFile($file='', $condition='') {
        /* Check file */
        if ($file == '')
            return;

        /* Check condition exists */
        if (!isset($this->styleIncludedFiles[$condition]))
            $this->styleIncludedFiles[$condition] = array();

        /* Add file to list */
        $this->styleIncludedFiles[$condition][] = $file;
    }

    /**
     * Insert extra content to HTML head part.
     * @param string $content
     * @return void
     */
    public function add_headExtra($content='') {
        $this->headExtras .= $content."\n";
    }

    /* Shortcuts to some headers and data insertion methods */

    /**
     * Insert RSS feed.
     * @param string $url
     * @param string $title
     * @return void
     */
    public function add_RSSFeed($url='', $title='') {
        $this->add_link('alternate', $url, array('title'=>$title, 'type'=>'application/rss+xml'));
    }

    /**
     * Set a HTTP redirection.
     * @param string $url
     * @param integer $delay
     * @return void
     */
    public function set_redirect($url, $delay=0) {
        if ($delay == 0)
            $this->add_header('Location', $url);
        else
            $this->add_meta('refresh', $delay.';url='.$url, 'http-equiv');
    }

    /* Public writing methods */

    /**
     * Write page header.
     * @return void
     */
    public function write_header() {
        /* Check step */
        if ($this->step != 'init')
            return;

        /* Send page headers */
        $this->send_headers();

        /* Check page is a HTML page */
        if ($this->headers['Content-Type'] !== 'text/html')
            return;

        /* Prepare HTML page */
        $this->prepare();

        /* Write all HTML header elements*/
        $this->write_doctype();
        $this->write_HTMLHead();
        $this->write_title();
        $this->write_metas();
        $this->write_links();
        $this->write_styleScripts();
        $this->write_JSPreScripts();
        $this->write_JSFiles();
        $this->write_JSScripts();
        $this->write_headExtras();

        /* Write HTML body */
        $this->write_HTMLBody();

        /* Update step */
        $this->step = 'body';
    }

    /**
     * Write page footer.
     * @return void
     */
    public function write_footer() {
        /* Send page headers if page is still on init step */
        if ($this->step == 'init')
            $this->send_headers();

        /* Check current step and page is a HTML page */
        if ($this->headers['Content-Type'] !== 'text/html' or $this->step != 'body')
            return;

        /* Write HTML page footer */
        echo "</body>\n";
        echo "</html>\n";

        /* Update step */
        $this->step = 'done';
    }

    /* Private special methods */

    /**
     * Send page headers.
     * @return void
     */
    private function send_headers() {
        /* Send status if different from 200 */
        if ($this->status !== 200) {
            $codes = http_getResponseCodeList();
            header('HTTP/1.1 '.$this->status.' '.$codes[$this->status]);
        }

        /* Send all headers */
        foreach ($this->headers as $name => $value) {
            if ($value !== '')
                header($name.': '.$value);
            else
                header($name);
        }
    }

    /**
     * Prepare page.
     * @return void
     */
    private function prepare() {
        /* Merge JS files in a cache file */

        /* Merge styles files in caches files */

        /* Add styles files to links */
        foreach ($this->styleFiles as $condition => $files) {
            foreach ($files as $file) {
                if ($condition == '')
                    $this->add_link('stylesheet', $file, array('type'=>'text/css'));
                else
                    $this->add_link('stylesheet', $file, array('type'=>'text/css', 'media'=>$condition));
            }
        }
    }

    /* Private writing methods */

    /**
     * Write HTML doctype.
     * @return void
     */
    private function write_doctype() {
        echo "<!DOCTYPE html>\n";
    }

    /**
     * Write HTML head.
     * @return void
     */
    private function write_HTMLHead() {
        global $CaMykS;

        /* Write HTML and head tags with or without language */
        if ($this->language == '')
            echo "<html>\n<head>\n";
        else
            echo "<html lang=\"".$this->language."\">\n<head>\n";

        /* Write base URL if required */
        if ($this->requiresBaseURL)
            echo "<base href=\"".$CaMykS->get_baseURL('AbsoluteLong')."\" />\n";
    }

    /**
     * Write HTML title.
     * @return void
     */
    private function write_title() {
        global $CaMykS;

        /* Prepare title */
        $title = array();

        /* Insert site title */
        if ($CaMykS->get_configValue('Website_Title') == '')
            $title[] = $CaMykS->get_configValue('Website_Title');

        /* If page is home page, display site title first, else display page title first */
        if ($this->title != '') {
            if ($this->isHome)
                $title[] = $this->title;
            else
                array_unshift($title, $this->title);
        }

        /* Write title tag */
        echo "<title>".implode(' - ', $title)."</title>\n";
    }

    /**
     * Write HTML metas.
     * @return void
     */
    private function write_metas() {
        foreach ($this->metas as $key => $meta) {
            if ($key == $meta['Method'])
                echo "<meta ".$key."=\"".$meta['Value']."\" />\n";
            else
                echo "<meta ".$meta['Method']."=\"".$key."\" content=\"".preg_replace('/"/', '\'', $meta['Value'])."\" />\n";
        }
    }

    /**
     * Write page header links.
     * @return void
     */
    private function write_links() {
        foreach ($this->links as $link) {
            echo '<link';
            foreach ($link as $key => $value)
                echo ' '.strtolower($key).'="'.$value.'"';
            echo " />\n";
        }
    }

    /**
     * Write inline styles.
     * @return void
     */
    private function write_styleScripts() {
        /* Check for styles to write */
        if (count($this->styleScripts) == 0 and count($this->styleIncludedFiles) == 0)
            return;

        /* Write style opening tag */
        echo "<style type=\"text/css\">\n";

        /* Insert included files */
        foreach ($this->styleIncludedFiles as $condition => $files) {
            /* Open condition */
            if ($condition !== '')
                echo "@media (".$condition.") { \n";

            /* Add files */
            foreach ($files as $file) {
                require $file;
            }

            /* Check for style scripts with same condition */
            if (isset($this->styleScripts[$condition])) {
                foreach ($this->styleScripts[$condition] as $script)
                    echo $script."\n";

                /* Remove scripts */
                unset($this->styleScripts[$condition]);
            }

            /* Close condition */
            if ($condition !== '')
                echo "} \n";
        }

        /* Write remaining styles */
        foreach ($this->styleScripts as $condition => $scripts) {
            /* Open condition */
            if ($condition !== '')
                echo "@media (".$condition.") { \n";

            /* Add scripts */
            foreach ($scripts as $script)
                echo $script."\n";

            /* Close condition */
            if ($condition !== '')
                echo "} \n";
        }

        /* Write style closing tag */
        echo  "</style>\n";
    }

    /**
     * Write JS script before files loading.
     * @return void
     */
    private function write_JSPreScripts() {
        /* Check pre scripts */
        if (count($this->JSScripts['pre']) == 0)
            return;

        /* Write script opening tag */
        echo "<script>\n";

        /* Add scripts */
        foreach($this->JSScripts['pre'] as $script)
            echo $script."\n";

        /* Write script closing tag */
        echo "</script>\n";

        /* Unload scripts */
        unset($this->JSScripts['pre']);
    }

    /**
     * Write JS files.
     * @return void
     */
    private function write_JSFiles() {
        foreach ($this->JSFiles as $file => $params) {
            echo '<script src="'.$file.'"';
            foreach ($params as $key => $value)
                echo ' '.$key;
                if ($value != '')
                    echo '="'.$value.'"';
            echo "></script>\n";
        }
    }

    /**
     * Write JS Scripts.
     * @return void
     */
    private function write_JSScripts() {
        /* Check scripts */
        if (array_countRecursive($this->JSScripts) == 0)
            return;

        /* Write script opening tag */
        echo "<script>\n";

        /* Read events' scripts */
        foreach($this->JSScripts as $event => $scripts) {
            /* Check scripts */
            if (count($scripts) == 0)
                continue;

            /* Open event */
            if ($event != 'main')
                echo "window.on".$event." = function() {\n";

            /* Add scripts */
            foreach ($scripts as $script)
                echo $script."\n";

            /* Close event */
            if ($event != 'main')
                echo "}\n";
        }

        /* Write script closing tag */
        echo "</script>\n";
    }

    /**
     * Write head extras.
     * @return void
     */
    private function write_headExtras() {
        echo $this->headExtras;
    }

    /**
     * Write body tag.
     * @return void
     */
    private function write_HTMLBody() {
        echo "</head>\n";

        if ($this->bodyClass === '')
            echo "<body>\n";
        else
            echo "<body class=\"".$this->bodyClass."\">\n";
    }
}
?>
