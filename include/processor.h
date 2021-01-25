#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  

  float Previdle();
  void Previdle(float idel);

  float Prevnonidle();
  void Prevnonidle(float idel);

  float Previowait();
  void Previowait(float iowait);

  float Prevtotal();
  void Prevtotal(float total);

  float Prevuser();
  void Prevuser(float user);

  float Prevnice();
  void Prevnice(float nice);

  float Prevsystem();
  void Prevsystem(float system);

  float Previrq();
  void Previrq(float irq);

  float Prevsoftirq();
  void Prevsoftirq(float softirq);

  float Prevsteal();
  void Prevsteal(float steal);

 private:
  float previdle_;
  float prevnonidle_;
  float previowait_;
  float prevtotal_;
  float prevuser_;
  float prevnice_;
  float prevsystem_;
  float previrq_;
  float prevsoftirq_;
  float prevsteal_;

};

#endif