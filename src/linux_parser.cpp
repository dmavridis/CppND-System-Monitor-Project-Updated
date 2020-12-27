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
  string line, key, value;
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
  string line, os, version, kernel;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
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
  string line, key, value;
  float memtotal, memavail;
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
  string line, uptime, idletime;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime >> idletime;
  }
  return stoi(uptime);
}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) { 
  string line, value;
  long int start_time, uptime;
  int arg = 22;  // field 22 of the line
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> value){
        --arg;
        if (arg == 0){
          start_time = stoi(value)/sysconf(_SC_CLK_TCK);
          uptime = UpTime() - start_time;
          return uptime;
        }
      }
    }
  }
  return uptime;
}

// Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
    return ActiveJiffies()+IdleJiffies(); 
}

// Read and return the number of active jiffies for the system
// https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
long LinuxParser::ActiveJiffies() { 
  vector<string> cpu_ = CpuUtilization();
  // user = user - guest;   
  // nice = nice - guest_nice; 
  // systemall = system + irq + softirq;
  // virtall = guest + guest_nice;
  // active = user + nice + systemall + steal + virtall;
  return (stoi(cpu_.at(CPUStates::kUser_)) - stoi(cpu_.at(CPUStates::kGuest_))) +
          (stoi(cpu_.at(CPUStates::kNice_)) - stoi(cpu_.at(CPUStates::kGuestNice_))) +
          (stoi(cpu_.at(CPUStates::kSystem_)) + stoi(cpu_.at(CPUStates::kIRQ_)) + stoi(cpu_.at(CPUStates::kSoftIRQ_))) +
          (stoi(cpu_.at(CPUStates::kGuestNice_)) + stoi(cpu_.at(CPUStates::kGuestNice_))) +
          (stoi(cpu_.at(CPUStates::kSteal_)));
}

// Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  vector<string> cpu_ = CpuUtilization();
  return stoi(cpu_.at(CPUStates::kIdle_)) + stoi(cpu_.at(CPUStates::kIOwait_));
}

// Read and return CPU utilization
// The sting vector is processed to extract the Jiffies of the CPU
vector<string> LinuxParser::CpuUtilization() { 
  string line, key, value;
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

// Read and return the number of active jiffies for a PID
// https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599
long LinuxParser::ActiveJiffies(int pid) { 
  string line, value;
  long int active_jiffies_ = 0;
  std::ifstream filestream(kProcDirectory+to_string(pid)+kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      for (int idx = 0; idx < 18; idx++){
        linestream >> value;
        if (idx >= 14 && idx <= 17)
          active_jiffies_ += stol(value);
      }
      return active_jiffies_;
    }  
  }
  return active_jiffies_;
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string line, key, value;
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
  string line, key, value;
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
  string line, value, command;
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

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) { 
  string line, key, value;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value){
        if (key == "VmSize"){
          value = to_string(stol(value)/1024); // return in MB
          return value;
        }
      }
    }
  }
  return value;
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) { 
  string line, key, value;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
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
  string line, user, x, value;
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