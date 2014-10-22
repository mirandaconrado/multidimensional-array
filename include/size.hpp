#ifndef __MULTIDIMENSIONAL_ARRAY__SIZE_HPP__
#define __MULTIDIMENSIONAL_ARRAY__SIZE_HPP__

#include <boost/iterator/iterator_facade.hpp>
#include <cassert>
#include <cstdlib>
#include <vector>

namespace MultidimensionalArray {
  class Size {
    public:
      typedef std::vector<unsigned int> SizeType;

      class const_iterator: public boost::iterator_facade<const_iterator,
      SizeType const, boost::forward_traversal_tag> {
        public:
          const_iterator() { }
          const_iterator(const_iterator const& other):
            values_(other.values_),
            size_(other.size_) { }
          const_iterator(const_iterator&& other):
            values_(std::move(other.values_)),
            size_(std::move(other.size_)) { }

          const_iterator const& operator=(const_iterator const& other) {
            values_ = other.values_;
            size_ = other.size_;
            return *this;
          }

          const_iterator const& operator=(const_iterator&& other) {
            values_.swap(other.values_);
            size_.swap(other.size_);
            return *this;
          }

        private:
          friend class boost::iterator_core_access;
          friend class Size;

          const_iterator(SizeType const& values, SizeType const& size):
            values_(values),
            size_(size) { }

          void increment() {
            size_t i = values_.size();

            while (1) {
              values_[i-1]++;
              if (values_[i-1] == size_[i-1] && i != 1) {
                values_[i-1] = 0;
                i--;
                if (i == 0)
                  break;
              }
              else
                break;
            }
          }

          bool equal(const_iterator const& other) const {
            if (values_.size() != other.values_.size() ||
                size_.size() != other.size_.size())
              return false;

            for (size_t i = 0; i < values_.size(); i++)
              if (values_[i] != other.values_[i] ||
                  size_[i] != other.size_[i])
                return false;

            return true;
          }

          SizeType const& dereference() const {
            return values_;
          }

          SizeType values_, size_;
      };

      Size():
        size_({}),
        total_size_(0) { }
      Size(Size const& other):
        size_(other.size_),
        total_size_(other.total_size_) { }
      Size(Size&& other):
        size_(std::move(other.size_)),
        total_size_(std::move(other.total_size_)) { }
      Size(SizeType const& other):
        size_(other) { compute_total_size(); }
      Size(SizeType&& other):
        size_(std::move(other)) { compute_total_size(); }
      Size(SizeType::value_type const* other, size_t n_elements):
        size_(n_elements) {
          assert(other != nullptr);
          assert(n_elements > 0);
          for (size_t i = 0; i < n_elements; i++)
            size_[i] = other[i];

          compute_total_size();
        }
      Size(std::initializer_list<unsigned int> list):
        size_(list) { compute_total_size(); }

      Size const& operator=(Size const& other) {
        size_ = other.size_;
        total_size_ = other.total_size_;
        return *this;
      }
      Size const& operator=(Size&& other) {
        size_.swap(other.size_);
        total_size_ = std::move(other.total_size_);
        return *this;
      }

      operator SizeType() { return size_; }
      operator SizeType() const { return size_; }

      SizeType::size_type size() const { return size_.size(); }
      size_t total_size() const { return total_size_; }

      void set_size(SizeType const& size) { size_ = size; compute_total_size(); }
      void set_size(SizeType&& size) { size_.swap(size); compute_total_size(); }

      SizeType::value_type& operator[](size_t index) {
        assert(index < size_.size());
        return size_[index];
      }
      SizeType::value_type const& operator[](size_t index) const {
        assert(index < size_.size());
        return size_[index];
      }

      bool same(Size const& other) const {
        return same(other.size_);
      }
      bool same(SizeType const& other) const {
        return same(&other[0], other.size());
      }
      bool same(SizeType::value_type const* other, size_t n_elements) const {
        assert(other != nullptr);
        if (size_.size() != n_elements)
          return false;

        for (size_t i = 0; i < n_elements; i++)
          if (size_[i] != other[i])
            return false;

        return true;
      }

