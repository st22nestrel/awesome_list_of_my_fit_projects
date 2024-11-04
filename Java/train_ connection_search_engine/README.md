# Train connection search engine

## About

This project provides functionality to download train connection data in xml format, serialize it from xml to `morphia` POJO classes using `jakarta` library. `morphia` also takes care of storing the objects in the MongoDB library.

### School subject and project
-   *subject*: **UPA - Data Storage and Preparation**

-   *project*: Project, part 1: storing large data in NoSQL databases (Projekt, 1. část: ukládání rozsáhlých dat v NoSQL databázích)

## Usage

Either use Intellij IDEA IDE to run the project or run followng serries of command in terminal:

```bash
# compile project, removing previous compilation result
mvn clean compile
# package project into JAR
mvn package
# run the application
mvn exec:java
```

MongoDB must be first installed and set up manually for the project to work. To install needed java libraries:
```bash
# list java dependencies
mvn dependency:tree
# install java dependencies
mvn install
```

## Disclaimer

Project was created in 2022 and [portal.cisjr](https://portal.cisjr.cz/pub/draha/celostatni/szdc/) does no longer provides data for year 2022. Apparently, data format also changed and you cannot simply use this app with data from current year.

## App

App TUI is in slovak language, here is a litte translation of its main prompt:

- SK:
    ```plaintext
    Pre stiahnutie jízdních řádů a na ich nahranie do databázy zadajte '1'
    Pre vyhľadávanie spojov zadajte '2'
    Pre ukončenie aplikácie zadajte '3'
    Svoj výber potvrďte stlačením tlačidla 'Enter': 
    ```
- ENG:
    ```plaintext
    To download timetables and upload them to the database, enter '1'
    To search for connections enter '2'
    To exit the application, enter '3'
    Confirm your selection by pressing the 'Enter' button:
    ```