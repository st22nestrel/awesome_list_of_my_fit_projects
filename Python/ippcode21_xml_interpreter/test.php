<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<style>
table, th, td {
    border: 1px solid black;
    border-collapse: collapse;
}
th, td {
  padding: 2px;
  text-align: left;
}
</style>
<title>Tests results</title>
</head>
<body>

<h2>Tests results</h2>
<a href="#sum">Click here to see summary</a>

<?php
/**
* file: test.php
* autor: Timotej Ponek xponek00
* IPP second assignment 
 */

include "testArgs.php";

    $Args = new testArgs();
    $Stats = new HTMLSupport();
    global $argc, $argv;
    $options = array_values(preg_grep("/^--help$|^--directory=|^--recursive$|^--parse-script=|^--int-script=|^--parse-only$|^--int-only$|^--jexamxml=|^--jexamcfg=|^--testlist=|^--match=/", $argv));

    $Dirs = array();
    $optIt = 0;
    $optSize = count($options);
    global $filesWithStats;
    
    // check arguments
    while($optIt < $optSize){
        switch($options[$optIt]){
            case "--help":
                if($optSize>1){
                    fprintf(STDERR, "Program launched with wrong arguments, launch with --help to see possible arguments");
                    exit(10);
                }
                else{
                    printHelp();
                    exit(0);
                }
                break;

            case "--recursive":
                $Args->recursive = true;
                $optIt++;
                break;

            case "--parse-only":
                if(!$Args->intOnly)
                    $Args->parseOnly = true;
                else
                    exit(0);
                $optIt++;
                break;

            case "--int-only":
                if(!$Args->parseOnly)
                    $Args->intOnly = true;
                else
                    exit(0);
                $optIt++;
                break;
            
            default:
                if(preg_match("/^--directory=|--parse-sript=|^--int-script=|^--jexamxml=|^--jexamcfg=|^--testlist=|^--match=/", $options[$optIt], $match)){
                    $file = preg_replace("/^$match[0]/", '', $options[$optIt]);
                    $file = trim($file, '"');
                    $optIt++;
                    switch($match[0]){
                        case "--directory=":
                            if($Args->testList)
                                exit(10);
                            $Args->directory = $file;
                            break;
                        case "--parse-script=":
                            $Args->parseScript = $file;
                            break;
                        case "--int-script=":
                            $Args->intScript = $file;
                            break;
                        case "--jexamxml=":
                            $Args->jexamXml = $file;
                            break;
                        case "--jexamcfg=":
                            $Args->jexamCfg = $file;
                            break;
                        case "--testlist":
                            if(isset($Args->directory))
                                exit(10);

                            fopen($file, "r");
                            while($line = fgets($file) || !feof(STDIN)){
                                trim($line);
                                
                                //forbid duplicates
                                if(in_array($line, $Dirs))
                                    ;
                                else
                                    array_push($Dirs, $line);                                    
                            }
                            break;

                        case "--match=":
                            $Args->regex = $file;
                            break;
                    
                    }
                }
                else{
                    fprintf(STDERR, "Program launched with wrong arguments, launch with --help to see possible arguments");
                    exit(10);
                }
                break;       
            }
            
    }

    if(!isset($Args->directory)){
        $Args->directory = getcwd();
    }
    if(!$Args->testList){
        array_push($Dirs, $Args->directory);
    }


    // List all files to array $LSfiles
    foreach($Dirs as $dir){{

        if(preg_match('/^.+\.src$/i', $dir)){
            $LSfiles = $dir;
        }
        else{
            try{
                $Directory = new RecursiveDirectoryIterator($dir);
            }
            catch(UnexpectedValueException $th){
                exit(11);
            }
            if($Args->recursive){
                $Directory = new RecursiveIteratorIterator($Directory); // we dont need directory later
            }
            // based on comment from docs https://www.php.net/manual/en/class.recursivedirectoryiterator.php#97228
            try{
                $LSfiles = new RegexIterator($Directory, "/$Args->regex\.src$/i", RecursiveRegexIterator::GET_MATCH);
            }
            catch (InvalidArgumentException $th) {
                exit(11);
            }}
    }

    
    $Stats->PrintResultStart();

    foreach($LSfiles as $file){
        
        $dir = preg_replace("/.src$/", "", $file[0]);
        $in = fopen("$dir.in", "c+");
        $out = fopen("$dir.out", "c+");
        $rc = fopen("$dir.rc", "c+");

        if(!$in || !$out || !$rc)
            exit(12); //11-err input files, 12-err output files;

        $ActualOut;
        $errorCode;
        $JexamErrcode;
        $DiffErrcode;


        if($Args->parseOnly){

            $tempName = tempnam(preg_replace('/\/.+$/', '', $dir), "jexam");
            exec("php $Args->parseScript < $file[0] > $tempName", $ActualOut, $errorCode);

            $errorCodeExp = trim(fgets($rc));

            if(strlen($errorCodeExp)){
                if(intval($errorCodeExp) == $errorCode){
                    if(!$errorCode){

                        exec("java -jar $Args->jexamXml $dir.out $tempName $tempName.xml $Args->jexamCfg", $bin, $JexamErrcode);

                        if(!$JexamErrcode)
                            $Stats->PrintSuccessfull($file[0], $errorCode);

                        else
                            $Stats->PrintUnsuccessfull($file[0], $errorCode, intval($errorCodeExp));

                        unlink("$tempName.xml");
                    }
                    else
                        $Stats->PrintSuccessfull($file[0], $errorCode);
                }

                else
                    $Stats->PrintUnsuccessfull($file[0], $errorCode, intval($errorCodeExp));
            }
            // if file was null
            else{
                fwrite($rc, 0);

                if(!$errorCode){ // errorCode == 0 

                    exec("java -jar $Args->jexamXml $dir.out $tempName $tempName.xml $Args->jexamCfg", $bin, $JexamErrcode);

                    if(!$JexamErrcode)
                        $Stats->PrintSuccessfull($file[0], $errorCode);
                    else
                        $Stats->PrintUnsuccessfull($file[0], $errorCode, "output differs from expected");

                    unlink("$tempName.xml");
                }
                else
                    $Stats->PrintUnsuccessfull($file[0], $errorCode, 0);
            }

            
        }
        else if($Args->intOnly){
            
            $tempName = tempnam(preg_replace('/\/.+$/', '', $dir), "jexam");
            $isItOk = exec("python3.8 $Args->intScript --source=$dir.src --input=$dir.in > $tempName", $ActualOut, $errorCode);
            $errorCodeExp = trim(fgets($rc));

            if(strlen($errorCodeExp)){
                if(intval($errorCodeExp) == $errorCode){
                    if(!$errorCode){

                        //0 - no diff
                        //1 - some diff
                        //2 andSoOn - trouble
                        exec("diff $tempName $dir.out", $bin, $DiffErrcode);
                        if(!$DiffErrcode)
                            $Stats->PrintSuccessfull($file[0], $errorCode);
                        else
                            $Stats->PrintUnsuccessfull($file[0], $errorCode, intval($errorCodeExp));
                    }
                    else
                        $Stats->PrintSuccessfull($file[0], $errorCode);
                }
                else
                    $Stats->PrintUnsuccessfull($file[0], $errorCode, intval($errorCodeExp));
            }

            // if file was null
            else{
                fwrite($rc, 0);

                $bin;
                if(!$errorCode){ 
                    exec("diff $tempName $dir.out", $bin, $DiffErrcode);
                    if(!$DiffErrcode)
                        $Stats->PrintSuccessfull($file[0], $errorCode);
                    else
                        $Stats->PrintUnsuccessfull($file[0], $errorCode, "output differs from expected");
                }
                else
                    $Stats->PrintUnsuccessfull($file[0], $errorCode, 0);
            }

        }

        // run parse and interpretation
        else{
            $tempName = tempnam(preg_replace('/\/.+$/', '', $dir), "jexam");

            exec("php $Args->parseScript < $file[0] > $tempName", $ActualOut, $errorCode);

            //parsing was ok
            if(!$errorCode){
                $tempIpp = tempnam(preg_replace('/\/.+$/', '', $dir), "IppCode");
                exec("python3.8 $Args->intScript --source=$tempName --input=$dir.in > $tempIpp", $ActualOut, $errorCode);
                $errorCodeExp = trim(fgets($rc));

                if(strlen($errorCodeExp)){ #errCode exists
                    if(intval($errorCodeExp) == $errorCode){
                        if(!$errorCode){ # check out

                            //0 - no diff
                            //1 - some diff
                            //2 andSoOn - trouble
                            exec("diff $tempIpp $dir.out", $bin, $DiffErrcode);
                            if(!$DiffErrcode)
                                $Stats->PrintSuccessfull($file[0], $errorCode);
                            else
                                $Stats->PrintUnsuccessfull($file[0], $errorCode, intval($errorCodeExp));
                        }
                        else
                            $Stats->PrintSuccessfull($file[0], $errorCode);
                    }
                    else
                        $Stats->PrintUnsuccessfull($file[0], $errorCode, intval($errorCodeExp));
                }

                // if file was null
                else{
                    fwrite($rc, 0);

                    if(!$errorCode){ 
                        exec("diff $tempIpp $dir.out", $bin, $DiffErrcode);
                        if(!$DiffErrcode)
                            $Stats->PrintSuccessfull($file[0], $errorCode);
                        else
                            $Stats->PrintUnsuccessfull($file[0], $errorCode, "output differs from expected");
                    }
                    else
                        $Stats->PrintUnsuccessfull($file[0], $errorCode, 0);
                }
                unlink($tempIpp);
                
            }
            else
                $Stats->PrintUnsuccessfull($file[0], $errorCode, "failed on parsing");
        }
    
        fclose($in);
        fclose($out);
        fclose($rc);
        unlink($tempName);
    }

}
    $Stats->PrintResultEnd();
    $Stats->PrintResultSummary();

?>

</body>
</html>