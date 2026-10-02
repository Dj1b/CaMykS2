<?php
/**
 * @brief Benching library.
 * <br /> Developers can add their own steps in CaMykS running bench, or create their own bench.
 * @details Engine / Object Library
 * @author CaMykS Team <CaMykS.contact@gmail.com>
 * @version 1.0pre5
 * @date Creation: Nov 2019
 * @date Modification: Oct 2026
 * @copyright 2019 - 2026 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2;

/**
 * Bench class.
 */
final class Bench extends CaMykSLibrary {
    /**
     * @var array $params
     * @brief Bench parameters.
     */
    protected array $params = array('output' => 'none');

    /**
     * @var string $status
     * @brief Bench status.
     */
    private string $status = 'idle';

    /**
     * @var array $bench
     * @brief Bench saved steps.
     */
    private array $bench = array();

    /**
     * @var array $results
     * @brief Bench computed results.
     */
    private array $results = array();

    /**
     * Object constructor.
     * @param array $params
     * @parblock - Available parameters are <br />
     * \e string output - \b none / \b shared / \b session / \b logs / \b html  <br />
     * @endparblock
     * @return void
     */
    public function __construct(array $params=array()) {
        parent::__construct($params);
    }

    /* Mark: Public methods */

    /**
     * Start bench.
     * @param string $title
     * @param float $startTime
     * @return void
     */
    public function start(string $title='Start', ?float $startTime=null): void {
        $this->status = 'running';
        $this->bench[] = array('title' => $title, 'time' => (is_null($startTime)? microtime(true) : $startTime));
    }

    /**
     * Add a step to bench.
     * @param string $title
     * @return void
     */
    public function add_step(string $title='Step'): void {
        $this->status = 'running';
        if ($title == 'Step') $title .= '-'.count($this->bench);
        $this->bench[] = array('title' => $title, 'time' => microtime(true));
    }

    /**
     * Stop bench.
     * @param string $title
     * @return void
     */
    public function stop(string $title='Stop'): void {
        $this->status = 'stopped';
        $this->bench[] = array('title' => $title, 'time' => microtime(true));
    }

    /**
     * Export results.
     * @param string $output
     * @return mixed
     */
    public function export(string $output=''): mixed {
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
        return null;
    }

    /**
     * Return results.
     * @return array
     */
    public function get_results(): array {
        return $this->results;
    }

    /**
     * Return current full time.
     * @return integer
     */
    public function get_fullTime(): int {
        /* Check bench has not started */
        if ($this->status == 'idle')
            return 0;

        /* Load start time */
        $start = $this->bench[0]['time'];

        /* Check bench is still running */
        if ($this->status == 'running')
            $stop = microtime(true);

        /* Check check is already stopped */
        elseif ($this->status == 'stopped') {
            $stop = end($this->bench);
            $stop = $stop['time'];

        /* Check undefined status */
        } else
            return 0;

        return $stop - $start;
    }

    /* Mark: Private methods */

    /**
     * Export results to CaMykS shared data.
     * @return void
     */
    private function export_toSharedData(): void {
        global $CaMykS;

        /* save bench to shared data  */
        $CaMykS->set_sharedData('BenchResult', $this->results);
    }

    /**
     * Export results to session.
     * @return void
     */
    private function export_toSession(): void {
        global $CaMykS;

        /* save bench to session */
        $CaMykS->set_sessionValue('BenchResult', $this->results);
    }

    /**
     * Export results to CaMykS logs.
     * @return void
     */
    private function export_toCaMykSLogs(): void {
        global $CaMykS;

        /* log information */
        $CaMykS->log_information($this->params['parent'], $msg1, $msg2, $this->params['file'], $this->params['line']);
    }

    /**
     * Export results to HTML.
     * @return void
     */
    private function export_toHTML(): void {
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
    private function compute_results(): void {

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
