# handy_list - single header C library for generic lists. 

### Generic Lists using C Macros

In standard C, it is hard to make a generic list and usually it involves 'void*' pointers. This library uses **C Preprocessor Macros** instead of `void*` pointers. The macros generate custom node structures at compile time. It uses linked lists as they can grow and shrink dynamically at runtime, using memory only when needed.

This approach ensures that list nodes hold exact data types and check types at compile time.

### Runtime Void* Linked Lists vs. Macro-Generated Generic Linked Lists

| Feature | `void*` Linked List | Macro-Generated Generic Linked List |
| :--- | :--- | :--- |
| **Type Safety** | **Low** — Accepts any pointer type, which can cause hidden bugs and crashes at runtime. | **High** — The compiler checks the type and gives an error if you link the wrong data types. |
| **Memory Allocations** | **High** — Needs separate allocations for the node structure and the actual data payload. | **Low** — The data and the node pointers (`next`/`prev`) are stored together in one allocation. |
| **Ease of Use** | **Hard** — Requires manual type casting and confusing pointer steps to read data. | **Easy** — Allows direct access to data fields using normal C struct arrow (`->`) syntax. |
| **Code Size** | **Small** — Uses one compiled block of code no matter how many data types you use. | **Larger** — The compiler creates new node structures for each unique data type you define. |



### 🧪 Running Test Cases

Script test.sh compiles and runs unit tests. File test.c has examples of how to use the library. 


### 📦 How to Link Your Application

Include the header handy_list.h in your project. 
To instantiate lists and see examples, refer to unit tests. 

