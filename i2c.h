#ifndef I2C_H
#define I2C_H

#include "sys_cfg.h"
#include <avr/interrupt.h>

ISR(TWI_vect);

class I2cMessage
{  
public:
  enum I2cMode
  {
    IM_READ = 0x00,
    IM_WRITE
  };

  I2cMessage(unsigned char address, char* buffer, unsigned short size);
  I2cMessage(unsigned char address, const enum I2cMode& mode, char* buffer, unsigned short size);
  ~I2cMessage();

  unsigned short size() const { return size_; }
  //void setSize(unsigned short value) { size_ = value; }

  char* buffer() const { return buffer_; }
  //void setBuffer(char* value) { buffer_ = value; }
  //void setBuffer(char* value, unsigned short size) { buffer_ = value; size_ = size; }

  unsigned char address() const { return address_; }
  unsigned char rawAddress() const { return address_ & 0x7f; }

private:
  char* buffer_;
  
  unsigned short size_;
  unsigned char address_;
};

class I2c
{
  friend void TWI_vect();
  
public:
  static I2c& instance();

  void start();
  void stop();

  void busy() const { return busy_; }
  void transfer(I2cMessage* msg);

private:
  enum MasterStatusCode
  {
    MSC_ERROR = 0x00,
    MSC_START_TRANSMIT = 0x08,
    MSC_START_REPEATED_TRANSMIT = 0x10,
    MSC_ADDR_WRITE_ACK = 0x18,
    MSC_ADDR_WRITE_NACK = 0x20,
    MSC_DATA_WRITE_ACK = 0x28,
    MSC_DATA_WRITE_NACK = 0x30,
    MSC_ARBITRATION_LOST = 0x38, // for both R/W
    MSC_ADDR_READ_ACK = 0x40,
    MSC_ADDR_READ_NACK = 0x48,
    MSC_DATA_READ_ACK = 0x50,
    MSC_DATA_READ_NACK = 0x58
  };
  
  static const unsigned char TWBR_REG = ((System::CPU_FREQUENCY / 2) / 400000LU) - 8; // Prescaler of 1,bitrate of 400kHz
  static const unsigned char TWCR_REG = 0x85; // Interrupt clear, Interrupt enable, I2C enable
  static const unsigned char TWSR_REG = 0x00; // Prescaler of 1

  I2c();
  ~I2c();

  void interrupt();
  void loadMessage();
  
  I2cMessage* message_;
  char* buffer_;
  
  unsigned short bufferCount_;
  unsigned char messageCount_;
  unsigned char address_;
  bool busy_;
};

#endif // I2C_H
