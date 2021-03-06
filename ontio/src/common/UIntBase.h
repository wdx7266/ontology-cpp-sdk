#ifndef UINTBASE_H
#define UINTBASE_H

#include <algorithm>
#include <stdexcept>
#include <string>
#include <vector>

#include "../io/Serializable.h"
#include "Helper.h"

class UIntBase : public Serializable
{
private:
  std::vector<unsigned char> data_bytes;

protected:
  void set_data_bytes(const std::vector<unsigned char> &_data_bytes)
  {
    data_bytes = _data_bytes;
  }

  void set_data_bytes(int bytes, const std::vector<unsigned char> &value)
  {
    if (value.empty())
    {
      data_bytes.assign(bytes, '0');
    }
    else if (int(value.size()) != bytes)
    {
      throw std::runtime_error("IllegalArgumentException");
    }
    else
    {
      data_bytes = value;
    }
  }

  std::vector<unsigned char> get_data_bytes() { return data_bytes; }

public:
  UIntBase() {}
  UIntBase(int bytes, const std::vector<unsigned char> &value)
  {
    if (value.empty())
    {
      data_bytes.assign(bytes, '0');
    }
    else if (int(value.size()) != bytes)
    {
      throw std::runtime_error("IllegalArgumentException");
    }
    else
    {
      data_bytes = value;
    }
  }

  UIntBase &operator=(const UIntBase &base)
  {
    this->data_bytes = base.data_bytes;
    return *this;
  }

  std::vector<unsigned char> get_data_bytes() const { return data_bytes; }

  void serialize(BinaryWriter *writer) { writer->write(data_bytes); }

  void deserialize(BinaryReader *reader) { reader->read(data_bytes); }

  std::vector<unsigned char> toArray() { return data_bytes; }

  std::string toHexString()
  {
    Helper helper;
    std::string str;
    str = helper.toHexString(toArray());
    std::reverse(str.begin(), str.end());
    return str;
  }

  bool equals(const UIntBase &uintbase)
  {
    return data_bytes == uintbase.data_bytes;
  }
};

#endif