#pragma once
#include <fstream>
#include <iomanip>
#include <exception>
#include <iostream>
#include <string>
#include <utility>
#include "builder.h"
#include "property.h"

namespace mesh_tools {
class SurfaceFile {
public:
  explicit SurfaceFile(std::string path)
    : file_path_(std::move(path)) {
  }

  virtual void ReadToSurface(Surface& surface) =0;

  virtual void SaveToFile(Surface& surface) =0;

  void CloseFile() noexcept {
    try {
      if (file_.is_open()) {
        file_.close();
      }
    } catch (std::exception& err) {
      std::cout << "Close file (" << file_path_ << ") failed. Exception: " <<
        err.what() << std::endl;
    }
  }

  void OpenForRead() {
    try {
      if (file_.is_open())
        file_.close();
      file_.open(file_path_, std::ios::in);
    } catch (std::exception& err) {
      std::cout << "Open file (" << file_path_ << ") failed. Exception: " << err
        .what() << std::endl;
    }
  }

  void OpenForWrite() {
    try {
      if (file_.is_open())
        file_.close();
      file_.open(file_path_, std::ios::out);
    } catch (std::exception& err) {
      std::cout << "Open file (" << file_path_ << ") failed. Exception: " << err
        .what() << std::endl;
    }
  }

protected:
  ~SurfaceFile() = default;
  std::string file_path_;
  std::fstream file_;
};

class ObjFile : public SurfaceFile {
public:
  explicit ObjFile(const std::string& path)
    : SurfaceFile(path) {
  }

  void ReadToSurface(Surface& surface) override {
    OpenForRead();
    Builder builder(surface);
    std::string word;
    while (file_ >> word) {
      if (word == "v" || word == "V") {
        double x[3];
        for (auto& i : x) {
          file_ >> i;
        }
        builder.AddVertex({x[0], x[1], x[2]});
      } else if (word == "f" || word == "F") {
        unsigned int x[3];
        for (auto& i : x) {
          file_ >> i;
          char next_char = ' ';
          while (next_char == ' ')
            next_char = static_cast<char>(file_.get() & 0xff);
          if (next_char == '/') {
            int ignore;
            file_ >> ignore;
          } else
            file_.putback(next_char);
        }
        builder.AddFacet(x[2] - 1, x[1] - 1, x[0] - 1);
      } else {
        std::getline(file_, word);
      }
    }
    builder.Build();
    CloseFile();
  }

  void SaveToFile(Surface& surface) override {
    OpenForWrite();
    Property<unsigned int> vertex_id(surface.vertex_manager());
    unsigned int counter = 0;
    file_.setf(std::ios_base::fixed, std::ios_base::floatfield);
    file_.precision(10);
    for (VertexIterator i(surface, 0); !i.IsEnd(); ++i) {
      vertex_id[i.vertex_no()] = ++counter;
      file_ << "v " << i.position()[0] << " " << i.position()[1] << " " << i.
        position()[2] << std::endl;
    }
    for (FacetIterator i(surface, 0); !i.IsEnd(); ++i) {
      file_ << "f";
      auto j = i.halfedge();
      do {
        file_ << " " << vertex_id[j.vertex().vertex_no()];
        j = j.next();
      }
      while (j != i.halfedge());
      file_ << std::endl;
    }
    CloseFile();
  }

  virtual ~ObjFile() = default;
private:
};
}
