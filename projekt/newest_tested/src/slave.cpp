#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string.h>
#include <unistd.h>

const std::string PWM_CHIP_PATH = "/sys/class/pwm/pwmchip0/";
const std::string PWM_CHANNEL_PATH = PWM_CHIP_PATH + "pwm0/";
const std::string EXPORT_PATH = PWM_CHIP_PATH + "export";
const std::string UNEXPORT_PATH = PWM_CHIP_PATH + "unexport";
const std::string ENABLE_PATH = PWM_CHANNEL_PATH + "enable";
const std::string DUTY_CYCLE_PATH = PWM_CHANNEL_PATH + "duty_cycle";
const std::string PERIOD_PATH = PWM_CHANNEL_PATH + "period";

#define FIFO_PATH "/tmp/signal_to_led_fifo" // Ścieżka do named pipe

void write_to_file(const std::string &path, const std::string &value) {
  std::ofstream file(path);
  if (!file.is_open()) {
    std::cerr << "Error: Unable to open " << path << std::endl;
    exit(1);
  }
  file << value;
  file.close();
}

void set_pwm_period(int period_ns) {
  write_to_file(PERIOD_PATH, std::to_string(period_ns));
}

void set_pwm_duty_cycle(int duty_cycle_ns) {
  write_to_file(DUTY_CYCLE_PATH, std::to_string(duty_cycle_ns));
}

void set_pwm_enable(bool enable) {
  write_to_file(ENABLE_PATH, enable ? "1" : "0");
}

void pwm_init() {
  write_to_file(EXPORT_PATH, "0");
  usleep(100000); // Wait for the PWM channel to be ready

  // Set the PWM period (in nanoseconds)
  const int period_ns = 1000000; // 1 ms period
  set_pwm_period(period_ns);
}
void pwm_cleanup() {
  set_pwm_enable(false);
  write_to_file(UNEXPORT_PATH, "0");
}

void set_led_brightness(float x) {
  if (x < 0)
    x = 0;
  if (x > 3.3)
    x = 3.3;

  // Calculate the duty cycle (in nanoseconds)
  const int period_ns = 1000000; // 1 ms period
  int duty_cycle_ns = static_cast<int>((x / 3.3) * period_ns);

  set_pwm_duty_cycle(duty_cycle_ns);
}

float receive_fifo() {
  int file;

  if ((file = open(FIFO_PATH, O_RDONLY)) < 0) {
    perror("Failed to open the FIFO");
    exit(1);
  }

  uint8_t buffer[sizeof(float)];
  if (read(file, buffer, sizeof(float)) != sizeof(float)) {
    perror("Failed to read from the FIFO");
  }

  close(file);

  // Konwersja bajtów na float
  float value;
  memcpy(&value, buffer, sizeof(float));
  return value;
}

int main() {
  pwm_init();
  set_pwm_enable(true);

  while (true) {
    float x = receive_fifo();
    set_led_brightness(x);
    usleep(500000); // Opóźnienie dla stabilności
  }

  pwm_cleanup();
  return 0;
}
