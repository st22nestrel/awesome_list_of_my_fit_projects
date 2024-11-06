# Faneuil Hall Problem

## About

This project implements a solution to the Faneuil Hall synchronization problem using processes and semaphores. The implementation simulates immigrants arriving at a building to get certificates, with a judge periodically entering to confirm their applications.

The program simulates three types of processes:

- Immigrant generator - creates immigrant processes
- Immigrants - enter building and request certificates
- Judge - enters periodically to confirm certificates

Key features:

- Process synchronization using semaphores
- Shared memory for process communication
- Logging of all actions to output file
- Configurable timing parameters

### School subject and project

-   *subject*: **IOS - Operating Systems**
-   *project*: Project 2 (Projekt 2)

## Usage

### Prerequisites
- GCC compiler
- POSIX compliant operating system (Linux/Unix)

### How to run

1. Compile the project
    ```bash
    make
    ```


2. Run the binary
    ```bash
    ./proj2 PI IG JG IT JT
    ```

Program arguments:

- `PI` - Number of immigrants (>0)
- `IG` - Maximum time (ms) for generating a new immigrant (0-2000)
- `JG` - Maximum time (ms) before judge enters building (0-2000)
- `IT` - Maximum time (ms) immigrant needs for certificate processing (0-2000)
- `JT` - Maximum time (ms) judge needs for confirmation (0-2000)

### Output

The program creates a proj2.out file containing a detailed log of all actions in the format:

```plaintext
<action_number> : <process_type> : <action> : <NE> : <NC> : <NB>
```

Where:

- NE = number of immigrants entered
- NC = number of immigrants checked
- NB = number of immigrants in building

### Example Usage

```bash
./proj2 5 100 100 100 100
```

This runs the simulation with:

- 5 immigrants
- Maximum 100ms delays for all timing parameters

The output will be written to proj2.out showing the sequence of events as immigrants enter, get checked, and receive certificates while the judge periodically enters to confirm their applications.

> [!TODO]
> patch this error 
>```
>free(): double free detected in tcache 2
>Aborted (core dumped)
>```