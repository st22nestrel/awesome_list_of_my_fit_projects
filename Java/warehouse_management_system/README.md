# Warehouse Management System

> [!IMPORTANT]  
> This is a team project, imported as a submodule. [Original repository](https://github.com/timom26/IJA-basic-warehouse-management.git)

## About

This project is a Java-based warehouse management system that provides functionality to:

- Visualize warehouse layout with shelves
- Display goods information via tooltips
- Manage shopping cart with maximum item count
- Draw warehouse grid layout
- Handle warehouse structure with rows and columns

### School subject and project
-   *subject*: **IJA - Java Programming Language**
-   *project*: Team project, Warehouse Management System

## Usage

### Prerequisites

- Java JDK 8 or higher
- Apache Ant build system
- JavaFX (included via build system)

### How to run

1. Get required libraries:
    ```bash
    ant get-libs
    ```

2. Build the project and generate documentation:
    ```bash
    ant compile
    ```

3. Run the application:
    ```bash
    ant run
    ```

Additional Commands:
- Clean build artifacts:
    ```bash
    ant clean
    ```

- Generate JavaDoc documentation:
    ```bash
    ant generate-javadoc
    ```

### Features

- Interactive warehouse visualization
- Grid-based layout system
- Tooltip information for shelf contents
- Shopping cart functionality with item limit (10 items)
- Dynamic warehouse size configuration (rows/columns)

#### Warehouse Display

The warehouse interface shows:

- Shelf layout in grid format
- Color coding for shelf contents (red for occupied shelves)
- Hover tooltips showing detailed shelf contents
- Obstacle grid visualization

## Project Structure

Key components:

- `src/store/ShoppingCart.java`: Shopping cart implementation
- `src/App/WarehouseController.java`: Main warehouse visualization controller
- `src/Reader/WarehouseStruct.java`: Warehouse structure management