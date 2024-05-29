#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PWM_CHIP_PATH "/sys/class/pwm/pwmchip0/"
#define PWM_CHANNEL_PATH PWM_CHIP_PATH "pwm0/"
#define EXPORT_PATH PWM_CHIP_PATH "export"
#define UNEXPORT_PATH PWM_CHIP_PATH "unexport"
#define ENABLE_PATH PWM_CHANNEL_PATH "enable"
#define DUTY_CYCLE_PATH PWM_CHANNEL_PATH "duty_cycle"
#define PERIOD_PATH PWM_CHANNEL_PATH "period"
#define FIFO_PATH "/tmp/signal_to_led_fifo" // Path to the named pipe

void write_to_file(const char *path, const char *value) {
    FILE *file = fopen(path, "w");
    if (!file) {
        perror("Error: Unable to open file");
        exit(1);
    }
    fprintf(file, "%s", value);
    fclose(file);
}

void set_pwm_period(int period_ns) {
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%d", period_ns);
    write_to_file(PERIOD_PATH, buffer);
}

void set_pwm_duty_cycle(int duty_cycle_ns) {
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%d", duty_cycle_ns);
    write_to_file(DUTY_CYCLE_PATH, buffer);
}

void set_pwm_enable(int enable) {
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
    set_pwm_enable(0);
    write_to_file(UNEXPORT_PATH, "0");
}

void set_led_brightness(float x) {
    if (x < 0) x = 0;
    if (x > 3.3) x = 3.3;

    // Calculate the duty cycle (in nanoseconds)
    const int period_ns = 1000000; // 1 ms period
    int duty_cycle_ns = (int)((x / 3.3) * period_ns);

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
        close(file);
        exit(1);
    }

    close(file);

    // Convert bytes to float
    float value;
    memcpy(&value, buffer, sizeof(float));
    return value;
}

int main() {
    pwm_init();
    set_pwm_enable(1);

    while (1) {
        float x = receive_fifo();
        set_led_brightness(x);
        usleep(500000); // Delay for stability
    }

    pwm_cleanup();
    return 0;
}

