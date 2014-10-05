#ifndef __MULTIDIMENSIONAL_ARRAY__VIEW_HPP__
#define __MULTIDIMENSIONAL_ARRAY__VIEW_HPP__

#include "array.hpp"

namespace MultidimensionalArray {
  template <class T>
  class View {
    public:
      View(View const& other);
      View(View&& other);

      View const& operator=(Array<T> const& other);
      template <class T2>
      View const& operator=(Array<T2> const& other);
      View const& operator=(ConstArray<T> const& other);
      template <class T2>
      View const& operator=(ConstArray<T2> const& other);
      View const& operator=(View const& other);
      template <class T2>
      View const& operator=(View<T2> const& other);
      View const& operator=(ConstView<T> const& other);
      template <class T2>
      View const& operator=(ConstView<T2> const& other);

      template <class... Args>
      T& operator()(Args const&... args);
      template <class... Args>
      T const& operator()(Args const&... args) const;

      T& get_value(unsigned int const indexes[]);
      T const& get_value(unsigned int const indexes[]) const;

      View set_range_begin(unsigned int dimension, unsigned int value);
      View set_range_end(unsigned int dimension, unsigned int value);
      View set_range_stride(unsigned int dimension, unsigned int value);
      View fix_dimension(unsigned int dimension, unsigned int value);

      typename Array<T>::SizeType get_size() const { return size_; }

    private:
      friend class Array<T>;

      View(Array<T>& array);

      template <class T2>
      void copy(T2 const& other);

      bool same_size(typename Array<T>::SizeType const& other_size) const;
      bool check_index(unsigned int const indexes[]) const;

      Array<T>& array_;
      typename Array<T>::SizeType size_, dimension_map_;
      typename Array<T>::SizeType offset_, gain_, fixed_values_;
      std::vector<bool> fixed_flag_;
  };
};

#include "view_impl.hpp"

#endif
