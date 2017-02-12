/*
  SerialPort - Arduino API like wrapper for using serial communication
*/

#ifndef SERIAL_PORT_HPP
#define SERIAL_HPP

#include <iostream>
#include <string>
#include <boost/asio.hpp>

class SerialPort {
private:
  boost::asio::io_service ioservice;
  boost::asio::serial_port port;
  std::string device_path;

public:
  SerialPort( std::string dev, unsigned baud );
  ~SerialPort();

  // check whether the port is open
  bool isOpen( void );

  // overloading bool()
  explicit operator bool() const;

  // get available bytes in buffer
  std::size_t available( void );

  // write one byte
  uint8_t write( uint8_t _byte );

  // write byte stream
  uint8_t write( uint8_t *_bytes, uint32_t _len );

  // flush buffer
  void flush( void );

  // read one byte
  uint8_t read( void );

  // read stream of bytes
  uint8_t read( uint8_t *_buffer, uint32_t _len );

};

#endif
