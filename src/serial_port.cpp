#include "serial_port.hpp"

SerialPort::SerialPort( std::string _dev , unsigned baud )
  :port( ioservice ),
  device_path( _dev ) {
  try {
    port.open( device_path );
  } catch( ... ) {

  }

  if( !port.is_open() ) {
    throw std::runtime_error( "Could not open specified port: " + device_path );
  }

  try {
    port.set_option( boost::asio::serial_port_base::baud_rate(baud) );
    port.set_option( boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none) );
    port.set_option( boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one) );
  } catch( ... ) {
    port.close();
    throw std::runtime_error( "Could not configure port: " + device_path );
  }
}

SerialPort::~SerialPort() {
  port.close();
}

bool SerialPort::isOpen( void ) {
  return port.is_open();
}

std::size_t SerialPort::available( void ) {
  uint32_t bytes_available = 0;
  if( port.is_open() ) {
    ::ioctl( port.lowest_layer().native_handle(), FIONREAD, &bytes_available);
  }
  return static_cast<std::size_t>(bytes_available);
}

uint8_t SerialPort::write( uint8_t _byte ) {
  if( port.is_open() ) {
    return boost::asio::write( port, boost::asio::buffer(&_byte,1) );
  } else {
    return 0;
  }
}

uint8_t SerialPort::write( uint8_t *_buffer, uint32_t _len ) {
  if( port.is_open() ) {
    return boost::asio::write( port, boost::asio::buffer(_buffer,_len) );
  } else {
    return 0;
  }
}

void SerialPort::flush( void ) {
  ::tcflush( port.lowest_layer().native_handle(), TCIOFLUSH );
}

uint8_t SerialPort::read( void ) {
  if( !port.is_open() ) {
    return 0;
  }

  uint8_t _byte;
  try {
    boost::asio::read( port, boost::asio::buffer(&_byte,1) );
  } catch( boost::exception_detail::clone_impl<boost::exception_detail::error_info_injector<boost::system::system_error> > &err) {
    port.close();
    throw std::runtime_error( "Error reading serial port" );
  }
  return _byte;
}

uint8_t SerialPort::read( uint8_t *_buffer, uint32_t _len ) {
  if( !port.is_open() ) {
    return 0;
  }

  try {
    return boost::asio::read( port, boost::asio::buffer(_buffer,_len) );
  } catch( boost::exception_detail::clone_impl<boost::exception_detail::error_info_injector<boost::system::system_error> >& err) {
    port.close();
    throw std::runtime_error( "Error reading serial port" );
  }
}
