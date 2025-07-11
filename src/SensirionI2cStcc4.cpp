/*
 * THIS FILE IS AUTOMATICALLY GENERATED
 *
 * Generator:     sensirion-driver-generator 1.2.0
 * Product:       stcc4
 * Model-Version: 3.4.0
 */
/*
 * Copyright (c) 2025, Sensirion AG
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of Sensirion AG nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "SensirionI2cStcc4.h"
#include <Arduino.h>

// make sure that we use the proper definition of NO_ERROR
#ifdef NO_ERROR
#undef NO_ERROR
#endif
#define NO_ERROR 0

static uint8_t communication_buffer[18] = {0};

SensirionI2cStcc4::SensirionI2cStcc4() {
}

float SensirionI2cStcc4::signalTemperature(uint16_t rawTemperature) {
    float temperature = 0.0;
    temperature = -45.0 + ((175.0 * rawTemperature) / 65535.0);
    return temperature;
}

float SensirionI2cStcc4::signalRelativeHumidity(uint16_t rawRelativeHumidity) {
    float relativeHumidity = 0.0;
    relativeHumidity = -6.0 + ((125.0 * rawRelativeHumidity) / 65535.0);
    return relativeHumidity;
}

int16_t SensirionI2cStcc4::readMeasurement(int16_t& co2Concentration,
                                           float& temperature,
                                           float& relativeHumidity,
                                           uint16_t& sensorStatus) {
    int16_t rawCo2Concentration = 0;
    uint16_t rawTemperature = 0;
    uint16_t rawRelativeHumidity = 0;
    uint16_t sensorStatusRaw = 0;
    int16_t localError = 0;
    localError = readMeasurementRaw(rawCo2Concentration, rawTemperature,
                                    rawRelativeHumidity, sensorStatusRaw);
    if (localError != NO_ERROR) {
        return localError;
    }
    co2Concentration = rawCo2Concentration;
    temperature = SensirionI2cStcc4::signalTemperature(rawTemperature);
    relativeHumidity =
        SensirionI2cStcc4::signalRelativeHumidity(rawRelativeHumidity);
    sensorStatus = sensorStatusRaw;
    return localError;
}

int16_t SensirionI2cStcc4::setPressureCompensation(uint16_t pressure) {
    int16_t localError = 0;
    localError = setPressureCompensationRaw((uint16_t)(pressure / 2));
    if (localError != NO_ERROR) {
        return localError;
    }
    return localError;
}

int16_t SensirionI2cStcc4::startContinuousMeasurement() {
    int16_t localError = NO_ERROR;
    uint8_t* buffer_ptr = communication_buffer;
    SensirionI2CTxFrame txFrame =
        SensirionI2CTxFrame::createWithUInt16Command(0x218b, buffer_ptr, 2);
    localError =
        SensirionI2CCommunication::sendFrame(_i2cAddress, txFrame, *_i2cBus);
    if (localError != NO_ERROR) {
        return localError;
    }
    return localError;
}

int16_t SensirionI2cStcc4::readMeasurementRaw(int16_t& co2ConcentrationRaw,
                                              uint16_t& temperatureRaw,
                                              uint16_t& relativeHumidityRaw,
                                              uint16_t& sensorStatusRaw) {
    int16_t localError = NO_ERROR;
    uint8_t* buffer_ptr = communication_buffer;
    SensirionI2CTxFrame txFrame =
        SensirionI2CTxFrame::createWithUInt16Command(0xec05, buffer_ptr, 12);
    localError =
        SensirionI2CCommunication::sendFrame(_i2cAddress, txFrame, *_i2cBus);
    if (localError != NO_ERROR) {
        return localError;
    }
    delay(1);
    SensirionI2CRxFrame rxFrame(buffer_ptr, 12);
    localError = SensirionI2CCommunication::receiveFrame(_i2cAddress, 12,
                                                         rxFrame, *_i2cBus);
    if (localError != NO_ERROR) {
        return localError;
    }
    localError |= rxFrame.getInt16(co2ConcentrationRaw);
    localError |= rxFrame.getUInt16(temperatureRaw);
    localError |= rxFrame.getUInt16(relativeHumidityRaw);
    localError |= rxFrame.getUInt16(sensorStatusRaw);
    return localError;
}

int16_t SensirionI2cStcc4::stopContinuousMeasurement() {
    int16_t localError = NO_ERROR;
    uint8_t* buffer_ptr = communication_buffer;
    SensirionI2CTxFrame txFrame =
        SensirionI2CTxFrame::createWithUInt16Command(0x3f86, buffer_ptr, 2);
    localError =
        SensirionI2CCommunication::sendFrame(_i2cAddress, txFrame, *_i2cBus);
    if (localError != NO_ERROR) {
        return localError;
    }
    delay(1200);
    return localError;
}

int16_t SensirionI2cStcc4::measureSingleShot() {
    int16_t localError = NO_ERROR;
    uint8_t* buffer_ptr = communication_buffer;
    SensirionI2CTxFrame txFrame =
        SensirionI2CTxFrame::createWithUInt16Command(0x219d, buffer_ptr, 2);
    localError =
        SensirionI2CCommunication::sendFrame(_i2cAddress, txFrame, *_i2cBus);
    if (localError != NO_ERROR) {
        return localError;
    }
    delay(500);
    return localError;
}

int16_t
SensirionI2cStcc4::performForcedRecalibration(int16_t targetCO2Concentration,
                                              int16_t& frcCorrection) {
    int16_t localError = NO_ERROR;
    uint8_t* buffer_ptr = communication_buffer;
    SensirionI2CTxFrame txFrame =
        SensirionI2CTxFrame::createWithUInt16Command(0x362f, buffer_ptr, 5);
    localError |= txFrame.addInt16(targetCO2Concentration);
    if (localError != NO_ERROR) {
        return localError;
    }
    localError =
        SensirionI2CCommunication::sendFrame(_i2cAddress, txFrame, *_i2cBus);
    if (localError != NO_ERROR) {
        return localError;
    }
    delay(90);
    SensirionI2CRxFrame rxFrame(buffer_ptr, 5);
    localError = SensirionI2CCommunication::receiveFrame(_i2cAddress, 3,
                                                         rxFrame, *_i2cBus);
    if (localError != NO_ERROR) {
        return localError;
    }
    localError |= rxFrame.getInt16(frcCorrection);
    return localError;
}

int16_t SensirionI2cStcc4::getProductId(uint32_t& productId,
                                        uint64_t& serialNumber) {
    int16_t localError = NO_ERROR;
    uint8_t* buffer_ptr = communication_buffer;
    SensirionI2CTxFrame txFrame =
        SensirionI2CTxFrame::createWithUInt16Command(0x365b, buffer_ptr, 18);
    localError =
        SensirionI2CCommunication::sendFrame(_i2cAddress, txFrame, *_i2cBus);
    if (localError != NO_ERROR) {
        return localError;
    }
    delay(1);
    SensirionI2CRxFrame rxFrame(buffer_ptr, 18);
    localError = SensirionI2CCommunication::receiveFrame(_i2cAddress, 18,
                                                         rxFrame, *_i2cBus);
    if (localError != NO_ERROR) {
        return localError;
    }
    localError |= rxFrame.getUInt32(productId);
    localError |= rxFrame.getInteger((uint8_t*)&serialNumber, LongInteger, 8);
    return localError;
}

int16_t SensirionI2cStcc4::setRhtCompensation(uint16_t rawTemperature,
                                              uint16_t rawHumidity) {
    int16_t localError = NO_ERROR;
    uint8_t* buffer_ptr = communication_buffer;
    SensirionI2CTxFrame txFrame =
        SensirionI2CTxFrame::createWithUInt16Command(0xe000, buffer_ptr, 8);
    localError |= txFrame.addUInt16(rawTemperature);
    if (localError != NO_ERROR) {
        return localError;
    }
    localError |= txFrame.addUInt16(rawHumidity);
    if (localError != NO_ERROR) {
        return localError;
    }
    localError =
        SensirionI2CCommunication::sendFrame(_i2cAddress, txFrame, *_i2cBus);
    if (localError != NO_ERROR) {
        return localError;
    }
    delay(1);
    return localError;
}

int16_t SensirionI2cStcc4::setPressureCompensationRaw(uint16_t pressure) {
    int16_t localError = NO_ERROR;
    uint8_t* buffer_ptr = communication_buffer;
    SensirionI2CTxFrame txFrame =
        SensirionI2CTxFrame::createWithUInt16Command(0xe016, buffer_ptr, 5);
    localError |= txFrame.addUInt16(pressure);
    if (localError != NO_ERROR) {
        return localError;
    }
    localError =
        SensirionI2CCommunication::sendFrame(_i2cAddress, txFrame, *_i2cBus);
    if (localError != NO_ERROR) {
        return localError;
    }
    delay(1);
    return localError;
}

int16_t SensirionI2cStcc4::performSelfTest(uint16_t& testResult) {
    int16_t localError = NO_ERROR;
    uint8_t* buffer_ptr = communication_buffer;
    SensirionI2CTxFrame txFrame =
        SensirionI2CTxFrame::createWithUInt16Command(0x278c, buffer_ptr, 3);
    localError =
        SensirionI2CCommunication::sendFrame(_i2cAddress, txFrame, *_i2cBus);
    if (localError != NO_ERROR) {
        return localError;
    }
    delay(360);
    SensirionI2CRxFrame rxFrame(buffer_ptr, 3);
    localError = SensirionI2CCommunication::receiveFrame(_i2cAddress, 3,
                                                         rxFrame, *_i2cBus);
    if (localError != NO_ERROR) {
        return localError;
    }
    localError |= rxFrame.getUInt16(testResult);
    return localError;
}

int16_t SensirionI2cStcc4::performConditioning() {
    int16_t localError = NO_ERROR;
    uint8_t* buffer_ptr = communication_buffer;
    SensirionI2CTxFrame txFrame =
        SensirionI2CTxFrame::createWithUInt16Command(0x29bc, buffer_ptr, 2);
    localError =
        SensirionI2CCommunication::sendFrame(_i2cAddress, txFrame, *_i2cBus);
    if (localError != NO_ERROR) {
        return localError;
    }
    delay(22000);
    return localError;
}

int16_t SensirionI2cStcc4::enterSleepMode() {
    int16_t localError = NO_ERROR;
    uint8_t* buffer_ptr = communication_buffer;
    SensirionI2CTxFrame txFrame =
        SensirionI2CTxFrame::createWithUInt16Command(0x3650, buffer_ptr, 2);
    localError =
        SensirionI2CCommunication::sendFrame(_i2cAddress, txFrame, *_i2cBus);
    if (localError != NO_ERROR) {
        return localError;
    }
    delay(2);
    return localError;
}

int16_t SensirionI2cStcc4::exitSleepMode() {
    int16_t localError = NO_ERROR;
    uint8_t* buffer_ptr = communication_buffer;
    SensirionI2CTxFrame txFrame =
        SensirionI2CTxFrame::createWithUInt8Command(0x0, buffer_ptr, 2);
    SensirionI2CCommunication::sendFrame(_i2cAddress, txFrame, *_i2cBus);
    delay(5);
    return localError;
}

int16_t SensirionI2cStcc4::enableTestingMode() {
    int16_t localError = NO_ERROR;
    uint8_t* buffer_ptr = communication_buffer;
    SensirionI2CTxFrame txFrame =
        SensirionI2CTxFrame::createWithUInt16Command(0x3fbc, buffer_ptr, 2);
    localError =
        SensirionI2CCommunication::sendFrame(_i2cAddress, txFrame, *_i2cBus);
    if (localError != NO_ERROR) {
        return localError;
    }
    return localError;
}

int16_t SensirionI2cStcc4::disableTestingMode() {
    int16_t localError = NO_ERROR;
    uint8_t* buffer_ptr = communication_buffer;
    SensirionI2CTxFrame txFrame =
        SensirionI2CTxFrame::createWithUInt16Command(0x3f3d, buffer_ptr, 2);
    localError =
        SensirionI2CCommunication::sendFrame(_i2cAddress, txFrame, *_i2cBus);
    if (localError != NO_ERROR) {
        return localError;
    }
    return localError;
}

int16_t SensirionI2cStcc4::performFactoryReset(uint16_t& factoryResetResult) {
    int16_t localError = NO_ERROR;
    uint8_t* buffer_ptr = communication_buffer;
    SensirionI2CTxFrame txFrame =
        SensirionI2CTxFrame::createWithUInt16Command(0x3632, buffer_ptr, 3);
    localError =
        SensirionI2CCommunication::sendFrame(_i2cAddress, txFrame, *_i2cBus);
    if (localError != NO_ERROR) {
        return localError;
    }
    delay(90);
    SensirionI2CRxFrame rxFrame(buffer_ptr, 3);
    localError = SensirionI2CCommunication::receiveFrame(_i2cAddress, 3,
                                                         rxFrame, *_i2cBus);
    if (localError != NO_ERROR) {
        return localError;
    }
    localError |= rxFrame.getUInt16(factoryResetResult);
    return localError;
}

void SensirionI2cStcc4::begin(TwoWire& i2cBus, uint8_t i2cAddress) {
    _i2cBus = &i2cBus;
    _i2cAddress = i2cAddress;
}
