<?php
/**
* file: parse.php
* autor: Timotej Ponek xponek00
* IPP first assignment 
 */
include "stats.php";

$order = 1;

ini_set('display_errors', 'stderr');

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    /**
     * @param inputed - string to check
     * @param inputtedNmb - number of argument in instruction
     */
    function checkVar(string $inputted, int $inputtedNmb){

        if( preg_match ( '/^[LTG]F@[a-zA-Z_\-$&%*!?][a-zA-Z0-9_\-$&%*!?]*$/' , $inputted )){
            echo "\t\t<arg", $inputtedNmb, " type=\"var\">",htmlspecialchars($inputted, ENT_XML1),"</arg", $inputtedNmb,">\n";
            return 1;
        }
        else
            return 0;
    }

    /**
     * @param inputed - string to check
     * @param inputtedNmb - number of argument in instruction
     */
    function checkSymb(string $inputted, int $inputtedNmb){
        
        $inputtedTrim = explode("@", $inputted, 2);

        $nmb = count($inputtedTrim);
        $inputtedTrim[0] = $inputtedTrim[0];

        switch($inputtedTrim[0]){
            case "int":
                if($nmb==2 && preg_match( "/^[\+|\-]?[0-9]*$/" , $inputtedTrim[1]))
                    echo "\t\t<arg", $inputtedNmb, " type=\"int\">",$inputtedTrim[1],"</arg", $inputtedNmb,">\n";
                else if($nmb==1)
                    echo "\t\t<arg", $inputtedNmb, " type=\"int\"></arg", $inputtedNmb,">\n";
                else
                    exit(23);
                break;
            
            case "float":
                if($nmb==2)
                    echo "\t\t<arg", $inputtedNmb, " type=\"float\">",$inputtedTrim[1],"</arg", $inputtedNmb,">\n";
                else if($nmb==1)
                    echo "\t\t<arg", $inputtedNmb, " type=\"float\"></arg", $inputtedNmb,">\n";
                else
                    exit(23);
                break;

            case "string":
                // single quotes are different to double quotes in parsing \x5C character
                if($nmb==2 && preg_match( '/^([^\x00-\x20\x23\x5C]*(\x5C(?:[0-1][0-9]{2}|2[0-4][0-9]|25[0-5]|\d{0,2}$|$))?)*$/u' , $inputtedTrim[1]))
                    echo "\t\t<arg", $inputtedNmb, " type=\"string\">",htmlspecialchars($inputtedTrim[1], ENT_XML1),"</arg", $inputtedNmb,">\n";
                else if($nmb==1)
                    echo "\t\t<arg", $inputtedNmb, " type=\"string\"></arg", $inputtedNmb,">\n";
                else
                    exit(23);
                break;

            case "bool":
                if($nmb==2 && preg_match( "/^true|false$/" , $inputtedTrim[1]))
                    echo "\t\t<arg", $inputtedNmb, " type=\"bool\">", strtolower($inputtedTrim[1]), "</arg", $inputtedNmb,">\n";
                else
                    exit(23);
                break;
            
            case "nil":
                if($nmb==2 && preg_match( "/^nil$/" , $inputtedTrim[1]))
                    echo "\t\t<arg", $inputtedNmb, " type=\"nil\">",$inputtedTrim[1],"</arg", $inputtedNmb,">\n";
                else
                    exit(23);
                break;

            default:
                return checkVar($inputted, $inputtedNmb);
                break;
        }
        return 1;
    }


    /**
     * @param inputed - string to check
     */
    function checkLabel(string $inputted){

        if( preg_match ( "/^[a-zA-Z_\-$&%*!?][a-zA-Z0-9_\-$&%*!?]*$/" , $inputted )){
            global $stats;
            echo "\t\t<arg1 type=\"label\">",htmlspecialchars($inputted, ENT_XML1),"</arg1>\n";
            return 1;
        }
        else
            return 0;
    }

    /**
     * Loops through lines until it finds line where header is expected or encounters EOF
     */
    function checkHeader() {

        global $stats;

        while(($line = trim(fgets(STDIN))) || !feof(STDIN)){
            if($line){
                if($line[0] == '#'){
                    $stats->comments++;
                    continue;
                }
                $lineTmp = explode('#', $line, 2);
                if(count($lineTmp) == 2)
                    $stats->comments++;

                $line = trim($lineTmp[0]);
                if (!strcasecmp($line, ".IPPcode21")){
                    echo "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
                    echo "<program language=\"IPPcode21\">\n";
                    return 1;
                }
                else
                    return 0;
                }
            }
    }

    /**
     * Loops through lines of code until EOF
     */
    function checkInstructions() {

        global $order;
        global $stats;

        while(($line = trim(fgets(STDIN))) || !feof(STDIN)){

            $lineArr = explode('#', $line, 2);
            if(count($lineArr) == 2)
                $stats->comments++;
            $lineArr = preg_split('/\s+/', trim($lineArr[0]));
            $lineArr[0] = strtoupper($lineArr[0]);
            $lineArrSize = count($lineArr);

            switch($lineArr[0]){
                case "DEFVAR":
                case "POPS":
                    echo "\t<instruction order=\"",$order++,"\" opcode=\"",strtoupper($lineArr[0]),"\">\n";
                    if( $lineArrSize>1 && checkVar($lineArr[1], 1)){ //var
                        if( $lineArrSize>2)
                            exit(23);
                    }
                    else
                        exit(23);
                    echo "\t</instruction>\n";
                    break;

                case "MOVE":
                case "NOT":
                case "INT2CHAR":
                case "STRLEN":
                case "TYPE":
                    echo "\t<instruction order=\"",$order++,"\" opcode=\"",strtoupper($lineArr[0]),"\">\n";
                    if($lineArrSize>1 && checkVar($lineArr[1], 1)){ //var
                        if($lineArrSize>2 && checkSymb($lineArr[2], 2)){  //symb
                            if($lineArrSize>3) // unexected symbol
                                exit(23);
                        }
                        else
                            exit(23);
                    }
                    else
                        exit(23);
                    echo "\t</instruction>\n";
                    break;

                case "RETURN":
                    $stats->jumps++;
                case "CREATEFRAME":
                case "PUSHFRAME":                
                case "POPFRAME":
                case "BREAK":
                    echo "\t<instruction order=\"",$order++,"\" opcode=\"",strtoupper($lineArr[0]),"\">\n";
                    if($lineArrSize>1)
                        exit(23);
                    echo "\t</instruction>\n";
                    break;

                case "CALL":
                    echo "\t<instruction order=\"",$order++,"\" opcode=\"",strtoupper($lineArr[0]),"\">\n";
                    if($lineArrSize>1 && checkLabel($lineArr[1])){
                        $stats->checkJump($order, $lineArr[1]);
                        if($lineArrSize>2)
                            exit(23);
                    }
                    else
                        exit(23);
                    echo "\t</instruction>\n";
                    break;

                case "PUSHS":
                case "WRITE":
                case "EXIT":
                case "DPRINT":
                    echo "\t<instruction order=\"",$order++,"\" opcode=\"",strtoupper($lineArr[0]),"\">\n";
                    if($lineArrSize>1 && checkSymb($lineArr[1], 1)){
                        if($lineArrSize>2)
                            exit(23);
                    }
                    else
                        exit(23);
                    echo "\t</instruction>\n";
                    break;
                
                case "ADD":
                case "SUB":
                case "MUL":
                case "IDIV":
                case "LT":
                case "GT":
                case "EQ":
                case "AND":
                case "OR":
                case "STRI2INT":
                case "CONCAT":
                case "GETCHAR":
                case "SETCHAR":
                    echo "\t<instruction order=\"",$order++,"\" opcode=\"",strtoupper($lineArr[0]),"\">\n";
                    if($lineArrSize>1 && checkVar($lineArr[1], 1)){ //var
                        if($lineArrSize>2 && checkSymb($lineArr[2], 2)){  //symb
                            if($lineArrSize>3 && checkSymb($lineArr[3], 3)){  //symb
                                if($lineArrSize>4)
                                    exit(23);
                            }
                            else
                                exit(23);
                        }
                        else
                            exit(23);
                    }
                    else
                        exit(23);
                    echo "\t</instruction>\n";
                    break;

                
                case "READ":
                    echo "\t<instruction order=\"",$order++,"\" opcode=\"",strtoupper($lineArr[0]),"\">\n";
                    if($lineArrSize>2 && checkVar($lineArr[1], 1)){ //var
                        if( preg_match("/^(int|float|bool|string)$/i", $lineArr[2]) ){  //type
                            echo "\t\t<arg2 type=\"type\">",strtolower($lineArr[2]),"</arg2>\n";
                            if($lineArrSize>3) // unexected symbol
                                exit(23);
                        }
                        else
                            exit(23);  
                    }
                    else
                        exit(23);
                    echo "\t</instruction>\n";
                    break;
        
                
                case "JUMP":
                    echo "\t<instruction order=\"",$order++,"\" opcode=\"",strtoupper($lineArr[0]),"\">\n";
                    if($lineArrSize>1 && checkLabel($lineArr[1])){
                        $stats->checkJump($order, $lineArr[1]);
                        if($lineArrSize>2)
                            exit(23);
                    }
                    else
                        exit(23);
                    echo "\t</instruction>\n";
                    break;

                case "LABEL":
                    echo "\t<instruction order=\"",$order++,"\" opcode=\"",strtoupper($lineArr[0]),"\">\n";
                    if($lineArrSize>1 && checkLabel($lineArr[1])){
                        $stats->addLabel($order, $lineArr[1]);
                        if($lineArrSize>2)
                            exit(23);
                    }
                    else
                        exit(23);
                    echo "\t</instruction>\n";
                    break;

                case "JUMPIFEQ":
                case "JUMPIFNEQ":
                    echo "\t<instruction order=\"",$order++,"\" opcode=\"",strtoupper($lineArr[0]),"\">\n";
                    if($lineArrSize>1 && checkLabel($lineArr[1])){  //label
                        $stats->checkJump($order, $lineArr[1]);
                        if($lineArrSize>2 && checkSymb($lineArr[2], 2)){  //symb
                            if($lineArrSize>3 && checkSymb($lineArr[3], 3)){  //symb
                                if($lineArrSize>4)
                                    exit(23);
                            }
                            else
                                exit(23);
                        }
                        else
                            exit(23);
                    }
                    else
                        exit(23);
                    echo "\t</instruction>\n";
                    break;

                default:
                    if(strlen($lineArr[0]) == 0)
                        ;
                    else
                        exit(22); // unknown or wrong OP code
                    break;
            }
        }
        return 1;
    }
 

    /**
     * Loops through argv array and parses defined options
     */
    function checkArgs(){
        global $argc, $argv;
        $options = array_values(preg_grep("/^--help$|^--stats=|^--loc$|^--comments$|^--labels$|^--jumps$|^--fwjumps$|^--backjumps$|^--badjumps$/", $argv));

        $optIt = 0;
        $optSize = count($options);
        global $filesWithStats;
        while($optIt < $optSize){
            switch($options[$optIt]){
                case "--help":
                    if($optSize>1){
                        fprintf(STDERR, "Program launched with wrong arguments, launch with --help to see possible arguments\n");
                        exit(10);
                    }
                    else{
                        printHelp();
                        exit(0);
                    }
                    break;
                case "--loc":
                case "--comments":
                case "--labels":
                case "--jumps":
                case "--fwjumps":
                case "--backjumps":
                case "--badjumps":
                    fprintf(STDERR, "Category of statistic before the occurance of --stats=\"file\" option\n");
                    exit(10);

                default:
                    if(preg_match("/--stats=/", $options[$optIt])){
                        $file = preg_replace("/^--stats=/", '', $options[$optIt]);
                        $file = trim($file, '"');
                        $filesWithStats->addFileName($file);
                        $optIt++;
                        while(($optIt != $optSize) && preg_match("/^--loc$|^--comments$|^--labels$|^--jumps$|^--fwjumps$|^--backjumps$|^--badjumps$/", $options[$optIt])){
                            $filesWithStats->addFileStatToPrint($options[$optIt++]);
                        }
                    }
                    else{
                        fprintf(STDERR, "Program launched with wrong arguments, launch with --help to see possible arguments");
                        exit(10);
                    }
                    break;                   
            }
        }
    }

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // "main"
    $filesWithStats = new Files();
    $stats = new Stats();
    checkArgs();
    if(checkHeader()){
        if(checkInstructions()){
            echo "</program>\n";
            $stats->linesOfCode = $order - 1;
            
            if($filesWithStats->fileNames != null){
                $stats->evalBadJumps();
                $stats->printStats($filesWithStats->fileNames, $filesWithStats->fileStatsToPrint);
            }
        }      
    }
    else
        exit (21);
?>