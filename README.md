# Inverted Search Engine

## 📘 Overview

This project implements a **Command-line based Inverted Search Engine** written in **C**.

An inverted search engine indexes the contents of multiple text files and allows efficient searching of tokens by mapping each word (or token) to the list of files in which it appears, along with the frequency of occurrence in each file.

The project is designed using a **hash table with direct chaining**, supports **database persistence**, and provides a **menu-driven interface** for user interaction.

---

## ⚙️ Features

* Validates and indexes multiple input `.txt` files.
* Builds an inverted index using a **hash table (27 buckets - 26 for alphabets, 1 for special characters)**.
* Supports **case-sensitive word storage** with **case-insensitive hashing**.
* Handles **punctuation-aware tokenization**:

  * Trailing punctuations removed
  * Internal punctuations preserved
  * Standalone punctuations treated as valid tokens
* Displays the complete inverted database in a formatted manner.
* Searches for a word and displays all matching files with occurrence counts.
* Saves the database into a structured backup file.
* Updates (rebuilds) the database from a previously saved backup file.
* Prevents duplicate file indexing during updates.
* Safe exit handling with unsaved database warning.
* Modular and maintainable code structure.

---

## 🧩 Project Structure

```
Inverted_Search/
|
├── .gitignore
├── invsch.h            # Common header file (structures, macros, prototypes)
├── main.c              # Entry point and menu-driven control logic
├── hash.c              # Hashing, word insertion, and word search logic
├── operations.c        # Create, display, search, save, and update database operations
├── helpers.c           # Input validation, utility, and helper functions
├── Makefile            # Build automation using make
└── invsch.exe          # Compiled executable (after build)
```

---

## 🧱 Compilation

### Using Makefile (Recommended)

```bash
make
```

This generates the executable:

````bash
./invsch.exe
````

To clean build files:

```bash
make clean
```

To rebuild from scratch:

```bash
make rebuild
```

---

## 🧠 Usage

### Run the Program

```bash
./invsch.exe file1.txt file2.txt file3.txt
```

Only valid, non-empty `.txt` files are accepted.

---

### Menu Options

```
Menu
1. Create Database
2. Display Database
3. Search Database
4. Update Database
5. Save Database
6. Exit
Enter option : 
```

---

### Create Database

Builds the inverted index from the input files.

```
Database created successfully!
```

---

### Display Database

Displays the entire inverted index in a formatted table showing:

* Hash index
* Word
* Number of files
* File names
* Word frequency per file

---

### Search a Word

```bash
Enter the word to search: hello
```

**Output Example:**

```
Word "hello" found in 2 file(s)
-> file1.txt : 3 times
-> file2.txt : 1 times
```

Search respects:

* Case sensitivity
* Internal punctuation
* Trailing punctuation stripping

---

### Save Database

```bash
Enter the Backup file name : backup.txt
```

The database is stored in a structured format that preserves:

* Hash index
* Word grouping
* File counts

---

### Update Database

```bash
Enter the Backup file name : backup.txt
```

Rebuilds the database from the backup file **and then creates the database for newly provided input files**.

During update:

* Files already present in the database are detected and removed from the input list
* Only **new (non-duplicate) files** are indexed
* Prevents duplicate indexing while allowing database expansion

---

## 🧩 Core Logic

* **`create_db()`**
  Reads validated input files, tokenizes content based on defined rules, and inserts tokens into the hash table.

* **`get_hash_index()`**
  Performs **case-insensitive hashing** on the first character to select the bucket.

* **`insert_word()`**
  Inserts a new word or updates an existing word’s file list and count.

* **`search_word()`**
  Searches for a word inside the appropriate hash bucket.

* **`display_db()`**
  Displays the entire database in a tabular format.

* **`save_db()` / `update_db()`**
  Handles database persistence and reconstruction using a custom file format.

---

## 🧠 Concepts Used

* Hash tables with direct chaining
* Linked lists (single and nested)
* File I/O operations
* Command-line argument validation
* String manipulation
* Modular C programming
* Menu-driven application design
* Memory allocation and safety checks
* Makefile-based build automation

---

## 🚧 Future Improvements

* Implement **phrase search** using positional indexing.
* Add **dynamic hash table resizing**.
* Improve tokenization configurability.
* Implement **database destruction / memory cleanup** on exit.
* Add performance statistics (load factor, collision count).
* Extend support for large files using buffered reading.

---

## 🧑‍💻 Author

**Alvin J**

[GitHub: A1vin-J](https://github.com/A1vin-J)

---

## 📄 License

This project is open-source and available under the **MIT License**.
