# WebDomCompiler to Shared Library Compiler

This project provides a simple tool to convert HTML, CSS, and JavaScript files into a **shared library** 
(`.so`) that can be dynamically loaded in C++ applications.

## 🚀 Features

-   Converts **HTML, CSS, and JavaScript** into a C++ shared library.

-   Generates `.so` files that can be loaded dynamically.

-   Uses C++17 and `std::filesystem` for file management.

-   Supports easy compilation using **GCC and Makefile**.


---

## 📌 Requirements

-   C++17 or later

-   GCC (g++)

-   Make

---

## 💻 Installation & Compilation

1.  Clone this repository:
```bash
git clone https://github.com/TonyGuerra122/web-dom-compiler.git
cd web-dom-compiler.git
```

2.  Build the project using **Makefile**:
```bash
make
```

---

## 📄 Usage
To convert a file into a shared library, run:
```bash
./bin/Program <file> <type>
```
Where:
-   `<file>` is the path to your HTML, CSS, or JavaScript file.
-   `<type>` is either html, css, or js.

### Example
```bash
./bin/Program index.html html
```
This will generate a `libhtml.so` file.

---

## 🔧 How It Works
1.  Reads the content of the **HTML, CSS, or JavaScript** file.

2.  Generates a **C++ source file** with an embedded string.

3.  Compiles the source file into a **.so** shared library.

4.  The **.so** file can then be dynamically loaded in a C++ program.

### 📂 Project Structure
```
web-dom-compiler/
│── bin/             # Compiled binaries
│── includes/        # Header files
│── src/             # Source code
│── Makefile         # Build automation
│── README.md        # Project documentation
```

### 🛠 Example C++ Code to Load the .so Library
To load and use the generated `.so` file in another C++ program:
```cpp
#include <iostream>
#include <dlfcn.h>

int main() {
    void* handle = dlopen("./libhtml.so", RTLD_LAZY);
    if (!handle) {
        std::cerr << "Error loading .so: " << dlerror() << std::endl;
        return 1;
    }

    auto get_html = (const char* (*)()) dlsym(handle, "get_html");
    if (!get_html) {
        std::cerr << "Error loading function: " << dlerror() << std::endl;
        dlclose(handle);
        return 1;
    }

    std::cout << "HTML from .so: " << get_html() << std::endl;
    dlclose(handle);
    return 0;
}
```

---

## 📜 License

This project is licensed under the MIT License.

---

## 🤝 Contributing

Feel free to fork this project and submit pull requests.

---

📧 Contact

For any questions, contact **[guerra.anthony122@gmail.com](mailto:guerra.anthony122@gmail.com)**.