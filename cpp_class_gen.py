import os
import sys

def generate_class_files(class_names, output_dir):
    # Create the specified output directory if it doesn't exist
    os.makedirs(output_dir, exist_ok=True)

    first_template_hpp = """\
#ifndef {guard}
# define {guard}

# include <iostream>
# include <string.h>
# include <unistd.h>
# include <cstdlib>
# include <ctime>
# define RED "\\033[31m"
# define GREEN "\\033[32m"
# define PURPLE "\\033[35m"
# define YELLOW "\\033[33m"
# define WHITE "\\033[37m"
# define CYAN "\\033[36m"
# define BOLD_RED "\\033[1;31m"
# define BOLD_GREEN "\\033[1;32m"
# define BOLD_PURPLE "\\033[1;35m"
# define BOLD_YELLOW "\\033[1;33m"
# define BOLD_WHITE 	"\\033[1;37m"
# define BOLD_CYAN "\\033[1;36m"
# define RESET "\\033[0m"

class {class_name} {{
public:
    {class_name}(); // Constructor
    {class_name}(const {class_name}& copy);//Copy constructor
    {class_name}& operator= (const {class_name}& copy); //Copy assignment constructor
    ~{class_name}(); // Destructor

private:
    // Add private members here
}};

#endif // {guard}
"""

    template_hpp = """\
#ifndef {guard}
# define {guard}

# include "{last_class_name}.hpp"

class {class_name} {{
public:
    {class_name}(); // Constructor
    {class_name}(const {class_name}& copy);//Copy constructor
    {class_name}& operator= (const {class_name}& copy); //Copy assignment constructor
    ~{class_name}(); // Destructor

private:
    // Add private members here
}};

#endif // {guard}
"""

    template_cpp = """\
#include "{includes_dir}/{class_name}.hpp"

{class_name}::{class_name}() {{
    std::cout << BOLD_WHITE << "{class_name}" << RESET 
    << " default constructor called" << std::endl;
}}

// Copy constructor
{class_name}::{class_name}(const {class_name}& copy) {{
    // Copy stuff
    std::cout << BOLD_WHITE << "{class_name}" << RESET 
    << " copy constructor called" << std::endl;
}}

// Copy assignment operator overload
{class_name}& {class_name}::operator= (const {class_name}& copy) {{
    // Self-assignment check
    if (this == &copy)
        return *this;
    // Copy stuff
    std::cout << BOLD_WHITE << "{class_name}" << RESET 
    << " copy assignment operator overload" << std::endl;
    return *this;
}}

{class_name}::~{class_name}() {{
    std::cout << BOLD_WHITE << "{class_name}" << RESET 
    << " destructor called" << std::endl;
}}
"""

    last_class_name = None
    for index, class_name in enumerate(class_names):
        # Prepare file names and content
        header_filename = os.path.join(output_dir, "includes", f"{class_name}.hpp")
        cpp_filename = os.path.join(output_dir, f"{class_name}.cpp")
        guard_name = f"{class_name.upper()}_HPP"  # Header guard name

        if index == 0:
            header_content = first_template_hpp.format(class_name=class_name, guard=guard_name)
        else:
            header_content = template_hpp.format(
                class_name=class_name,
                guard=guard_name, 
                last_class_name=last_class_name if last_class_name else'')

        cpp_content = template_cpp.format(class_name=class_name, includes_dir="includes")

        # Write header file into 'includes' directory under the specified output directory
        os.makedirs(os.path.dirname(header_filename), exist_ok=True)
        with open(header_filename, "w") as header_file:
            header_file.write(header_content)

        print(f"Generated {header_filename}")

        # Write cpp file under the specified output directory
        with open(cpp_filename, "w") as cpp_file:
            cpp_file.write(cpp_content)

        print(f"Generated {cpp_filename}")
        last_class_name = class_name

    print("All files generated successfully.")

if __name__ == "__main__":
    # Read class names from command-line arguments
    class_names = sys.argv[1:-1]  # All but the last argument
    output_directory = sys.argv[-1]  # Last argument is the output directory

    if len(class_names) < 1 or not output_directory:
        print("Usage: python3 cppClassGenerator.py ClassName1 ClassName2 ... OutputDirectory")
        sys.exit(1)

    generate_class_files(class_names, output_directory)

