// ADlibc++-shell-testapi.hpp

# include <filesystem>
# include <iostream>
# include <cstdlib>
# include <unistd.h>

namespace AD {
    namespace shell_command {
        void pwd() {
            std::cout << std::filesystem::current_path() << std::endl;
        }

        std::string pwd(bool asString) {
            return std::filesystem::current_path().string();
        }

        void cd(const std::string& path) {
            // std::filesystem::current_path(path);
            try {
                std::filesystem::current_path(path);
            } catch (const std::filesystem::filesystem_error& e) {
                std::cerr << "switch the directory failed: " << e.what() << std::endl;
            }
        }

        void set_env_val(const std::string& name, const std::string& val) {
            if (setenv(name.c_str(), val.c_str(), 1) != 0) {
                throw std::runtime_error("unable to set env val");
            }
        }

        constexpr auto& setval = set_env_val;

        inline void exec(const std::string& path, 
            std::initializer_list<std::string> args = {}) {
            std::vector<char*> argv;
            argv.push_back(const_cast<char*>(path.c_str()));
            for (const auto& arg : args) {
                argv.push_back(const_cast<char*>(arg.c_str()));
            }
            argv.push_back(nullptr);
            if (execvp(path.c_str(), argv.data()) == -1) {
                throw std::runtime_error("execvp failed: " + std::string(strerror(errno)));
            }
        }

        inline void exec(const std::string& path,
                             const std::string& argv0,
                             std::initializer_list<std::string> args = {}) {
            std::vector<char*> argv;
            argv.push_back(const_cast<char*>(argv0.c_str()));
            for (const auto& arg : args) {
                argv.push_back(const_cast<char*>(arg.c_str()));
            }
            argv.push_back(nullptr);
            if (execvp(path.c_str(), argv.data()) == -1) {
                throw std::runtime_error("execvp failed: " + std::string(strerror(errno)));
            }
        }

        inline pid_t make_sub_process() {
            pid_t pid = fork();
            if (pid < 0) {
                throw std::runtime_error("fork failed: " + std::string(strerror(errno)));
            }
            return pid;
        }

        inline pid_t make_sub_process(std::function<void()> task) {
            pid_t pid = fork();
            if (pid < 0) {
                throw std::runtime_error("fork failed: " + std::string(strerror(errno)));
            }
            if (pid == 0) {
                task();
                exit(0);
            }
            return pid;
        }

        inline pid_t make_sub_process(const std::string& path,
                                      std::initializer_list<std::string> args = {}) {
            pid_t pid = fork();
            if (pid < 0) {
                throw std::runtime_error("fork failed: " + std::string(strerror(errno)));
            }
            if (pid == 0) { exec(path, args); }
            return pid;
        }

        inline pid_t make_sub_process(const std::string& path,
                                      const std::string& argv0,
                                      std::initializer_list<std::string> args = {}) {
            pid_t pid = fork();
            if (pid < 0) {
                throw std::runtime_error("fork failed: " + std::string(strerror(errno)));
            }
            if (pid == 0) { exec(path, argv0, args); }
            return pid;
        }

        inline pid_t make_sub_process(std::function<void()> task,
                                      std::function<void(int)> on_complete) {
            pid_t pid = fork();
            if (pid < 0) {
                throw std::runtime_error("fork failed: " + std::string(strerror(errno)));
            }
            if (pid == 0) {
                task();
                exit(0);
            } else {
                int status;
                waitpid(pid, &status, 0);
                if (on_complete) { on_complete(WEXITSTATUS(status)); }
            }
            return pid;
        }
    } // namespace shell_command
    namespace shcmd = shell_command;
} // namespaces AD
