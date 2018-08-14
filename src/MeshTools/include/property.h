#pragma once
#include "propertymanager.h"

namespace mesh_tools {
template <class ValueType>
class Property {
public:
  Property(PropertyManager& manager)
    : manager_(&manager) {
    storage_ = manager_->CreateProperty<ValueType>();
  }

  ~Property() {
    manager_->RemoveProperty(storage_);
  }

  ValueType& operator[](const unsigned int pos) {
    return (*storage_)[pos];
  }

private:
  PropertyStorage<ValueType>* storage_;
  PropertyManager* manager_;
};
}
