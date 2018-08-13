#pragma once
#include <vector>
#include <list>
#include <algorithm>
#include "propertystorage.h"
#include "surfaceelement.h"
namespace mesh_tools
{

class PropertyManager
{
public:

  PropertyManager()
  :free_list_head_(INVALID_ID), element_head_(INVALID_ID), element_tail_(INVALID_ID), element_id_count_(0), element_number_(0)
  {
  }

  unsigned int AddElement()
  {
    unsigned int new_element;
    if (free_list_head_ == INVALID_ID)
    {
      base_info_.emplace_back();
      new_element = static_cast<unsigned int>(base_info_.size() - 1);
      for (auto i : property_list_)
      {
        i->Add();
      }
    }
    else
    {
      new_element = free_list_head_^FREE_MASK;
      free_list_head_ = base_info_[new_element].id;
      for (auto i : property_list_)
      {
        i->Renew(new_element);
      }
    }
    base_info_[new_element].id = element_id_count_++;
    base_info_[new_element].next = INVALID_ID;
    base_info_[new_element].prev = element_tail_;
    if (element_tail_ == INVALID_ID)
      element_head_ = new_element;
    else
      base_info_[element_tail_].next = new_element;
    element_tail_ = new_element;
    element_number_++;
    return new_element;
  }

  void RemoveElement(const unsigned int id)
  {
    assert(!IsFree(id));
    base_info_[id].id = free_list_head_;
    free_list_head_ = id ^ FREE_MASK;

    if (id != element_head_)
      base_info_[base_info_[id].prev].next = base_info_[id].next;
    else
      element_head_ = base_info_[id].next;
    if (id != element_tail_)
      base_info_[base_info_[id].next].prev = base_info_[id].prev;
    else
      element_tail_ = base_info_[id].prev;
    for (auto i : property_list_)
    {
      i->Remove(id);
    }
    element_number_--;
  }

  unsigned int GetPrevElementPosition(unsigned int id) const 
  {
    assert(!IsFree(id));
    return base_info_[id].prev;
  }

  unsigned int GetNextElementPosition(unsigned int id) const
  {
    assert(!IsFree(id));
    return base_info_[id].next;
  }

  unsigned int size()const
  {
    return element_number_;
  }

  template<class ValueType>
  PropertyStorage<ValueType>* CreateProperty()
  {
    auto *new_property = new PropertyStorage<ValueType>();
    property_list_.emplace_back(new_property);
    new_property->Resize(element_number_);
    return new_property;
  }

  template<class ValueType>
  void RemoveProperty(PropertyStorage<ValueType>* property)
  {
    auto position = std::find(property_list_.begin(), property_list_.end(), property);
    property_list_.erase(position);
    delete property;
  }

  unsigned int UniqueID(const unsigned int id)
  {
    assert(!IsFree(id));
    return base_info_[id].id;
  }

  unsigned int ElementBegin()
  {
    return element_head_;
  }
protected:

  bool IsFree(const unsigned int id)const
  {
    assert(IsValid(id));
    return base_info_[id].id & FREE_MASK;
  }

  bool IsValid(const unsigned int id)const
  {
    return id < base_info_.size();
  }

  unsigned int free_list_head_;
  unsigned int element_head_;
  unsigned int element_tail_;
  unsigned int element_id_count_;
  unsigned int element_number_;
  std::vector<SurfaceElement> base_info_;
  std::list<PropertyStorageBase*> property_list_;
};

class FacetManager :public PropertyManager
{
public:
  FacetManager()
  {
    facets_ = CreateProperty<FacetElement>();
  }
  ~FacetManager()
  {
    RemoveProperty(facets_);
  }
  void AddFacet(const unsigned int halfedge_no)
  {
    const auto new_facet = AddElement();
    (*facets_)[new_facet].halfedge = halfedge_no;
  }
  FacetElement FacetAt(const unsigned int facet_no)const
  {
    return (*facets_)[facet_no];
  }
private:
  PropertyStorage<FacetElement>* facets_;
};

class HalfedgeManager :public PropertyManager
{
public:
  HalfedgeManager()
  {
    halfedges_ = CreateProperty<HalfedgeElement>();
  }
  ~HalfedgeManager()
  {
    RemoveProperty(halfedges_);
  }
  void AddHalfedge(const unsigned int opposite, const unsigned int next, const unsigned int prev, const unsigned int vertex, const unsigned int facet)
  {
    const auto new_halfedge = AddElement();
    (*halfedges_)[new_halfedge].opposite = opposite;
    (*halfedges_)[new_halfedge].next = next;
    (*halfedges_)[new_halfedge].prev = prev;
    (*halfedges_)[new_halfedge].vertex = vertex;
    (*halfedges_)[new_halfedge].facet = facet;
  }
  void SetOpposite(const unsigned int halfedge_no, const unsigned int opposite)
  {
    (*halfedges_)[halfedge_no].opposite = opposite;
  }
  HalfedgeElement HalfedgeAt(const unsigned int halfedge_no)const
  {
    return (*halfedges_)[halfedge_no];
  }
private:
  PropertyStorage<HalfedgeElement>* halfedges_;
};

class VertexManager :public PropertyManager
{
public:
  VertexManager()
  {
    vertices_ = CreateProperty<VertexElement>();
  }
  ~VertexManager()
  {
    RemoveProperty(vertices_);
  }
  void AddVertex(const unsigned int halfedge, const Point3& position)
  {
    const auto new_vertex = AddElement();
    (*vertices_)[new_vertex].halfedge = halfedge;
    (*vertices_)[new_vertex].position = position;
  }

  void SetHalfedge(const unsigned int vertex_no, const unsigned int halfedge_no)
  {
    (*vertices_)[vertex_no].halfedge = halfedge_no;
  }

  VertexElement VertexAt(const unsigned int vertex_no)const
  {
    return (*vertices_)[vertex_no];
  }
private:
  PropertyStorage<VertexElement>* vertices_;
};
}