      bool check_index(Size const& index) const {
        return check_index(index.size_);
      }
      bool check_index(SizeType const& index) const {
        return check_index(&index[0], index.size());
      }
      bool check_index(SizeType::value_type const* index,
          size_t n_elements) const {
        assert(index != nullptr);
        if (size_.size() != n_elements)
          return false;

        for (size_t i = 0; i < n_elements; i++)
          if (size_[i] <= index[i])
            return false;

        return true;
      }

      template <class... Args>
      size_t get_position_variadic(Args const&... args) const {
        SizeType::value_type index[] =
        {static_cast<SizeType::value_type>(args)...};
        return get_position(index, sizeof...(args));
      }
      size_t get_position(SizeType const& index) const {
        return get_position(&index[0], index.size());
      }
      size_t get_position(SizeType::value_type const* index,
          size_t n_elements) const {
        assert(index != nullptr);
        assert(check_index(index, n_elements));

        size_t position = index[0];
        for (size_t i = 0; i < n_elements-1; i++) {
          position *= size_[i+1];
          position += index[i+1];
        }

        return position;
      }

      template <class... Args>
      size_t get_view_position_variadic(SizeType const& dimension_map,
          SizeType const& offset, SizeType const& gain,
          SizeType const& fixed_values, std::vector<bool> const& fixed_flag,
          SizeType const& original_size, Args const&... args) const {
        SizeType::value_type index[] =
        {static_cast<SizeType::value_type>(args)...};
        return get_view_position(dimension_map, offset, gain, fixed_values,
            fixed_flag, original_size, index, sizeof...(args));
      }
      size_t get_view_position(SizeType const& dimension_map,
          SizeType const& offset, SizeType const& gain,
          SizeType const& fixed_values, std::vector<bool> const& fixed_flag,
          SizeType const& original_size, SizeType const& index) const {
        return get_view_position(dimension_map, offset, gain, fixed_values,
            fixed_flag, original_size, &index[0], index.size());
      }
      size_t get_view_position(SizeType const& dimension_map,
          SizeType const& offset, SizeType const& gain,
          SizeType const& fixed_values, std::vector<bool> const& fixed_flag,
          SizeType const& original_size, SizeType::value_type const* index,
          size_t n_elements) const {
        assert(index != nullptr);
        assert(check_index(index, n_elements));

        size_t indexes_dimension = 0, indexes_i = 0;

        size_t position = 0;
        if (fixed_flag[indexes_dimension])
          position = fixed_values[indexes_dimension];
        else
          position = index[indexes_i++] * gain[dimension_map[indexes_dimension]]
            + offset[dimension_map[indexes_dimension]];
        indexes_dimension++;

        while (indexes_dimension < gain.size() &&
            indexes_i < dimension_map.size()) {
          position *= original_size[indexes_dimension];

          if (fixed_flag[indexes_dimension])
            position += fixed_values[indexes_dimension];
          else
            position +=
              index[indexes_i++] * gain[dimension_map[indexes_dimension]]
              + offset[dimension_map[indexes_dimension]];
          indexes_dimension++;
        }

        assert(indexes_dimension == gain.size());
        assert(indexes_i == dimension_map.size());

        return position;
      }

      const_iterator cbegin() const {
        return const_iterator(SizeType(size_.size(), 0), size_);
      }

      const_iterator cend() const {
        SizeType end(size_.size(), 0);

        end[0] = size_[0];

        return const_iterator(end, size_);
      }

      void swap(Size& other) {
        size_.swap(other.size_);
        size_t temp = other.total_size_;
        other.total_size_ = total_size_;
        total_size_ = temp;
      }

    private:
      void compute_total_size() {
        total_size_ = 1;
        for (auto v : size_)
          total_size_ *= v;
      }

      SizeType size_;
      size_t total_size_;
  };
};

#endif
