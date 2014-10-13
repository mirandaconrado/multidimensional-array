#ifndef __MULTIDIMENSIONAL_ARRAY__VIEW_IMPL_HPP__
#define __MULTIDIMENSIONAL_ARRAY__VIEW_IMPL_HPP__

#include "view.hpp"

namespace MultidimensionalArray {
  template <class T>
  View<T>::View(View const& other):
    array_(other.array_),
    size_(other.size_),
    original_view_(other.original_view_),
    dimension_map_(other.dimension_map_),
    offset_(other.offset_),
    gain_(other.gain_),
    fixed_values_(other.fixed_values_),
    fixed_flag_(other.fixed_flag_) { }

  template <class T>
  View<T>::View(View&& other):
    array_(other.array_),
    size_(std::move(other.size_)),
    original_view_(std::move(other.original_view_)),
    dimension_map_(std::move(other.dimension_map_)),
    offset_(std::move(other.offset_)),
    gain_(std::move(other.gain_)),
    fixed_values_(std::move(other.fixed_values_)),
    fixed_flag_(std::move(other.fixed_flag_)) { }

  template <class T>
  View<T> const& View<T>::operator=(Array<T> const& other) {
    assert(same_size(other.get_size()));
    copy(other.get_pointer());
    return *this;
  }

  template <class T>
  template <class T2>
  View<T> const& View<T>::operator=(Array<T2> const& other) {
    assert(size_.same(other.get_size()));
    copy(other.get_pointer());
    return *this;
  }

  template <class T>
  View<T> const& View<T>::operator=(ConstArray<T> const& other) {
    assert(size_.same(other.get_size()));
    copy(other.get_pointer());
    return *this;
  }

  template <class T>
  template <class T2>
  View<T> const& View<T>::operator=(ConstArray<T2> const& other) {
    assert(size_.same(other.get_size()));
    copy(other.get_pointer());
    return *this;
  }

  template <class T>
  View<T> const& View<T>::operator=(View const& other) {
    assert(size_.same(other.get_size()));
    copy(other);
    return *this;
  }

  template <class T>
  template <class T2>
  View<T> const& View<T>::operator=(View<T2> const& other) {
    assert(size_.same(other.get_size()));
    copy(other);
    return *this;
  }

  template <class T>
  View<T> const& View<T>::operator=(ConstView<T> const& other) {
    assert(size_.same(other.get_size()));
    copy(other);
    return *this;
  }

  template <class T>
  template <class T2>
  View<T> const& View<T>::operator=(ConstView<T2> const& other) {
    assert(size_.same(other.get_size()));
    copy(other);
    return *this;
  }

  template <class T>
  template <class... Args>
  T& View<T>::operator()(Args const&... args) {
    assert(array_.get_pointer() != nullptr);
    if (original_view_)
      return array_.get_pointer()[size_.get_position_variadic(args...)];
    else
      return array_.get_pointer()[get_position_variadic(args...)];
  }

  template <class T>
  template <class... Args>
  T const& View<T>::operator()(Args const&... args) const {
    assert(array_.get_pointer() != nullptr);
    if (original_view_)
      return array_.get_pointer()[size_.get_position_variadic(args...)];
    else
      return array_.get_pointer()[get_position_variadic(args...)];
  }

  template <class T>
  T& View<T>::get(Size::SizeType const& index) {
    assert(index.size() == size().size());
    assert(array_.get_pointer() != nullptr);
    if (original_view_)
      return array_.get_pointer()[size_.get_position(index)];
    else
      return array_.get_pointer()[get_position(&index[0])];
  }

  template <class T>
  T const& View<T>::get(Size::SizeType const& index) const {
    assert(index.size() == size().size());
    assert(array_.get_pointer() != nullptr);
    if (original_view_)
      return array_.get_pointer()[size_.get_position(index)];
    else
      return array_.get_pointer()[get_position(&index[0])];
  }

