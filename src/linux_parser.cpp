#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>
#include "linux_parser.h"

using std::string;
using std::vector;
using std::to_string;
using std::stof;
using std::stol;

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

string LinuxParser::Kernel() {
  string os, kernel,temp ;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> temp >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
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

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  string line;
  string key;
  string value;
  float memtotal;
  float memavail;
  float memutil = 0.0;

  std::ifstream filestream(kProcDirectory+kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "MemTotal")
          memtotal = stof(value);
        if (key == "MemAvailable"){
          memavail = stof(value);
          if (memtotal > 0){
            memutil = 1-memavail/memtotal;
            return memutil;
          } 
        }
      }
    }
  }
  return memutil;
}

// Read and return the system uptime
long LinuxParser::UpTime() { 
  string uptime, idletime;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime >> idletime;
  }
  return stoi(uptime);
}

<<<<<<< HEAD
// Not used: Read and return the number of jiffies for the system
=======
// Read and return the number of jiffies for the system
>>>>>>> play
long LinuxParser::Jiffies() { 
    return ActiveJiffies()+IdleJiffies(); 
}

// Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) { 
  string line;
  string value;
  int cpu_idx = 1;
  
  long int utime;
  long int stime;
  long int cutime;
  long int cstime;

  std::ifstream filestream(kProcDirectory+to_string(pid)+kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> value) {
        if (cpu_idx == 14)
          utime = stol(value);
        else if (cpu_idx == 15)
          stime = stol(value);
        else if (cpu_idx == 16)
          cutime = stol(value);
        else if (cpu_idx == 17)
          cstime = stol(value);
        else if (cpu_idx > 17)
          break;
        cpu_idx++;
      }
    }  
  }
  return utime + stime + cutime + cstime;
}

<<<<<<< HEAD
// Not used: Read and return the number of active jiffies for the system
=======
// Read and return the number of active jiffies for the system
// https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
>>>>>>> play
long LinuxParser::ActiveJiffies() { 
  vector<string> cpu_ = CpuUtilization();
  unsigned long long int user = stoi(cpu_.at(0));
  unsigned long long int nice = stoi(cpu_.at(1));
  unsigned long long int system = stoi(cpu_.at(2));
  unsigned long long int irq = stoi(cpu_.at(5));
  unsigned long long int softirq = stoi(cpu_.at(6));
  unsigned long long int steal = stoi(cpu_.at(7));
  unsigned long long int guest = stoi(cpu_.at(8));
  unsigned long long int guest_nice = stoi(cpu_.at(9));

  user = user - guest;   
  nice = nice - guest_nice; 

  unsigned long long int systemall = system + irq + softirq;
  unsigned long long int virtall = guest + guest_nice;
  unsigned long long int active = user + nice + systemall + steal + virtall;
  return active;
}

<<<<<<< HEAD
// Not used: Read and return the number of idle jiffies for the system
=======
// Read and return the number of idle jiffies for the system
>>>>>>> play
long LinuxParser::IdleJiffies() { 
  vector<string> cpu_ = CpuUtilization();
  unsigned long long idle = stoi(cpu_.at(3));
  unsigned long long iowait = stoi(cpu_.at(4));
  return idle+iowait; 
}

// Read and return CPU utilization
// The sting vector is processes to extract the Jiffies of the CPU
vector<string> LinuxParser::CpuUtilization() { 
  string line;
  string key;
  string value;
  vector<string> cpu;

  std::ifstream filestream(kProcDirectory+kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key) {
        if (key == "cpu"){
          while(linestream >> value)
            cpu.emplace_back(value);
        return cpu;
        }
      }
    }
  }
  return cpu;
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string line;
  string key;
  string value;
  int proctotal;
  std::ifstream filestream(kProcDirectory+kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes"){
          proctotal = stoi(value);
          return proctotal;

        }
      }
    }
  }
  return proctotal;
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string line;
  string key;
  string value;
  int procrunning;
  std::ifstream filestream(kProcDirectory+kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running"){
          procrunning = stoi(value);
          return procrunning;
        }
      }
    }
  }
  return procrunning;
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) { 
  string line;
  string value;
  string command;

  std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> value)
        command += value;
      return command;
    }
  }
  return command;
}

<<<<<<< HEAD
// Read and return CPU utilization for a process
vector<string> LinuxParser::CpuUtilization(int pid) { 
  string line;
  string value;
  vector<string> cpu;

  // getting the arguments 14, 15, 16, 17 and 22 from /proc/pid/stat
  int cpu_idx = 1;
  std::ifstream filestream(kProcDirectory+to_string(pid)+kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> value) {
        if (cpu_idx >= 14 && cpu_idx <= 17)
            cpu.emplace_back(value);
        if (cpu_idx == 22){
          cpu.emplace_back(value);
          return cpu;
        }
        cpu_idx++;
      }
    }
  }
  return cpu;
}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) { 
=======
// Read and return the memory used by a process
string LinuxParser::Ram(int pid) { 
  string key;
  string line;
  string value;

  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value){
        if (key == "vmSize")
          return value;
      }
    }
  }
  return value;
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) { 
>>>>>>> play
  string key;
  string line;
  string value;

  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value){
<<<<<<< HEAD
        if (key == "vmSize")
=======
        if (key == "Uid")
>>>>>>> play
          return value;
      }
    }
  }
  return value;
}

<<<<<<< HEAD
// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) { 
  string key;
  string line;
  string value;

  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
=======
// Read and return the user associated with a process
string LinuxParser::User(int pid) { 
  string user;
  string value;
  string line;
  string x;
  string uid = Uid(pid);

  std::ifstream filestream(kPasswordPath);
>>>>>>> play
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
<<<<<<< HEAD
      while (linestream >> key >> value){
        if (key == "Uid")
          return value;
      }
    }
  }
  return value;
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) { 
  string user;
  string value;
  string line;
  string x;
  string uid = Uid(pid);

  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> user >> x >> value){
        if (value == uid)
          return user;
      }
    }
  }
  return user;
}

// Read and return the uptime of a process
// Field 22 of the line
long LinuxParser::UpTime(int pid) {
  string line;
  string value;
  long int uptime;
  int arg = 22;
=======
      while (linestream >> user >> x >> value){
        if (value == uid)
          return user;
      }
    }
  }
  return user;
}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) { 
  string line;
  string value;
  long int start_time;
  long int uptime;
  int arg = 22;  // field 22 of the line
>>>>>>> play
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> value){
        --arg;
        if (arg == 0){
<<<<<<< HEAD
          uptime = stoi(value); //sysconf(_SC_CLK_TCK);
=======
          start_time = stoi(value)/sysconf(_SC_CLK_TCK);
          uptime = UpTime() - start_time;
>>>>>>> play
          return uptime;
        }
      }
    }
  }
  return uptime;
}