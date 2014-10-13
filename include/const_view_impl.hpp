#ifndef __MULTIDIMENSIONAL_ARRAY__CONST_VIEW_IMPL_HPP__
#define __MULTIDIMENSIONAL_ARRAY__CONST_VIEW_IMPL_HPP__

#include "const_view.hpp"

namespace MultidimensionalArray {
  template <class T>
  ConstView<T>::ConstView(ConstView const& other):
    array_(other.array_),
    carray_(other.carray_),
    size_(other.size_),
    original_view_(other.original_view_),
    dimension_map_(other.dimension_map_),
    offset_(other.offset_),
    gain_(other.gain_),
    fixed_values_(other.fixed_values_),
    fixed_flag_(other.fixed_flag_) { }

  template <class T>
  ConstView<T>::ConstView(ConstView&& other):
    array_(other.array_),
    carray_(other.carray_),
    size_(std::move(other.size_)),
    original_view_(std::move(other.original_view_)),
    dimension_map_(std::move(other.dimension_map_)),
    offset_(std::move(other.offset_)),
    gain_(std::move(other.gain_)),
    fixed_values_(std::move(other.fixed_values_)),
    fixed_flag_(std::move(other.fixed_flag_)) { }

  template <class T>
  ConstView<T>::ConstView(View<T> const& other):
    array_(&other.array_),
    carray_(nullptr),
    size_(other.size_),
    original_view_(other.original_view_),
    dimension_map_(other.dimension_map_),
    offset_(other.offset_),
    gain_(other.gain_),
    fixed_values_(other.fixed_values_),
    fixed_flag_(other.fixed_flag_) { }

  template <class T>
  ConstView<T>::ConstView(View<T>&& other):
    array_(&other.array_),
    carray_(nullptr),
    size_(std::move(other.size_)),
    original_view_(std::move(other.original_view_)),
    dimension_map_(std::move(other.dimension_map_)),
    offset_(std::move(other.offset_)),
    gain_(std::move(other.gain_)),
    fixed_values_(std::move(other.fixed_values_)),
    fixed_flag_(std::move(other.fixed_flag_)) { }

  template <class T>
  template <class... Args>
  T const& ConstView<T>::operator()(Args const&... args) const {
    assert(get_pointer() != nullptr);
    if (original_view_)
      return get_pointer()[size_.get_position_variadic(args...)];
    else
      return
        get_pointer()[size_.get_view_position_variadic(dimension_map_,
            offset_, gain_, fixed_values_, fixed_flag_,
            (array_!=nullptr?array_->size():carray_->size()), args...)];
  }

  template <class T>
  T const& ConstView<T>::get(Size::SizeType const& index) const {
    assert(index.size() == size().size());
    assert(get_pointer() != nullptr);
    if (original_view_)
      return get_pointer()[size_.get_position(index)];
    else
      return get_pointer()[size_.get_view_position(dimension_map_, offset_,
          gain_, fixed_values_, fixed_flag_,
          (array_!=nullptr?array_->size():carray_->size()), index)];
  }

  template <class T>
  ConstView<T> ConstView<T>::set_range_begin(size_t dimension,
      size_t value) const {
    assert(dimension < size().size());
    assert(size_[dimension] > value);

    ConstView<T> ret(*this);
    ret.original_view_ = false;
    ret.size_[dimension] -= value;
    ret.offset_[dimension_map_[dimension]] +=
      value * gain_[dimension_map_[dimension]];
    return ret;
  }

  template <class T>
  ConstView<T> ConstView<T>::set_range_end(size_t dimension,
      size_t value) const {
    assert(dimension < size().size());
    assert(size_[dimension] >= value);
    assert(value > 0);

    ConstView<T> ret(*this);
    ret.original_view_ = false;
    ret.size_[dimension] = value;
    return ret;
  }

  template <class T>
  ConstView<T> ConstView<T>::set_range_stride(size_t dimension,
      size_t value) const {
    assert(dimension < size().size());
    assert(value > 0);

    ConstView<T> ret(*this);
    ret.original_view_ = false;
    ret.size_[dimension] = (ret.size_[dimension] + value - 1)/value;
    ret.gain_[dimension_map_[dimension]] *= value;
    return ret;
  }

  template <class T>
  ConstView<T> ConstView<T>::fix_dimension(size_t dimension,
      size_t value) const {
    assert(dimension < size().size());
    assert(size_[dimension] > value);

    ConstView<T> ret(*this);
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
  ConstView<T>::ConstView(Array<T> const& array):
    array_(&array),
    carray_(nullptr),
    size_(array.get_size()),
    original_view_(true),
    dimension_map_(size().size()),
    offset_(size().size(), 0),
    gain_(size().size(), 1),
    fixed_values_(size().size(), 0),
    fixed_flag_(size().size(), false) {
      for (unsigned int i = 0; i < size().size(); i++)
        dimension_map_[i] = i;
    }

  template <class T>
  ConstView<T>::ConstView(ConstArray<T> const& array):
    array_(nullptr),
    carray_(&array),
    size_(array.get_size()),
    original_view_(true),
    dimension_map_(size().size()),
    offset_(size().size(), 0),
    gain_(size().size(), 1),
    fixed_values_(size().size(), 0),
    fixed_flag_(size().size(), false) {
      for (unsigned int i = 0; i < size().size(); i++)
        dimension_map_[i] = i;
    }

  template <class T>
  T const* ConstView<T>::get_pointer() const {
    if (array_ != nullptr)
      return array_->get_pointer();
    else
      return carray_->get_pointer();
  }
};

#endif
