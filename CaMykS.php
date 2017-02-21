<?php
/*
 * CaMykS Engine
 * Developed by		: camyks.net
 * Author		    : CaMykS Team <camyks.contact@gmail.com>
 * Object Version	: 2.0a
 * Object Type      : Engine / Main Object
 * Creation Date    : Feb 2017
 * Last Modif Date  : Feb 2017
 *
 * CaMykS engine object
 */
 
class CaMykS {
 
   
  /*
   * constructor
   * @return void
   * @access public
   */
  public __construct() {
   
  }
   
  /*
   * run CaMykS engine
   * @param integer $mode to run
   * @return void
   * @access public
   */
  public run($mode) {
  
  }
}

/* check for manager execution */
if ($_REQUEST['SCRIPT_FILENAME'] === 'CaMykS.php') {
  $CaMykS = new CaMykS();
  /* execute CaMykS engine */
  $CaMykS->execute('manager');
}