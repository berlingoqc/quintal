#include "serialport.hpp"

#include <iostream>

SerialPortReader::SerialPortReader(asio::any_io_executor ex, std::string dev,
                                   DataCallback callback)
    : port(ex, dev), data_callback_(callback) {

  port.set_option(boost::asio::serial_port_base::baud_rate(default_braud));
}

SerialPortReader::SerialPortReader(asio::any_io_executor ex, std::string dev,
                                   uint32_t braud_rate, DataCallback callback)
    : port(ex, dev), data_callback_(callback) {
  port.set_option(boost::asio::serial_port_base::baud_rate(braud_rate));
}

bool SerialPortReader::read_async(uint32_t timeout_override) {
  try {
    read_buffer.fill({}); // Clear Buffer

    if (timeout_override not_eq SerialPortReader::ignore_timeout) {
      read_timeout = timeout_override;
    }
    using namespace asio::placeholders;

    port.async_read_some(
        asio::buffer(read_buffer),
        bind(&SerialPortReader::read_handler, this, error, bytes_transferred));

    return true;

  } catch (std::exception const &ex) {
    std::cerr << ex.what() << std::endl;
    return false;
  }
}

void SerialPortReader::read_handler(boost::system::error_code ec,
                                    size_t bytes_transferred) {

  if (ec.failed()) {
    std::cerr << "Error reading from serial port: " << ec.message() << std::endl;
    return;
  }

  const char *received_data =
      reinterpret_cast<const char *>(read_buffer.data());

  // Forward the received data to the callback function
  if (data_callback_) {
    data_callback_(received_data, bytes_transferred);
  }

  read_async(ignore_timeout);
}

SerialPortWriter::SerialPortWriter(boost::asio::io_context& io_context, const std::string& port_name)
  : io_context_(io_context), serial_port_(io_context, port_name) {
    serial_port_.set_option(boost::asio::serial_port_base::baud_rate(9600));
}

SerialPortWriter::SerialPortWriter(boost::asio::io_context& io_context, const std::string& port_name, uint32_t braud_rate)
  : io_context_(io_context), serial_port_(io_context, port_name) {
    serial_port_.set_option(boost::asio::serial_port_base::baud_rate(braud_rate));
}

void SerialPortWriter::writeData(const std::string& data) {
  io_context_.post([this, data]() {
    boost::asio::async_write(serial_port_,
        boost::asio::buffer(data),
        boost::bind(&SerialPortWriter::handleWrite, this,
          boost::asio::placeholders::error,
          boost::asio::placeholders::bytes_transferred));
  });
}

void SerialPortWriter::handleWrite(const boost::system::error_code& error, size_t bytes_transferred) {
  if (error) {
    std::cerr << "Error writing to serial port: " << error.message() << std::endl;
  }
}
