<?php
/**
* file: testArgs.php
* autor: Timotej Ponek xponek00
* IPP second assignment 
 */

    //keeps start-up options of script in one structure
    class testArgs{
        public $directory = null;//cwd default
        public $parseScript = null;
        public $intScript = null;
        public $jexamXml = "/pub/courses/ipp/jexamxml/jexamxml.jar";
        public $jexamCfg= "/pub/courses/ipp/jexamxml/options";

        public $parseOnly = false;
        public $intOnly = false;
        public $recursive = false;

        public $testList = false;
        public $regex = "^.+";

        public function __construct(/* string $cwd */){
            //$this->directory = $cwd;
            $this->parseScript = "parse.php";
            $this->intScript = "interpret.py";

        }
        
    }

/**
 * Prints help about usage of command line options
 */
function printHelp(){
    echo ("Usage: tests functionality of parse.php and interpret.py scripts
    Restrictions: --testlist=\"file\" option cannot be combined with --directory=\"path\":
    Options:
    \t--help: writes out this to console
    \t--directory=\"path\": searches for tests in given directory (cwd implicitly)
    \t--recursive: searches for tests in given directory and all subdirectories
    \t--parse-script=\"file\": path to parse.php (cwd implicitly)
    \t--int-script=\"file\": path to interpret.py (cwd implicitly)
    \t--parse-only: tests only functionality of parse.php
    \t--int-only: tests only functionality of interpret.py
    \t--jexamxml=\"file\": path to JAR archive with A7Soft JExamXML (/pub/courses/ipp/jexamxml/jexamxml.jar implicitly)
    \t--jexamcfg=\"file\": path to file with configuration of A7Soft JExamXML (/pub/courses/ipp/jexamxml/options implicitly)
    \t--testlist=\"file\": used to give test files or directories explicitly via file
    \t--match=\"regex\": searches for tests that comply with given regex (only in cwd without --recursive option)\n");
}

    //Functions to print html with results of tests
    class HTMLSupport{
        //TODO print results for each folder
        //and make temp variables for it
        private $passed = 0;
        private $failed = 0;
        //private $error code ok but output differs..

        function PrintResultStart(string $folder){
            //TODO echo on each line I guess
            //TODO caption folder..
            echo ('
            <table style="width:auto;">
            <tr>
                <th>Test</th>
                <th>result</th>
            </tr>');
            //

        }

        //TODO function that shows where test failed
        function PrintSuccesfull(string $test){
            $this->passed++;
            echo ("<tr>
                <td>$test</td>
                <td style=\"color:chartreuse;\">passed</td>
            </tr>");
        }

        function PrintUnsuccesfull(string $test){
            $this->failed++;
            echo ("<tr>
                <td>$test</td>
                <td style=\"color:red;\">not passed</td>
            </tr>");
        }
        
        function PrintResultEnd(string $folder){
            echo ("</table>");
        }

        function PrintResultSummary(){
            echo ("<div id=\"sum\"></div>
            <caption>Summary</caption>
            <ul style=\"margin-top: 0mm;\">
                <li>passed = $this->passed</li>
                <li>failed = $this->failed</li>
            </ul>");
        }
    }
?>