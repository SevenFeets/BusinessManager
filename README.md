## Search Services – CLI Business Manager (C)

This project is a **console-based Business Manager** written in **C**, designed to manage businesses, services, inventory, orders, users, work schedules, and ratings/reviews via a simple **command-line interface (CLI)**.

The code is organised into small, focused modules (e.g. `Business`, `User`, `Service`, `Inventory`, `WorkSchedule`) to keep logic clean and maintainable. Data is stored in plain files and/or binary files (see `ManagerFiles`, `FileHelper`) so the application can persist information between runs.

---

### Features

- **Business management**
  - Create, update, list, and delete businesses
  - Store business details such as name, address, contact information, and categories
- **Address & contact details**
  - Structured address handling via `Address` module
  - Validation utilities via `General` helpers
- **Services & inventory**
  - Manage services a business offers (`Service.c/h`)
  - Track stock/items via `Inventory.c/h`
- **Orders**
  - Represent and manage business orders (`BusinessOrder.c/h`)
- **Users & authentication (if enabled in your build)**
  - User entities and basic user-related operations (`User.c/h`)
- **Work schedules**
  - Store and manage work hours per business (`WorkSchedule.c/h`)
- **Ratings & reviews**
  - Record and show ratings/reviews for businesses (`RatingsReviews.c/h`)
- **File-backed persistence**
  - Helper utilities for reading/writing structured data to files (`ManagerFiles.c/h`, `FileHelper.c/h`)
  - May use binary persistence via `program.bin`, `Compressed.bin`, and/or text files like `program.txt`

> Note: Exact available features depend on how you wire the modules in `Main.c` and `BusinessManager.c`. The above list reflects the general design intent from the module layout.

---

### Project Structure (Key Modules)

- **`Main.c` / `Main.h`**: Entry point; sets up the CLI, main loop, menus and high-level program flow.
- **`BusinessManager.c` / `BusinessManager.h`**: Core orchestration layer for business-related operations, menus, and high-level actions.
- **Domain modules**
  - **`Business.c/h`**: Business entity logic (fields, operations, lookups).
  - **`Address.c/h`**: Address structure and helper functions.
  - **`Service.c/h`**: Services offered by businesses.
  - **`Inventory.c/h`**: Stock, quantities and related operations.
  - **`BusinessOrder.c/h`**: Business orders and order-related logic.
  - **`User.c/h`**: User structures and related operations.
  - **`WorkSchedule.c/h`**: Workday/work-hour definitions and operations.
  - **`RatingsReviews.c/h`**: Rating/review structures and operations.
- **Infrastructure & utilities**
  - **`ManagerFiles.c/h`**: High-level file management (load/save all data, backups, etc.).
  - **`FileHelper.c/h`**: Low-level file helpers and utilities.
  - **`LinkedList.c/h`**: Generic linked list utilities used across modules.
  - **`Date.c/h`**: Date structure and operations.
  - **`General.c/h`**: General helper functions, validation, formatting, etc.
  - **`Macros.h`**: Global macros/constants, configuration flags.

---

### Requirements

- **Windows**
  - Visual Studio with **C/C++** workload (the solution includes `Search_services.sln` / `Search_services.vcxproj`), or
  - Any C compiler that can handle C99 or later (e.g. `gcc` via MinGW or WSL).
- **Other platforms (Linux/macOS)**
  - Standard **C compiler** (`gcc` or `clang`)
  - Compatible `Makefile` or manual compilation command (see below for example).

---

### Building and Running (Windows – Visual Studio)

1. **Open the solution**
   - Double-click `Search_services.sln` or open it from Visual Studio.
2. **Select build configuration**
   - Choose **Debug** or **Release**, and **x64** (or the architecture you prefer).
3. **Build the solution**
   - From the menu: **Build → Build Solution** (or press `Ctrl+Shift+B`).
   - The output executable is typically created in `x64/Debug/Search_services.exe` (or `x64/Release/`).
4. **Run**
   - From Visual Studio: **Debug → Start Without Debugging** (`Ctrl+F5`), or
   - From a terminal/PowerShell:

```bash
cd "i:\WORKING SEARCH BUSINESS VERSION\WORKING SEARCH BUSINESS VERSION\x64\Debug"
.\Search_services.exe
```

---

### Building from the Command Line (Generic C Compiler)

If you are not using Visual Studio, you can compile directly with `gcc` (you may need to adjust the file list or add/remove modules depending on your configuration):

```bash
gcc -std=c99 -Wall -Wextra -O2 ^
  Address.c Business.c BusinessManager.c BusinessOrder.c Date.c FileHelper.c General.c ^
  Inventory.c LinkedList.c Main.c ManagerFiles.c RatingsReviews.c Service.c User.c WorkSchedule.c ^
  -o Search_services.exe
```

On Linux/macOS (Bash-style shell), you can use:

```bash
gcc -std=c99 -Wall -Wextra -O2 \
  Address.c Business.c BusinessManager.c BusinessOrder.c Date.c FileHelper.c General.c \
  Inventory.c LinkedList.c Main.c ManagerFiles.c RatingsReviews.c Service.c User.c WorkSchedule.c \
  -o search_services
```

> Depending on your code, you might need to adjust compiler flags or file lists (for example, if some modules are optional or if you need extra libraries).

---

### Using the CLI

Once you start the program, you typically interact via text menus. While the exact wording depends on `Main.c`, the flow generally looks like:

1. **Main menu**
   - Options for managing businesses, services, inventory, users, orders, ratings, schedules, etc.
2. **Sub-menus**
   - Each area (e.g. Businesses, Inventory) offers operations such as:
     - Add / Edit / Delete
     - List all
     - Search/filter by criteria (e.g. name, category, date)
3. **Persistence**
   - When you exit, data is saved to the corresponding files.
   - On the next startup, the data is loaded back.

If you want to customise or understand the menu flow, start by reading `Main.c` and `BusinessManager.c`.

---

### Data Files

- **`program.bin`, `Compressed.bin`**: Binary data files (may be used for storing compressed or structured data).
- **`program.txt`**: Human-readable text version of data (if your build writes it).
- Additional files may be created at runtime by `ManagerFiles` / `FileHelper`.

Deleting these files may reset stored data, so back them up if they contain important information.

---

### Contributing / Extending

- **Add new entities**: Create a new `*.c` / `*.h` pair (similar to `Service` or `Business`) and plug them into `BusinessManager` and/or `Main.c` menus.
- **Change persistence format**: Modify `ManagerFiles` and `FileHelper` to read/write in your preferred format (e.g. JSON, CSV, different binary layout).
- **Improve UX**: Tweak menu texts, validation messages, and navigation in `Main.c` and `General.c`.

When making changes, keep functions small and focused, and reuse shared helpers in `General`, `LinkedList`, `Date`, and `FileHelper` wherever possible.

---

### License

This repository currently does not declare a specific license. If you plan to share or open-source it, consider adding a `LICENSE` file (for example, MIT or GPL) to clarify usage terms.


