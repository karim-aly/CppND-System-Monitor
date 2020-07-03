#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <algorithm>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// Helper function to check if a process with certain pid exists in processes_ vector
int getIndexOf(vector<Process>& processes_, int pid) {
    // loop over all processes
    for (unsigned int i=0; i<processes_.size(); i++) {
        // check if required pid exists
        if (processes_[i].Pid() == pid) {
            return i;
        }
    }

    // no process with this pid found
    return -1;
} 

// Return a container composed of the system's processes
vector<Process>& System::Processes() {
    // Get Current Processes PIDs
    const vector<int> &pids = LinuxParser::Pids();

    // Loop over all PIDs available
    for (int pid : pids) {
        // check if this pid exists in processes_ vector
        int index = getIndexOf(processes_, pid);

        // create a new process object if this pid isn't in processes_ vector already
        if (index == -1) {
            Process process(pid);
            processes_.emplace_back(process);
        }
    }

    // Loop over all processes and check if any should be removed
    for (unsigned int i=0; i<processes_.size(); i++) {
        auto process = processes_[i];
        if (std::find(pids.begin(), pids.end(), process.Pid()) == pids.end()) {
            processes_.erase(processes_.begin() + i);
        }
    }

    // Sort processes according to the overloaded operator '<' in Process class
    std::sort(processes_.begin(), processes_.end(), std::greater <>());

    return processes_;
}

// Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }