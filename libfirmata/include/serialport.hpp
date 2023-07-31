#pragma once

#include <boost/asio.hpp>
#include <boost/asio/serial_port.hpp>
#include <boost/bind/bind.hpp>
#include <iomanip>
#include <iostream>
#include <optional>

namespace asio = boost::asio;

struct SerialPortReader {
  static constexpr uint32_t ignore_timeout = -1;
  static constexpr uint32_t buffer_size = 256;
  static constexpr uint32_t default_braud = 9600;

public:
  using DataCallback = std::function<void(const char*, size_t)>;

  SerialPortReader(asio::any_io_executor ex, std::string dev, DataCallback callback);
  SerialPortReader(asio::any_io_executor ex, std::string dev, uint32_t braud_rate, DataCallback callback);

  bool read_async(uint32_t timeout_override);

private:
  void read_handler(boost::system::error_code ec, size_t bytes_transferred);

  uint32_t read_timeout = 10;
  std::array<std::byte, buffer_size> read_buffer{};
  asio::serial_port port;
  DataCallback data_callback_;
};

class SerialPortWriter {
public:
    SerialPortWriter(boost::asio::io_context& io_context, const std::string& port_name);
    SerialPortWriter(boost::asio::io_context& io_context, const std::string& port_name, uint32_t braud_rate);

    void writeData(const std::string& data);

private:
    void handleWrite(const boost::system::error_code& error, size_t bytes_transferred);

    boost::asio::io_context& io_context_;
    boost::asio::serial_port serial_port_;
};