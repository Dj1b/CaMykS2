<?php
/**
 * @brief Benching library.
 * <br /> Developers can add their own steps in CaMykS running bench, or create their own bench.
 * @details Engine / Object Library
 * @author CaMykS Team <CaMykS.contact@gmail.com>
 * @version 1.0.1
 * @date Creation: Nov 2019
 * @date Modification: Nov 2019
 * @copyright 2019 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

/**
 * CBench class.
 */
final class CBench {
    /**
     * @var array $params
     * @brief CBench parameters.
     */
    private $params = array('output'    => 'none');

    /**
     * @var string $status
     * @brief CBench status.
     */
    private $status = 'idle';

    /**
     * @var array $bench
     * @brief CBench saved steps.
     */
    private $bench = array();

    /**
     * @var array $results
     * @brief CBench computed results.
     */
    private $results = array();

    /**
     * Object constructor.
     * @param array $params
     * @parblock - Available parameters are <br />
     * \e string output - \b none / \b shared / \b session / \b logs / \b html  <br />
     * @endparblock
     * @return void
     */
    public function __construct($params=array()) {
        /* Merge params */
        $this->params = array_merge($this->params, $params);
    }

    /* public methods */

    /**
     * Start bench.
     * @param string $title
     * @return void
     */
    public function start($title='Start', $startTime=null) {
        $this->status = 'running';
        $this->bench[] = array('title' => $title, 'time' => (is_null($startTime)? microtime(true) : $startTime));
    }

    /**
     * Add a step to bench.
     * @param string $title
     * @return void
     */
    public function add_step($title='Step') {
        $this->status = 'running';
        if ($title == 'Step') $title .= '-'.count($this->bench);
        $this->bench[] = array('title' => $title, 'time' => microtime(true));
    }

    /**
     * Stop bench.
     * @param string $title
     * @return void
     */
    public function stop($title='Stop') {
        $this->status = 'stopped';
        $this->bench[] = array('title' => $title, 'time' => microtime(true));
    }

    /**
     * Export results.
     * @param string $output
     * @return mixed
     */
    public function export($output='') {
        /* Check if bench is stopped */
        if ($this->status != 'stopped')
            $this->stop();

        /* Check output method */
        $method = $output != '' ? $output : $this->params['output'];

        /* Compute result */
        $this->compute_results();

        /* Export case of method */
        switch ($method) {
            case 'shared': return $this->export_toSharedData();
            case 'session': return $this->export_toSession();
            case 'logs': return $this->export_toCaMykSLogs();
            case 'html': return $this->export_toHTML();
            case 'none': default : return $this->get_results();
        }
        return;
    }

    /**
     * Return results.
     * @return array
     */
    public function get_results() {
        return $this->results;
    }

    /* private methods */

    /**
     * Export results to CaMykS shared data.
     * @return void
     */
    private function export_toSharedData() {
        global $CaMykS;

        /* save bench to shared data  */
        $CaMykS->set_sharedData('CBenchResult', $this->results);
    }

    /**
     * Export results to session.
     * @return void
     */
    private function export_toSession() {
        global $CaMykS;

        /* save bench to session */
        $CaMykS->set_sessionValue('CBenchResult', $this->results);
    }

    /**
     * Export results to CaMykS logs.
     * @return void
     */
    private function export_toCaMykSLogs() {
        global $CaMykS;

        /* log information */
        $CaMykS->log_information($this->params['parent'], $msg1, $msg2, $this->params['file'], $this->params['line']);
    }

    /**
     * Export results to HTML.
     * @return void
     */
    private function export_toHTML() {
        /* display results as HTML */
        echo '<div style="display:none">'."\n";
        foreach ($this->results as $r) {
            echo $r['title'] . ' : '.$r['time']."<br />\n";
        }
        echo '</div>';
    }

    /**
     * Compute results.
     * @return void
     */
    private function compute_results() {

        /* Read all steps to build result */
        for ($i=0; $i<count($this->bench)-1; $i++) {
            $t = (int)(($this->bench[$i+1]['time']-$this->bench[$i]['time'])*1000);
            $this->results[$i] = array('title' => $this->bench[$i]['title'], 'time' => $t.' ms');
        }

        /* Build total value */
        $t = (int)(($this->bench[count($this->bench)-1]['time']-$this->bench[0]['time'])*1000);
        $this->results[$i] = array('title' => 'Total', 'time' => $t.' ms');
    }
}
?>
