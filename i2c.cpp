#include "i2c.h"
#include <stddef.h>
#include <avr/interrupt.h>
#include <avr/io.h>

#define i2cStart()    (TWCR |= (1 << 5))
#define i2cWrite(dat) (TWDR = dat)

/* ISR */
static volatile I2c& instance = I2c::instance(); // Hook for the interrupt handler

ISR(TWI_vect)
{
  instance.interrupt();
  TWCR |= (1 << 7); // Clear interrupt
}

/* I2cMessage */
I2cMessage::I2cMessage(unsigned char address, char* buffer, unsigned short size) :
  I2cMessage(address, IM_READ, buffer, size)
{
  
}

I2cMessage::I2cMessage(unsigned char address, const enum I2cMode& mode, char* buffer, unsigned short size)
{
  address_ = address;
  if(mode == IM_WRITE)
    address &= ~(1 << 7);
  else
    address_ |= (1 << 7);

  buffer_ = buffer;
  size_ = size;
}

I2cMessage::~I2cMessage()
{
  
}

/* I2c */
I2c& I2c::instance()
{
  static I2c instance;
  return instance;  
}

void I2c::start()
{
  TWBR = TWBR_REG;
  TWSR = TWSR_REG;
  TWCR = TWCR_REG;
}

void I2c::stop()
{

}

void I2c::transfer(I2cMessage* msg)
{
  if(busy_ || msg == NULL)
    return;

  message_ = msg;
  messageCount_ = 1;
  loadMessage();
  
  busy_ = true;
  i2cStart();
}

I2c::I2c()
{
  message_ = NULL;
  buffer_ = NULL;
  bufferCount_ = 0;
  messageCount_ = 0;
  address_ = 0;
  busy_ = false;
}

I2c::~I2c()
{

}

void __attribute__((always_inline)) I2c::interrupt()
{
    /*
   *     MSC_ERROR = 0x00,
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
   */

  // Upper 5 bits represent the status
  switch(TWSR & 0xf8) {
    case MSC_START_TRANSMIT:
    case MSC_START_REPEATED_TRANSMIT:
      i2cWrite(address_);
      break;
    default:
    case MSC_ERROR:
      break;
  }
}

void __attribute__((always_inline)) I2c::loadMessage()
{
  buffer_ = message_->buffer();
  bufferCount_ = message_->size();
  address_ = message_->address();
}

