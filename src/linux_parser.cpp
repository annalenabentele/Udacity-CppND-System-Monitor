#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

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
  string os;
  string version;
  string kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >>version >> os >> kernel;
  }
  return kernel;
}

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

float LinuxParser::MemoryUtilization() { 
  string line;
  string key;
  string value;
  float mem_total = 0.0f;
  float mem_free = 0.0f;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);

      while (linestream >> key >> value) {
        if (key == "MemTotal:" && value != "") {
          mem_total = std::stof(value);
        }
        if (key == "MemFree:" && value != ""){
          mem_free = std::stof(value);
        }
      }
    }
  }
  return (mem_total- mem_free) / mem_total;
 }

long LinuxParser::UpTime() { 
  long uptime;
  long idletime = 0.0;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime >> idletime;
  }
  return uptime;
}

vector<string> LinuxParser::CpuUtilization() { 
  vector<string> result;
  string line;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line); // read only first line
    std::istringstream linestream(line);
    std::stringstream iss(line);
    string value;
    while(iss >> value){
      result.push_back(value);
    }
  }
  return result;
}

vector<string> LinuxParser::CpuUtilization(int pid) { 
  
  vector<string> result;
  string line;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line); // read only first line
    std::istringstream linestream(line);
    std::stringstream iss(line);
    string value;
    while(iss >> value){
      result.push_back(value);
    }
  }
  return result;
}

int LinuxParser::TotalProcesses() { 
  int value = 0;
  string key = "processes";
  string line;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      if(line.find(key) != string::npos){
        std::istringstream linestream(line);
        linestream >> key >> value;
      }
    }
  }
  return value;
}

int LinuxParser::RunningProcesses() { 
  int value = 0;
  string key = "procs_running";
  string line;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      if(line.find(key) != string::npos){
        std::istringstream linestream(line);
        linestream >> key >> value;
      }
    }
  }
  return value;
 }

string LinuxParser::Command(int pid) { 
  string cmd = " ";
  string line;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      cmd = line;
    }
  }
  return cmd; 
 }

string LinuxParser::Ram(int pid) { 
  string ram = " ";
  string key = "VmSize:";
  string line;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      if(line.find(key) != string::npos){
        std::istringstream linestream(line);
        linestream >> key >> ram;
      }
    }
  }
  float ram_float = 0.0f;
  if(ram != ""){
    ram_float = std::stof(ram);
  }
  return to_string(int(ram_float/1024.0));
 }

string LinuxParser::Uid(int pid) { 
  string uid = " ";
  string key = "Uid:";
  string line;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      if(line.find(key) != string::npos){
        std::istringstream linestream(line);
        linestream >> key >> uid;
      }
    }
  }
  return uid;
 }

string LinuxParser::User(int pid) { 
  string user = " ";
  string key = ":x:" + LinuxParser::Uid(pid);
  string line;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      if(line.find(key) != string::npos){
        user = line.substr(0, line.find(':'));
      }
    }
  }
  return user; 
}

long LinuxParser::UpTime(int pid) { 
  long uptime = 0.0;
  string line;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      std::stringstream iss(line);
      string value;
      std::vector<string> values;
      while(iss >> value){
        values.push_back(value);
      }
      uptime = std::stoi(values[21]);
    }
  }
  return LinuxParser::UpTime() - (uptime / sysconf(_SC_CLK_TCK));
 }