# Train connection search engine

## About

This project provides functionality to download train connection data in xml format, serialize it from xml to `morphia` POJO classes using `jakarta` library. `morphia` also takes care of storing the objects in the MongoDB library.

### School subject and project
-   *subject*: **UPA - Data Storage and Preparation**

-   *project*: Project, part 1: storing large data in NoSQL databases (Projekt, 1. část: ukládání rozsáhlých dat v NoSQL databázích)

## Usage

### Prerequisites

- Java 11
- MongoDB ([installation manual](https://www.mongodb.com/docs/manual/tutorial/install-mongodb-on-ubuntu/#install-mongodb-community-edition)) - tested with MongoDB: 8.0.3 & Mongosh: 2.3.3


### How to run

Either use Intellij IDEA IDE to run the project or run followng serries of command in terminal:

```bash
# compile project, removing previous compilation result
mvn clean compile
# package project into JAR
mvn package
# run the application
mvn exec:java
```

To install needed java libraries:
```bash
# list java dependencies
mvn dependency:tree
# install java dependencies
mvn install
```

### App


App TUI is in slovak language, here is a litte translation of its main prompt:

- SK:
    ```plaintext
    Pre nahranie jízdních řádů do databázy zadajte '1'
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

### Example search query

```plaintext
Pre nahranie jízdních řádů do databázy zadajte '1'
Pre vyhľadávanie spojov zadajte '2'
Pre ukončenie aplikácie zadajte '3'
Svoj výber potvrďte stlačením tlačidla 'Enter': 
2
Zadejte stanici 'odkud': 
Praha hl. n.
Zadejte stanici 'kam': 
Brno hl. n.
Zadejte 'datum_a_cas' vo formáte 'yyyy-mm-ddThh:mm:ss': 
2022-01-01T12:00:00
```

### Useful mongosh commands

After you installed mongodb you can use its command line interface `mongosh`

1. Enter command line interface:
    ```bash
    mongosh
    ```
2. You can list all present dbs. We will select `morphia_quickstart` created by our app:
    ```bash
    # display dbs
    show dbs
    # select `morphia_quickstart` db
    use morphia_quickstart
    ```
3. List all the collections and contents of the collection:
    ```bash
    # display collections
    show collections
    # show all items from the `Locations` collection
    db.Locations.find()
    ```
4. Display only `PrimaryLocationName` entry of each item:
    ```bash
    db.Locations.find({}, {PrimaryLocationName: 1, _id: 0}).pretty()
    ```
5. Display all the items where `PrimaryLocationName` starts with "Brno":
    ```bash
    db.Locations.find({PrimaryLocationName: /^Brno/}, {PrimaryLocationName: 1, _id: 0}).pretty()
    ```

You probably noticed that we do not get displayed all the items in the collection. You can use `db.Locations.find({}, {PrimaryLocationName: 1, _id: 0}).pretty().toArray()` to display more items, or increase `DBQuery.shellBatchSize = 100` or some larger number.