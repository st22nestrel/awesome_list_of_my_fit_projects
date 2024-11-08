<?php
/**
* file: testArgs.php
* autor: Timotej Ponek xponek00
* IPP second assignment 
 */

    //Class that keeps start-up options of script in one structure
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

        public function __construct(){
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

    // Includes functions to print html with results of tests
    class HTMLSupport{
        private $passed = 0;
        private $failed = 0;

        /**
         * Begins Html table
         */
        static function PrintResultStart(){
            echo ('
            <table style="width:auto;">
            <tr>
                <th>Test</th>
                <th>Actual err code(Expected)</th>
                <th>Result</th>
            </tr>');
        }

         /**
         * Outputs succesfull test into html table
         * @param test - full path of test
         * @param errCode - errCode of test
         */
        function PrintSuccessfull(string $test, int $errCode){
            $this->passed++;
            echo ("<tr>
                <td href=\"$test\">$test</td>
                <td style=\"text-align:right;\">$errCode</td>
                <td style=\"color:chartreuse;\">passed</td>
            </tr>");
        }

        /**
         * Outputs unsuccesfull test into html table
         * @param test - full path of test
         * @param errCode - errCode of test
         * @param expectedErrCode - expected errCode or message that otput differs if error codes were ok
         */
        function PrintUnsuccessfull(string $test, int $errCode, $expectedErrCode){
            $this->failed++;
            if(is_string($expectedErrCode)){
                echo ("<tr>
                    <td href=\"$test\">$test</td>
                    <td style=\"color:red;text-align:right;\">$expectedErrCode</td>
                    <td style=\"color:red;\">not passed</td>
                </tr>");
            }
            else{
                echo ("<tr>
                    <td href=\"$test\">$test</td>
                    <td style=\"color:red;text-align:right;\">$errCode($expectedErrCode)</td>
                    <td style=\"color:red;\">not passed</td>
                </tr>");
            }
        }
        
        /**
         * Ends Html table
         */
        static function PrintResultEnd(){
            echo ("</table>");
        }

        /**
         * Prints brief summary of all tests
         */
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