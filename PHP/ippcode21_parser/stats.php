<?php
/**
* file: stats.php
* autor: Timotej Ponek xponek00
* IPP first assignment 
 */

// enum based on this answer from StackOverflow
// https://stackoverflow.com/a/254543
// I use it for storing categories of statistics into array as int
abstract class Options{
    const loc = 0;
    const comments = 1;
    const labels = 2;
    const jumps = 3;
    const fwjumps = 4;
    const backjumps = 5;
    const badjumps = 6;
}


/**
 * Prints help about usage of command line options
 */
function printHelp(){
    echo ("Usage: reads code from stdin and outputs code in xml representation to stdout
    Options:
    \t--help: writes out this to console
    \t--stats=\"file\": write stats selected by options following this to the given file, if no stats are selected -> error 10
    \tStats that can be selected:
    \t\t--loc: number of lines with asembler instructions
    \t\t--comments: number of lines with comments
    \t\t--labels: number of defined labels
    \t\t--jumps: number of all jump instructions (conditional/unconditional, calls and returns from functions)
    \t\t--fwjumps: number of forward jumps
    \t\t--backjumps: number of back jumps
    \t\t--badjumps: number of jumps to non-existent labels\n");
}

// Class that keeps track of statistics needed for STATP extension
class Stats{

    /**
     * Categories of statistics
     */
    public int $linesOfCode=0;
    public int $comments=0;
    private int $labels=0;
    public int $jumps=0;
    private int $forwardJumps=0;
    private int $backJumps=0;
    private int $badJumps=0;


    public $NamedLabels = array();
    public $NotDefinedLabels = array();

    /**
     * Adds Label to NamedLabels and if label was already encountered
     * adds number of encounters to \forwardjumps
     * @param line - lineOfCode where label was encountered
     * @param name - name of label
     */
    function addLabel(int $line, string $name)
    {
        if(array_key_exists($name, $this->NotDefinedLabels)){
            $this->forwardJumps += $this->NotDefinedLabels[$name];
            unset($this->NotDefinedLabels[$name]);
        }
        $this->NamedLabels[$name] = $line;
        $this->labels++;
    }

    /**
     * check whether we are jumping on defined label and adjusts 
     * the values backjump and jumps according to it
     * @param line - lineOfCode where label was encountered
     * @param label - name of label
     */
    function checkJump(int $line, string $label)
    {
        if(array_key_exists($label, $this->NamedLabels)){
            $isDefined = $this->NamedLabels[$label];
            if($isDefined < 0){
                $this->NotDefinedLabels[$label]++;    
            }
            else if($isDefined < $line){
                $this->backJumps++;
            }
        }
        else{
            $this->NamedLabels[$label] = -$line;
            $this->NotDefinedLabels[$label] = 1;
        }
        $this->jumps++;
    }

    // evaluates badjumps at the and of code parsing
    function evalBadJumps()
    {
        foreach($this->NotDefinedLabels as $val){
            $this->badJumps += $val;
        }
    }

    /**
     * Prints stats to files acquired from arguments
     * @param files - array with filenames
     * @param stats - array with categories of statistics
     */
    function printStats(array $files, array $stats){
        $nmbOfFiles = count($files);
        $bytes = 0;
        for($i = 0; $i < $nmbOfFiles; $i++){
            $handle = fopen($files[$i], "w");
            
            if($handle == false){
                exit(12); //error opening output files
            }
            
            foreach($stats[$i] as $toBePrinted){
                switch($toBePrinted){
                    case Options::loc:
                        fwrite($handle, "$this->linesOfCode\n");
                        break;
                    case Options::comments:
                        fwrite($handle, "$this->comments\n");
                        break;
                    case Options::labels:
                        fprintf($handle, "$this->labels\n");
                        break;
                    case Options::jumps:
                        fprintf($handle, "$this->jumps\n");
                        break;
                    case Options::fwjumps:
                        fprintf($handle, "$this->forwardJumps\n");
                        break;
                    case Options::backjumps:
                        fprintf($handle, "$this->backJumps\n");
                        break;
                    case Options::badjumps:
                        fprintf($handle, "$this->badJumps\n");
                        break;
                    default:
                        break;
                }
            }
            fclose($handle);
        }
    }
}


// Class for storing filenames and stats categories that will be printed to them
class Files{
    public $fileNames = array();

    public $fileStatsToPrint = array();


    /**
     * Stores filename where given categories of statistics are going to be printed
     * @param name - filename
     */
    function addFileName(string $name){
        if(in_array($name, $this->fileNames)){
            fprintf(STDERR, "Error: Attempt to write multiple categories of statistics to same file more than once!\n" );
            exit(12);
        }
        else
            array_push($this->fileNames, $name);
            
    }
    
    /**
     * Stores statistics that are later going to be printed into a corresponding file
     * @param stat - category of statistic
     */
    function addFileStatToPrint(string $stat){
        $where = count($this->fileNames) - 1;

        if(!isset($this->fileStatsToPrint[$where])){
            $this->fileStatsToPrint[$where] = array();
        }

        switch($stat){
            case "--loc":
                array_push($this->fileStatsToPrint[$where], Options::loc);
                break;
            case "--comments":
                array_push($this->fileStatsToPrint[$where], Options::comments);
                break;
            case "--labels":
                array_push($this->fileStatsToPrint[$where], Options::labels);
                break;
            case "--jumps":
                array_push($this->fileStatsToPrint[$where], Options::jumps);
                break;
            case "--fwjumps":
                array_push($this->fileStatsToPrint[$where], Options::fwjumps);
                break;
            case "--backjumps":
                array_push($this->fileStatsToPrint[$where], Options::backjumps);
                break;
            case "--badjumps":
                array_push($this->fileStatsToPrint[$where], Options::badjumps);
                break;
            default:
                break;
        }
    }
}
?>