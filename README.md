# Electronics Service Management Application

## Overview

The application is launched by running the `run.sh` script inside the `GUI` folder. This initializes the graphical user interface built using the **Qt** library. The interface provides access to all functionalities defined in the application menu, in accordance with the project requirements.

---

## Features

### Employee Management

The application supports full employee management, including:
- Adding new employees
- Editing existing employee information
- Removing employees

When hiring a new person, the corresponding objects are created using the **Factory Method** design pattern, based on the employee type. After validating the input data, the employee is added to the application's internal data structures. Name updates and employee dismissal are handled through dedicated functionalities.

---

### Repair Request Processing

Repair requests are processed in two distinct stages:

1. **Initial Allocation**  
   Valid repair requests are assigned to available technicians. Requests that cannot be immediately allocated are placed in a waiting list.

2. **Real-Time Simulation**  
   Repair operations are simulated in real time as requests are processed and completed. Every modification of the waiting list triggers an update of the corresponding file.

---

### Invalid Requests and Reports

Invalid requests are displayed by directly reading data from the associated output file, from which appliance brands, models, and their frequency of occurrence are extracted.

All other reports are generated using the locally stored employee list, ensuring that the displayed information always reflects the most recent changes.

---

## Architecture and Design

### Core Logic

The `Service` class, responsible for managing the core application logic, is implemented using the **Singleton** design pattern. This ensures the existence of a single service instance throughout the entire runtime of the application.

### Extensibility

The application menu also includes options for adding or removing appliance brands and models. This functionality reuses the existing data management methods, ensuring consistency and automatic updates of the internal structures.

---

## Technical Considerations

### Smart Pointers

A technical aspect that required special attention was the use of `std::unique_ptr`. As this concept was recently introduced, integrating it into the application architecture required adapting object management strategies and defining auxiliary methods to enable safe resource transfer while respecting ownership semantics.

### Time Management

The use of the C++ time library required an adjustment period, particularly in understanding how timestamps are represented and manipulated. This functionality is essential for simulating the real-time processing of repair requests.

### Requirements Interpretation

Some project requirements were expressed in a general manner, which required additional design decisions to ensure a coherent and functional implementation.

### Edge Case Handling

Identifying and handling edge cases was a crucial part of the development process, given the wide range of possible scenarios that may occur in employee and request management. Dedicated tests were created to validate the application's behavior in unexpected situations.

### Debugging

The debugging process was influenced by the complexity of compiler-generated error messages. As a result, special attention was given to writing clean, well-structured code to minimize such issues.

---

## Design Patterns

During development, several design patterns were analyzed with the goal of extending the application architecture. However, it was determined that introducing additional patterns would not provide a clear functional benefit and would instead unnecessarily increase code complexity.

Only design patterns that naturally fit the problem domain and contribute to code clarity and maintainability were used, without compromising readability or simplicity.

---

## Coding Style

By personal choice, `using namespace std;` was deliberately avoided, even though it would have reduced typing effort. This required explicitly prefixing all standard library components with `std::`, and any omission was immediately flagged by the compiler.

---

## Testing and Validation

To validate the application, an isolated testing environment was created in the `tests/` directory. This environment includes three primary input files:
- **Employees**
- **Service configuration** (brands and models)
- **Repair request flow**

The CSV format was chosen due to its standardization and ease of processing structured data.

---

### Test Coverage

The test suite focuses on two main areas:

#### 1. Data Validation and Filtering

Input files intentionally contain invalid data mixed with valid entries, such as:
- Invalid personal identification numbers (CNP)
- Employees under the legal working age (16 years)
- Incorrect date formats
- Empty lines
- Requests for non-existent appliances

The parsing mechanism detects these issues, displays explicit warning messages in the console without stopping program execution, and automatically removes corrupted data from memory.

Invalid or irreparable requests are redirected to dedicated output files (e.g., `cereriInvalide.csv`), ensuring clear error traceability.

#### 2. Logic Verification

The tests monitor data integrity in real time. For example:
- When an employee is dismissed, their existence is verified and the number of active employees is updated accordingly.
- The simulation of valid repair requests ensures correct allocation to technicians, respecting availability and specialization constraints.
