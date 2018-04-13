#pragma once
#include <fstream>
#include <builder.h>
#include <exception>
#include <iostream>
#include <string>

namespace mesh_tools
{
  class SurfaceFile
  {
  public:
    explicit SurfaceFile(const std::string &path){set_file(path);}
    ~SurfaceFile()
    {
       CloseFile();
    }

    virtual void ReadToSurface(Surface *surface)=0;

    virtual void SaveToFile(Surface *surface)=0;

    void CloseFile() noexcept
    {
      try
      {
        if(file_.is_open())
        {
          file_.close();
        }
      }
      catch(std::exception &err)
      {
        std::cout<<"Close file ("<< file_path_ <<") failed. Exception: "<<err.what()<<std::endl;
      }
    }
  private:
    void set_file(const std::string &path) noexcept
    {
      file_path_ = path;
      try
      {
        file_.open(path);
      }
      catch(std::exception &err)
      {
        std::cout<<"Open file ("<< file_path_ <<") failed. Exception: "<<err.what()<<std::endl;
      }
    }
    std::string file_path_;
    std::fstream file_;
  };
}
