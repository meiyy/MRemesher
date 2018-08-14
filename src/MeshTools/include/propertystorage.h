#pragma once
#include <vector>

namespace mesh_tools {
/*
* Base class of properties of mesh elements.
*/
class PropertyStorageBase {
public:
  virtual ~PropertyStorageBase() = default;
  virtual void Add() = 0;
  virtual void Renew(unsigned int id) = 0;
  virtual void Remove(unsigned int id) = 0;
  virtual void Resize(unsigned int new_size) = 0;
};

class PropertyManager;

/*
* Class Template of properties. It should never be constructed directly,
* and shuold always be initialized by corresponding property manager.
*/
template <class ValueType>
class PropertyStorage : public PropertyStorageBase {
  friend PropertyManager;
public:

  ValueType& operator[](const unsigned int pos) {
    return property_data_[pos];
  }

  unsigned int size() const {
    return static_cast<unsigned int>(property_data_.size());
  }

private:
  PropertyStorage() = default;

  void Add() override {
    property_data_.emplace_back();
  }

  void Renew(unsigned int id) override {
    new(&property_data_[id]) ValueType();
  }

  void Remove(unsigned int id) override {
    property_data_[id].~ValueType();
  }

  void Resize(unsigned int new_size) override {
    property_data_.resize(new_size);
  }

  std::vector<ValueType> property_data_;
};

}
