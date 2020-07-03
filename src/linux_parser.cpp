#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <unordered_map>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;
using std::unordered_map;

// An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// TODO Later: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// Helper function to read '/proc/meminfo' file and return the info in a map
unordered_map<string, string> ReadMemoryInfoFile() {
  unordered_map<string, string> memInfo;
  string line;
  string key;
  string value;

  // open target file for reading line by line
  std::ifstream filestream(LinuxParser::kProcDirectory + LinuxParser::kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      // split line at spaces to get first 2 parts
      std::istringstream linestream(line);
      if (linestream >> key >> value) {
        // remove last character of the key ':'
        key.erase(key.size()-1);
        // add the key/value pair in the memory info map
        memInfo.insert({ key, value });
      }
    }
  }
  return memInfo;
}

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  float totalMemory = 0.0f;
  float freeMemory = 0.0f;

  // Read The System file '/proc/meminfo'
  const unordered_map<string, string> &memInfo = ReadMemoryInfoFile();
  
  // Check if the Attribute 'MemTotal' exists in the map
  auto memTotalElemPtr = memInfo.find("MemTotal");
  if (memTotalElemPtr != memInfo.end()) {
    totalMemory = stof(memTotalElemPtr->second);
  }

  // Check if the Attribute 'MemFree' exists in the map
  auto memFreeElemPtr = memInfo.find("MemFree");
  if (memFreeElemPtr != memInfo.end()) {
    freeMemory = stof(memFreeElemPtr->second);
  }

  // Return The Utilization Percentage
  return (totalMemory - freeMemory) / totalMemory;
}

// Read and return the system uptime
long LinuxParser::UpTime() {
  string line;
  string uptime;
  string idletime;

  // open target file for reading line by line
  std::ifstream filestream(LinuxParser::kProcDirectory + LinuxParser::kUptimeFilename);
  if (filestream.is_open()) {
    if (std::getline(filestream, line)) {
      // split line at spaces to get first 2 parts
      std::istringstream linestream(line);
      if (linestream >> uptime >> idletime) {
        // convert uptime to long and return
        return static_cast<long>(stof(uptime));
      }
    }
  }
  return 0;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  string line;
  string buffer;
  vector<string> cpu_readings;

  // open target file for reading line by line
  std::ifstream filestream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      // split line at spaces to get first 2 parts
      std::istringstream linestream(line);
      if (linestream >> buffer) {
        // check if cpu is the first word in the line
        if (buffer == "cpu") {
          // read the 10 cpu readings (aggregate processor information)
          for (int i=0; i<10; i++) {
            if (linestream >> buffer) {
              cpu_readings.push_back(buffer);
            }
          }
          break;
        }
      }
    }
  }
  return cpu_readings;
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line;
  string key;
  string value;

  // open target file for reading line by line
  std::ifstream filestream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      // split line at spaces to get first 2 parts
      std::istringstream linestream(line);
      if (linestream >> key >> value) {
        // check if key is equal to 'processes'
        if (key == "processes") {
          return std::stoi(value);
        }
      }
    }
  }
  return 0;
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line;
  string key;
  string value;

  // open target file for reading line by line
  std::ifstream filestream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      // split line at spaces to get first 2 parts
      std::istringstream linestream(line);
      if (linestream >> key >> value) {
        // check if key is equal to 'procs_running'
        if (key == "procs_running") {
          return std::stoi(value);
        }
      }
    }
  }
  return 0;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid[[maybe_unused]]) { return 0; }