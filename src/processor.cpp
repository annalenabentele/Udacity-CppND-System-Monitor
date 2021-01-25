#include "processor.h"
#include "linux_parser.h"
#include <vector>

using std::string;
using std::vector;

float Processor::Previdle() { return previdle_; }
float Processor::Prevnonidle() { return prevnonidle_; }
float Processor::Previowait() { return previowait_; }
float Processor::Prevtotal() { return prevtotal_; }
float Processor::Prevuser() { return prevuser_; }
float Processor::Prevnice() { return prevnice_; }
float Processor::Prevsystem() { return prevsystem_; }
float Processor::Previrq() { return previrq_; }
float Processor::Prevsoftirq() { return prevsoftirq_; }
float Processor::Prevsteal() { return prevsteal_; }

void Processor::Previdle(float idle) { previdle_ = idle; }
void Processor::Prevnonidle(float nonidle) { prevnonidle_ = nonidle; }
void Processor::Previowait(float iowait) { previowait_ = iowait; }
void Processor::Prevtotal(float total){ prevtotal_ = total; }
void Processor::Prevuser(float user){ prevuser_ = user; }
void Processor::Prevnice(float nice){ prevnice_ = nice; }
void Processor::Prevsystem(float system){ prevsystem_ = system; }
void Processor::Previrq(float irq){ previrq_ = irq; }
void Processor::Prevsoftirq(float softirq){ prevsoftirq_ = softirq; }
void Processor::Prevsteal(float steal){ prevsteal_ = steal; }

float Processor::Utilization() { 

    vector<string> utilization = LinuxParser::CpuUtilization();
    float user = std::stof(utilization[1]);
    float nice = std::stof(utilization[2]);
    float system = std::stof(utilization[3]);
    float idle = std::stof(utilization[4]);
    float iowait = std::stof(utilization[5]);
    float irq = std::stof(utilization[6]);
    float softirq = std::stof(utilization[7]);
    float steal = std::stof(utilization[8]);

    float previdle_new = Previdle() + Previowait();
    float idle_new = idle + iowait;

    float prevnonidle_new = Prevuser() + Prevnice() + Prevsystem() + Previrq() + Prevsoftirq() + Prevsteal();
    float nonidle_new = user + nice + system + irq + softirq + steal;

    float prevtotal_new = previdle_new + prevnonidle_new;
    float total_new = idle_new + nonidle_new;

    //differentiate: actual value minus the previous one
    float totald = total_new - prevtotal_new;
    float idled = idle_new - previdle_new;

    return (totald - idled)/totald;; 
}