  template <class T>
  View<T> View<T>::set_range_begin(size_t dimension, size_t value) {
    assert(dimension < size().size());
    assert(size_[dimension] > value);

    View<T> ret(*this);
    ret.original_view_ = false;
    ret.size_[dimension] -= value;
    ret.offset_[dimension_map_[dimension]] +=
      value * gain_[dimension_map_[dimension]];
    return ret;
  }

  template <class T>
  View<T> View<T>::set_range_end(size_t dimension, size_t value) {
    assert(dimension < size().size());
    assert(size_[dimension] >= value);
    assert(value > 0);

    View<T> ret(*this);
    ret.original_view_ = false;
    ret.size_[dimension] = value;
    return ret;
  }

  template <class T>
  View<T> View<T>::set_range_stride(size_t dimension, size_t value) {
    assert(dimension < size().size());
    assert(value > 0);

    View<T> ret(*this);
    ret.original_view_ = false;
    ret.size_[dimension] = (ret.size_[dimension] + value - 1)/value;
    ret.gain_[dimension_map_[dimension]] *= value;
    return ret;
  }

  template <class T>
  View<T> View<T>::fix_dimension(size_t dimension, size_t value) {
    assert(dimension < size().size());
    assert(size_[dimension] > value);

    View<T> ret(*this);
    ret.original_view_ = false;
    Size::SizeType temp(ret.size_.get_size());
    temp.erase(temp.begin()+dimension);
    ret.size_.set_size(std::move(temp));
    ret.dimension_map_.erase(ret.dimension_map_.begin()+dimension);

    ret.fixed_values_[dimension_map_[dimension]] =
      offset_[dimension_map_[dimension]] +
      value * gain_[dimension_map_[dimension]];
    ret.fixed_flag_[dimension_map_[dimension]] = true;

    return ret;
  }

  template <class T>
  View<T>::View(Array<T>& array):
    array_(array),
    size_(array.get_size()),
    original_view_(true),
    dimension_map_(size().size()),
    offset_(size().size(), 0),
    gain_(size().size(), 1),
    fixed_values_(size().size(), 0),
    fixed_flag_(size().size(), false) {
      for (size_t i = 0; i < size().size(); i++)
        dimension_map_[i] = i;
    }

  template <class T>
  template <class... Args>
  size_t View<T>::get_position_variadic(Args const&... args) const {
    assert(sizeof...(args) == size().size());
    Size::SizeType::value_type indexes[] =
    {static_cast<Size::SizeType::value_type>(args)...};

    return get_position(indexes);
  }

  template <class T>
  size_t View<T>::get_position(
      Size::SizeType::value_type const* indexes) const {
    assert(indexes != nullptr);
    assert(size_.check_index(indexes, size().size()));

    size_t indexes_dimension = 0, indexes_i = 0;

    size_t position = 0;
    if (fixed_flag_[indexes_dimension])
      position = fixed_values_[indexes_dimension];
    else
      position = indexes[indexes_i++] * gain_[dimension_map_[indexes_dimension]]
        + offset_[dimension_map_[indexes_dimension]];
    indexes_dimension++;

    while (indexes_dimension < gain_.size() &&
        indexes_i < dimension_map_.size()) {
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
    assert(indexes_i == dimension_map_.size());

    return position;
  }

  template <class T>
  template <class T2>
  void View<T>::copy(T2 const* other) {
    assert(other != nullptr);
    auto it1 = size_.cbegin();
    auto it2 = size_.cend();
    for (size_t i = 0; i < get_total_size() && it1 != it2; i++, ++it1)
      get(*it1) = other[i];
  }

  template <class T>
  template <class T2>
  void View<T>::copy(View<T2> const& other) {
    auto it1 = size_.cbegin();
    auto it2 = size_.cend();
    for (; it1 != it2; ++it1) {
      auto v = *it1;
      get(v) = other.get(v);
    }
  }

  template <class T>
  template <class T2>
  void View<T>::copy(ConstView<T2> const& other) {
    auto it1 = size_.cbegin();
    auto it2 = size_.cend();
    for (; it1 != it2; ++it1) {
      auto v = *it1;
      get(v) = other.get(v);
    }
  }
};

#endif
