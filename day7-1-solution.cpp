#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<algorithm>
#include<numeric>

struct File {
    std::string name;
    long long size;
};

class Directory {
    public:
        long long size;
        std::string name;
        Directory *parent;
        std::vector<Directory> subdirectories;
        std::vector<File> files;

    public:
        Directory(std::string name, Directory *parent) {
            size = 0;
            this->name = name;
            this->parent = parent;
        }

        /********************/
        /* GETTER FUNCTIONS */
        /********************/

        long long get_size() {
            auto get_file_size = [] (long long acc, File file) -> long long {
                return acc + file.size;
            };

            long long total_file_sizes = std::reduce(
                files.begin(),
                files.end(),
                0,
                get_file_size
            );

            auto get_subdirectory_size = [] (long long acc, Directory directory) -> long long {
                return acc + directory.get_size();
            };

            long long total_subdirectory_sizes = std::reduce(
                subdirectories.begin(),
                subdirectories.end(),
                0,
                get_subdirectory_size
            );

            return total_file_sizes + total_subdirectory_sizes;
        }

        Directory* get_subdirectory(std::string name) {
            for (auto &subdirectory : subdirectories) {
                if (subdirectory.name == name) return &subdirectory;
            }

            return nullptr;
        }

        std::vector<Directory> get_all_subdirectories() {
            std::vector<Directory> directories;
            directories.insert(directories.end(), subdirectories.begin(), subdirectories.end());

            for(auto directory: subdirectories) {
                auto sub_subdirectories = directory.get_all_subdirectories();
                directories.insert(
                    directories.end(),
                    sub_subdirectories.begin(),
                    sub_subdirectories.end()
                );
            }

            return directories;
        }

        /*********************/
        /* BOOLEAN FUNCTIONS */
        /*********************/

        bool subdirectory_exists(std::string name) {
            auto name_matches = [&] (Directory directory) -> bool {
                return directory.name == name;
            };

            return std::any_of(
                subdirectories.begin(),
                subdirectories.end(),
                name_matches
            );
        }

        bool subdirectory_exists_recursively(std::string name) {
            if (subdirectory_exists(name)) return true;

            for (auto directory : subdirectories) {
                if (directory.subdirectory_exists_recursively(name)) return true;
            }

            return false;
        }
        
        bool file_exists(std::string name) {
            auto name_matches = [&] (File file) -> bool {
                return file.name == name;
            };

            return std::any_of(files.begin(), files.end(), name_matches);
        }

        /***********************/
        /* INSERTION FUNCTIONS */
        /***********************/

        void add_file(std::string name, long long size) {
            if (file_exists(name)) return;

            files.push_back({ name, size });
            this->size += size;
        }

        void add_subdirectory(std::string name, Directory *parent) {
            if (subdirectory_exists(name)) return;

            subdirectories.push_back(Directory(name, parent));
        }

        void print_all_subdirectories() {
            for (auto dir: subdirectories) std::cout<<dir.name<<" ";
            std::cout<<"/"<<std::endl;
        }
};

/********/
/* MAIN */
/********/

int main() {
    std::ifstream file;
    file.open("day7-input.txt", std::ios::in);

    Directory root("/", nullptr);
    Directory *current_dir = &root;

    while(!file.eof()) {
        std::string line;
        std::getline(file, line);

        if (line.length() == 0) continue;

        // It's a command.
        if (line.at(0) == '$') {
            std::size_t first_space = line.find(' ');
            std::string command = line.substr(first_space+1, 2);

            if (command == "cd") {
                std::string directory_name = line.substr(first_space+4);

                if (directory_name == "..") current_dir = current_dir->parent;
                else if (directory_name == "/") continue;
                else current_dir = current_dir->get_subdirectory(directory_name);
            }
        }

        // It's a directory listing.
        else if (line.substr(0,3) == "dir") {
            std::size_t first_space = line.find(' ');
            std::string directory_name = line.substr(first_space+1);

            current_dir->add_subdirectory(directory_name, current_dir);
        }

        // It's a file listing.
        else {
            std::size_t first_space = line.find(' ');
            std::string file_size_str = line.substr(0, first_space);
            
            auto file_name = line.substr(first_space+1);
            long long file_size = std::stoi(file_size_str, nullptr, 10);
            current_dir->add_file(file_name, file_size);
        }
    }

    /**********************/
    /* FINAL CALCULATIONS */
    /**********************/

    auto sum = [] (long long acc, Directory directory) -> long long {
        auto size = directory.get_size();
        return (size <= 100000)? acc+size : acc;
    };

    auto all_dirs = root.get_all_subdirectories();

    long long total_filtered_size = std::reduce(
        all_dirs.begin(), all_dirs.end(), 0, sum
    );

    std::cout<<total_filtered_size<<std::endl;

    return 0;
}
