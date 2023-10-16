#include "maxSpiHandler.hpp"
#include <stdio.h>
#include <string.h>

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
    setReg(REG_SCAN_LIMIT, numOfDigits - 1);
    setReg(REG_DECODE_MODE, 0x00);
    setReg(REG_DISPLAY_TEST, 0x00);
    setIntensity(intensity);
    clearDisplay();
    turnOn();
}

void MaxSpiHandler::clearDisplay() {
    for (int i = 1; i < 9; i++) {
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

void MaxSpiHandler::setDispHex(uint32_t value) {
    char buf[9];
    sprintf(buf, "%x", value);
    setDispStr(buf);
}

int MaxSpiHandler::setDispDec(uint32_t value) {
    if (value > 99999999)
        return 1;

    char buf[9];
    sprintf(buf, "%d", (int)value);
    return setDispStr(buf);
}

int MaxSpiHandler::setDispStr(char* buf) {
    int len = strlen(buf);
    if (len > 8)
        return 1;

    //buf[0] is the most significant digit
    //reg 1 is the least significant digit register

    //set the high digits as blank
    for (int i = 8; i > (int)strlen(buf); i--) {
        setReg(i, MAX7219_SYM_BLANK);
    }
    //set the digits that do exist
    for (int i = strlen(buf); i > 0; i--) {
        setReg(i, char2Symbol(buf[len - i]));
    }
    return 0;
}

uint8_t MaxSpiHandler::char2Symbol(char c) {
    switch (c) {
    case '0':
        return MAX7219_SYM_0;
    case '1':
        return MAX7219_SYM_1;
    case '2':
        return MAX7219_SYM_2;
    case '3':
        return MAX7219_SYM_3;
    case '4':
        return MAX7219_SYM_4;
    case '5':
        return MAX7219_SYM_5;
    case '6':
        return MAX7219_SYM_6;
    case '7':
        return MAX7219_SYM_7;
    case '8':
        return MAX7219_SYM_8;
    case '9':
        return MAX7219_SYM_9;
    case 'a':
    case 'A':
        return MAX7219_SYM_A;
    case 'B':
    case 'b':
        return MAX7219_SYM_B;
    case 'C':
    case 'c':
        return MAX7219_SYM_C;
    case 'D':
    case 'd':
        return MAX7219_SYM_D;
    case 'E':
    case 'e':
        return MAX7219_SYM_E;
    case 'F':
    case 'f':
        return MAX7219_SYM_F;
    case 'G':
    case 'g':
        return MAX7219_SYM_G;
    case 'H':
    case 'h':
        return MAX7219_SYM_H;
    case 'I':
    case 'i':
        return MAX7219_SYM_I;
    case 'J':
    case 'j':
        return MAX7219_SYM_J;
    case 'K':
    case 'k':
        return MAX7219_SYM_MINUS;
    case 'L':
    case 'l':
        return MAX7219_SYM_L;
    case 'M':
    case 'm':
        return MAX7219_SYM_MINUS;
    case 'N':
    case 'n':
        return MAX7219_SYM_N;
    case 'O':
    case 'o':
        return MAX7219_SYM_O;
    case 'P':
    case 'p':
        return MAX7219_SYM_P;
    case 'Q':
    case 'q':
        return MAX7219_SYM_Q;
    case 'R':
    case 'r':
        return MAX7219_SYM_R;
    case 'S':
    case 's':
        return MAX7219_SYM_S;
    case 'T':
    case 't':
        return MAX7219_SYM_T;
    case 'U':
    case 'u':
        return MAX7219_SYM_U;
    case 'V':
    case 'v':
        return MAX7219_SYM_MINUS;
    case 'W':
    case 'w':
        return MAX7219_SYM_MINUS;
    case 'X':
    case 'x':
        return MAX7219_SYM_MINUS;
    case 'Y':
    case 'y':
        return MAX7219_SYM_MINUS;
    case 'Z':
    case 'z':
        return MAX7219_SYM_MINUS;
    default:
        return MAX7219_SYM_MINUS;
    }

}
