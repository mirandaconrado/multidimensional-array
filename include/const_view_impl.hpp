#ifndef __MULTIDIMENSIONAL_ARRAY__CONST_VIEW_IMPL_HPP__
#define __MULTIDIMENSIONAL_ARRAY__CONST_VIEW_IMPL_HPP__

#include "const_view.hpp"

#include <cassert>

namespace MultidimensionalArray {
  template <class T>
  ConstView<T>::ConstView(ConstView const& other):
    array_(other.array_),
    size_(other.size_),
    dimension_map_(other.dimension_map_),
    offset_(other.offset_),
    gain_(other.gain_),
    fixed_values_(other.fixed_values_),
    fixed_flag_(other.fixed_flag_) { }

  template <class T>
  ConstView<T>::ConstView(ConstView&& other):
    array_(other.array_),
    size_(std::move(other.size_)),
    dimension_map_(std::move(other.dimension_map_)),
    offset_(std::move(other.offset_)),
    gain_(std::move(other.gain_)),
    fixed_values_(std::move(other.fixed_values_)),
    fixed_flag_(std::move(other.fixed_flag_)) { }

  template <class T>
  template <class... Args>
  T const& ConstView<T>::operator()(Args const&... args) const {
    assert(sizeof...(args) == size_.size());
    unsigned int indexes[] = {
      static_cast<typename Array<T>::SizeType::value_type>(args)...};
    assert(check_index(indexes));

    return get_value(indexes);
  }

  template <class T>
  T const& ConstView<T>::get_value(unsigned int const indexes[]) const {
    assert(check_index(indexes));
    unsigned int indexes_i = 0, indexes_dimension = 0;

    size_t position = 0;
    if (fixed_flag_[indexes_dimension])
      position = fixed_values_[indexes_dimension];
    else
      position = indexes[indexes_i++] * gain_[dimension_map_[indexes_dimension]]
        + offset_[dimension_map_[indexes_dimension]];
    indexes_dimension++;

    while (indexes_dimension < gain_.size() && indexes_i < size_.size()) {
      position *= array_.get_size()[indexes_dimension];

      if (fixed_flag_[indexes_dimension])
        position += fixed_values_[indexes_dimension];
      else
        position +=
          indexes[indexes_i++] * gain_[dimension_map_[indexes_dimension]]
          + offset_[dimension_map_[indexes_dimension]];
      indexes_dimension++;
    }

    assert(indexes_dimension == gain_.size());
    assert(indexes_i == size_.size());

    return array_.get_pointer()[position];
  }

  template <class T>
  ConstView<T> ConstView<T>::set_range_begin(unsigned int dimension,
      unsigned int value) {
    assert(dimension < size_.size());
    assert(size_[dimension] > value);

    ConstView<T> ret(*this);
    ret.offset_[dimension_map_[dimension]] +=
      value * gain_[dimension_map_[dimension]];
    return ret;
  }

  template <class T>
  ConstView<T> ConstView<T>::set_range_end(unsigned int dimension,
      unsigned int value) {
    assert(dimension < size_.size());
    assert(size_[dimension] >= value);
    assert(value > 0);

    ConstView<T> ret(*this);
    ret.size_[dimension] = value;
    return ret;
  }

  template <class T>
  ConstView<T> ConstView<T>::set_range_stride(unsigned int dimension,
      unsigned int value) {
    assert(dimension < size_.size());
    assert(value > 0);

    ConstView<T> ret(*this);
    ret.size_[dimension] = (ret.size_[dimension] + value - 1)/value;
    ret.gain_[dimension_map_[dimension]] *= value;
    return ret;
  }

  template <class T>
  ConstView<T> ConstView<T>::fix_dimension(unsigned int dimension,
      unsigned int value) {
    assert(dimension < size_.size());
    assert(size_[dimension] > value);

    ConstView<T> ret(*this);
    ret.size_.erase(ret.size_.begin()+dimension);
    ret.dimension_map_.erase(ret.dimension_map_.begin()+dimension);

    ret.fixed_values_[dimension_map_[dimension]] =
      offset_[dimension_map_[dimension]] +
      value * gain_[dimension_map_[dimension]];
    ret.fixed_flag_[dimension_map_[dimension]] = true;

    return ret;
  }

  template <class T>
  ConstView<T>::ConstView(ConstArray<T>& array):
    array_(array),
    size_(array.get_size()),
    dimension_map_(size_.size()),
    offset_(size_.size(), 0),
    gain_(size_.size(), 1),
    fixed_values_(size_.size(), 0),
    fixed_flag_(size_.size(), false) {
      for (unsigned int i = 0; i < size_.size(); i++)
        dimension_map_[i] = i;
    }

  template <class T>
  bool ConstView<T>::same_size(
      typename ConstArray<T>::SizeType const& other_size) const {
    if (size_.size() != other_size.size())
      return false;

    for (unsigned int i = 0; i < size_.size(); i++)
      if (size_[i] != other_size[i])
        return false;

    return true;
  }

  template <class T>
  bool ConstView<T>::check_index(unsigned int const indexes[]) const {
    for (unsigned int i = 0; i < size_.size(); i++)
      if (indexes[i] >= size_[i])
        return false;

    return true;
  }
};

#endif
