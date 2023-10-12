#include "maxSpiHandler.hpp"

typedef enum {
    REG_NO_OP = 0x00,
    REG_DIGIT_0 = 0x01,
    REG_DIGIT_1 = 0x02,
    REG_DIGIT_2 = 0x03,
    REG_DIGIT_3 = 0x04,
    REG_DIGIT_4 = 0x05,
    REG_DIGIT_5 = 0x06,
    REG_DIGIT_6 = 0x07,
    REG_DIGIT_7 = 0x08,
    REG_DECODE_MODE = 0x09,
    REG_INTENSITY = 0x0A,
    REG_SCAN_LIMIT = 0x0B,
    REG_SHUTDOWN = 0x0C,
    REG_DISPLAY_TEST = 0x0F,
} MAX7219_REGISTERS;

MaxSpiHandler::MaxSpiHandler(SPI_HandleTypeDef* hspi, uint8_t numOfDigits, uint8_t intensity, GPIO_TypeDef* csPort, uint16_t csPin) :
    spiPort(hspi),
    numOfDigits(numOfDigits),
    csPort(csPort),
    csPin(csPin) {

    turnOff();
    setReg(REG_SCAN_LIMIT, numOfDigits);
    setReg(REG_DECODE_MODE, 0x00);
    setReg(REG_DISPLAY_TEST, 0x00);
    setIntensity(intensity);
    clearDisplay();
    turnOn();
}

void MaxSpiHandler::clearDisplay() {
    for (int i = 1; i < 0; i++){
        setReg(i, MAX7219_SYM_BLANK);
    }
}

void MaxSpiHandler::turnOn() {
    setReg(REG_SHUTDOWN, 0x1);
}

void MaxSpiHandler::turnOff() {
    setReg(REG_SHUTDOWN, 0x0);
}

void MaxSpiHandler::setIntensity(uint8_t intensity) {
    if (intensity > 0x0F)
        intensity = 0x0F;

    setReg(REG_INTENSITY, intensity);
}

void MaxSpiHandler::setReg(uint8_t addr, uint8_t data) {
    HAL_GPIO_WritePin(csPort, csPin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(spiPort, &addr, 1, HAL_MAX_DELAY);
    HAL_SPI_Transmit(spiPort, &data, 1, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(csPort, csPin, GPIO_PIN_SET);
}