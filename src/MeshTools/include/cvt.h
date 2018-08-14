#pragma once
#include "sample.h"

namespace mesh_tools {

class CVT {
public:
  CVT(Surface& surface, Sample& sample)
    : surface_(surface),
      sample_(sample) {
  }

  void init(unsigned int sample_size) {
    sample_.FixedNumberRandomSample(sample_size);
  }

  void computeCVT() {

  }

  auto& sample() {
    return sample_;
  }

private:
  Surface& surface_;
  Sample& sample_;
};


}
