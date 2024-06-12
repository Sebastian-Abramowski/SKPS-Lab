#include <fcntl.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PWM_CHIP_PATH "/sys/class/pwm/pwmchip0/"
#define PWM_CHANNEL0_PATH PWM_CHIP_PATH "pwm0/"
#define PWM_CHANNEL1_PATH PWM_CHIP_PATH "pwm1/"
#define EXPORT_PATH PWM_CHIP_PATH "export"
#define UNEXPORT_PATH PWM_CHIP_PATH "unexport"
#define ENABLE0_PATH PWM_CHANNEL0_PATH "enable"
#define ENABLE1_PATH PWM_CHANNEL1_PATH "enable"
#define DUTY_CYCLE0_PATH PWM_CHANNEL0_PATH "duty_cycle"
#define DUTY_CYCLE1_PATH PWM_CHANNEL1_PATH "duty_cycle"
#define PERIOD0_PATH PWM_CHANNEL0_PATH "period"
#define PERIOD1_PATH PWM_CHANNEL1_PATH "period"
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

void set_pwm_period(int period_ns, const char *period_path) {
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%d", period_ns);
    write_to_file(period_path, buffer);
}

void set_pwm_duty_cycle(int duty_cycle_ns, const char *duty_cycle_path) {
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%d", duty_cycle_ns);
    write_to_file(duty_cycle_path, buffer);
}

void set_pwm_enable(int enable, const char *enable_path) {
    write_to_file(enable_path, enable ? "1" : "0");
}

void pwm_init() {
    write_to_file(EXPORT_PATH, "0");
    write_to_file(EXPORT_PATH, "1");
    usleep(100000); // Wait for the PWM channels to be ready

    // Set the PWM period (in nanoseconds)
    const int period_ns = 1000000; // 1 ms period
    set_pwm_period(period_ns, PERIOD0_PATH);
    set_pwm_period(period_ns, PERIOD1_PATH);
}

void pwm_cleanup() {
    set_pwm_enable(0, ENABLE0_PATH);
    set_pwm_enable(0, ENABLE1_PATH);
    write_to_file(UNEXPORT_PATH, "0");
    write_to_file(UNEXPORT_PATH, "1");
}

void set_led_brightness(float x, const char *duty_cycle_path) {
    if (x < 0) x = 0;
    if (x > 3.3) x = 3.3;

    // Calculate the duty cycle (in nanoseconds)
    const int period_ns = 1000000; // 1 ms period
    int duty_cycle_ns = (int)((x / 3.3) * period_ns);

    set_pwm_duty_cycle(duty_cycle_ns, duty_cycle_path);
}

void receive_fifo(float *values) {
    int file;
    if ((file = open(FIFO_PATH, O_RDONLY)) < 0) {
        perror("Failed to open the FIFO");
        exit(1);
    }

    uint8_t buffer[4 * sizeof(float)];
    if (read(file, buffer, 4 * sizeof(float)) != 4 * sizeof(float)) {
        perror("Failed to read from the FIFO");
        close(file);
        exit(1);
    }

    close(file);

    // Convert bytes to float array
    memcpy(values, buffer, 4 * sizeof(float));
}


int main() {
    pwm_init();
    set_pwm_enable(1, ENABLE0_PATH);
    set_pwm_enable(1, ENABLE1_PATH);

    float readings[4];
    while (1) {
        receive_fifo(readings);
        set_led_brightness(readings[0], DUTY_CYCLE0_PATH);  // set LED brightness on PWM0
        set_led_brightness(readings[2], DUTY_CYCLE1_PATH);  // set LED brightness on PWM1
        printf("Received values: %f, %f, %f, %f\n", readings[0], readings[1], readings[2], readings[3]);
        usleep(250000); // Delay for stability
    }

    pwm_cleanup();
    return 0;
}
