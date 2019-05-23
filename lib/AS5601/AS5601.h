/**
 * AS5601 Driver
 *
 * Arduino Library
 * for AS5601 Magnet Rotary Sensor
 *
 * License: BSD 3-Clause
 *
 * Nick Schwarzenberg,
 * 06/2016, v0.1.0
 */


// include these dependencies in your top-level .ino!
#include <Arduino.h>
#include <i2c_t3.h>
#include <stdint.h>

// prevent redefinitions
#ifndef AS5601_driver
#define AS5601_driver


class AS5601
{
    public:

        /* :: Configuration :: */


        // 7-bit device address
        uint8_t address = 0x36;

        // 8-bit register addresses
        static struct ByteRegister {
            static const uint8_t
            ZMCO = 0x00,
            ABN = 0x09,
            PUSHTHR = 0x0A,
            STATUS = 0x0B,
            AGC = 0x1A,
            BURN = 0xFF;
        } ByteRegister;

        // 16-bit register start (high byte) addresses
        static struct WordRegister {
            static const uint8_t
            ZPOS = 0x01,
            CONF = 0x03,
            RAWANGLE = 0x0C,
            ANGLE = 0x0E,
            MAGNITUDE = 0x1B;
        } WordRegister;

        // initialization with explicit I2C channel
        AS5601(i2c_t3 *i2cChannel)
        {
            // host I2C bus as master in the passed channel
            wireChannel = i2cChannel;
        }

        void initWire() {
          if (wireChannel == &Wire) {
            wireChannel->begin(I2C_MASTER,0x00,I2C_PINS_7_8, I2C_PULLUP_EXT, 100000);
          }
          else if (wireChannel == &Wire2) {
            wireChannel->begin(I2C_MASTER,0x00,I2C_PINS_3_4, I2C_PULLUP_EXT, 100000);
          }
        }


        /* :: Low-Level Access :: */


        // low-level: read one byte from an 8-bit register
        uint8_t readRaw8( uint8_t registerAddress )
        {
            // send START for I2C frame to the AS5601
            wireChannel->beginTransmission( this->address );

            // send register address
            wireChannel->write( registerAddress );

            // flush, but do not release bus (no STOP)
            wireChannel->endTransmission( false );

            // request one byte as response from the AS5601, release and wait
            // (casting is necessary due to similar declarations of .requestFrom)
            wireChannel->requestFrom( (uint8_t) this->address, (uint8_t) 1, (uint8_t) true );

            // return response
            return wireChannel->read();
        }

        // low-level: read two bytes as 16-bit word from two 8-bit registers
        uint16_t readRaw16( uint8_t registerStartAddress )
        {
            // get high byte, then low byte
            uint8_t highByte = this->readRaw8( registerStartAddress );
            uint8_t lowByte = this->readRaw8( registerStartAddress + 1 );

            // combine to 16-bit word
            return word( highByte, lowByte );
        }


        // low-level: write one byte to an 8-bit register
        void writeRaw8( uint8_t registerAddress, uint8_t value )
        {
            // send START for I2C frame to the AS5601
            wireChannel->beginTransmission( this->address );

            // send register address and value
            wireChannel->write( registerAddress );
            wireChannel->write( value );

            // flush and release (STOP)
            wireChannel->endTransmission( true );
        }

        // low-level: write 16-bit word as two bytes to two 8-bit registers
        void writeRaw16( uint8_t registerStartAddress, uint16_t value )
        {
            // send high byte, then low byte
            this->writeRaw8( registerStartAddress, (uint8_t) highByte( value ) );
            this->writeRaw8( registerStartAddress + 1, (uint8_t) lowByte( value ) );
        }



        /* :: Higher-Level Methods :: */


        // query status to find out if magnet is detected
        bool magnetDetected()
        {
            // query status register
            uint8_t status = this->readRaw8( AS5601::ByteRegister::STATUS );

            // return true if bit 5 is set
            return bitRead( status, 5 ) == 1 ? true : false;
        }

        // get current magnetic magnitude (12 bit)
        uint16_t getMagnitude()
        {
            // read and return two-byte magnitude
            return this->readRaw16( AS5601::WordRegister::MAGNITUDE );
        }

        // get current gain of AGC (8 bit)
        uint8_t getGain()
        {
            // read and return one-byte gain
            return this->readRaw8( AS5601::ByteRegister::AGC );
        }

        // get raw angle (12 bit)
        uint16_t getRawAngle()
        {
            // read and return two-byte raw angle
            return this->readRaw16( AS5601::WordRegister::RAWANGLE );
        }

        // set zero-position to specified raw angle (12 bit)
        void setZeroPosition( uint16_t rawAngle )
        {
            // send position setting command
            this->writeRaw16( AS5601::WordRegister::ZPOS, rawAngle );
        }

        // convenience method: read current raw angle and pass it to .setZeroPosition(rawAngle)
        void setZeroPosition() { this->setZeroPosition( this->getRawAngle() ); }

        // set angle resolution (affecting output value range and update speed)
        void setResolution( uint16_t angleSteps )
        {
            char power = -1;

            // coerce angle steps to supported values (8, 16, 32, …, 2048)
            angleSteps = min( max( angleSteps, 8 ), 2048 );

            // find dual logarithm (2^power >= angleSteps)
            // (by comparing increasing powers of two with angleSteps)
            while ( ( 1 << ++power ) < angleSteps );

            // send ABN setting command (-3 (2^3 = 8) shifts the powers 3…11 (for 8…2048) to 0…8)
            this->writeRaw8( AS5601::ByteRegister::ABN, power-3 );
        }

        // get zero-adjusted and filtered angle (12 bit)
        uint16_t getAngle()
        {
            // read and return two-byte clean angle
            return this->readRaw16( AS5601::WordRegister::ANGLE );
        }

    private:

        i2c_t3 *wireChannel;
};


// see ifndef above
#endif
