# Store Management System

> [!CAUTION]
> This project is not runnable as it is missing some source files

## About

This project implements a basic store management system that provides functionality to:

- Manage store goods and items
- Track goods inventory
- Handle store shelf operations
- Manage goods dates and quantities

The project implements several key classes:

- `StoreGoods` - Represents a type of goods with its items
- `StoreGoodsItem` - Represents individual items of goods with dates
- `StoreShelf` - Implements shelf management functionality

### School subject and project
-   *subject*: **IJA - Java Programming Language**
-   *project*: Homework 1 - Store Management Implementation

## Usage

### Prerequisites

- Java Development Kit (JDK)
- JUnit for running tests

### Project Structure

The main implementation is located in `src/ija/ija2020/homework1/`.

### Running Tests

The project includes test cases in Homework1Test.java that verify:

- Basic goods and items management
- Shelf operations
- Reference implementation comparison

To run the tests:
```bash
# compile (package the JAR)
javac -cp junit.jar src/ija/ija2020/homework1/*.java
# execute
java -cp junit.jar:src org.junit.runner.JUnitCore ija.ija2020.homework1.Homework1Test
```

### Tests Details
The test suite includes:

- `test01()` - Verifies basic implementation of StoreGoods and StoreGoodsItem
- `test02()` - Verifies implementation of StoreShelf
- `test03()` - Verifies against reference implementation

> [!NOTE] 
> The reference implementation is provided in a JAR archive and should be imported, not implemented.