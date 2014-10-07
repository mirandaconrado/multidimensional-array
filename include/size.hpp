#ifndef __MULTIDIMENSIONAL_ARRAY__values_HPP__
#define __MULTIDIMENSIONAL_ARRAY__values_HPP__

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

      Size() { }
      Size(Size const& other):
        size_(other.size_) { }
      Size(Size&& other):
        size_(std::move(other.size_)) { }
      Size(SizeType const& other):
        size_(other) { }
      Size(SizeType&& other):
        size_(std::move(other)) { }
      Size(SizeType::value_type const* other, size_t n_elements):
        size_(n_elements) {
          assert(other != nullptr);
          assert(n_elements > 0);
          for (size_t i = 0; i < n_elements; i++)
            size_[i] = other[i];
        }

      Size const& operator=(Size const& other) {
        size_ = other.size_;
        return *this;
      }
      Size const& operator=(Size&& other) {
        size_.swap(other.size_);
        return *this;
      }

      SizeType const& get_size() const { return size_; }
      void set_size(SizeType const& size) { size_ = size; }
      void set_size(SizeType&& size) { size_.swap(size); }

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
        if (size_.size() != n_elements)
          return false;

        for (size_t i = 0; i < n_elements; i++)
          if (size_[i] != other[i])
            return false;

        return true;
      }

      bool check_index(Size const& other) const {
        return check_index(other.size_);
      }
      bool check_index(SizeType const& other) const {
        return check_index(&other[0], other.size());
      }
      bool check_index(SizeType::value_type const* other,
          size_t n_elements) const {
        if (size_.size() != n_elements)
          return false;

        for (size_t i = 0; i < n_elements; i++)
          if (size_[i] <= other[i])
            return false;

        return true;
      }

      const_iterator cbegin() const {
        return const_iterator(SizeType(size_.size(), 0), size_);
      }

      const_iterator cend() const {
        SizeType end(size_.size(), 0);

        end[0] = size_[0];

        return const_iterator(end, size_);
      }

    public:
      SizeType size_;
  };
};

#endif
