#pragma once

#include <Arrays/array2_accessor.h>
#include <Arrays/array3_accessor.h>
#include <Arrays/array1.h>
#include <Arrays/array2.h>
#include <Arrays/array3.h>


#include <constants.h>
#include <parallel.h>
#include <serial.h>
#include <type_helpers.h>
#include <iostream>

namespace jet
{
    //! \brief Assigns \p value to 1D array \p output with \p size.
    //!
    //! This funciton assigns \p value to 1D array output with \p size. The output
    //! array must support random access operator [].
    template<typename ArrayType, typename T>
    void SetRange1(size_t size, const T& value, ArrayType* output);

    //! \brief Assigns \p value to 1D array \p output from \p begin to \p end.
    template<typename ArrayType, typename T>
    void SetRange1(size_t begin, size_t end, const T& value, ArrayType* output);

    //! \brief Copies \p input array to \p output array with \p size
    template<typename ArrayType1, typename ArrayType2>
    void CopyRange1(const ArrayType1& input, size_t size, const ArrayType2* output);

    //! \brief Copies \p input array to \p output array from \p begin to \p end.
    template<typename ArrayType1, typename ArrayType2>
    void CopyRange1(const ArrayType1& input, size_t begin, size_t end, ArrayType2* output);
    
    //! \brief Copies 2D \p input array to \p output array with \p sizeX and \p sizeY.
    template<typename ArrayType1, typename ArrayType2>
    void CopyRange2(const ArrayType1& input, size_t sizeX,
                size_t sizeY, ArrayType2* output);
    
    //! \brief Copies 2D \p input array to \p output array from ( \p beginX, \p beginY) to ( \p endX, \p endY).
    template<typename ArrayType1, typename ArrayType2>
    void CopyRange2(const ArrayType1& input, size_t beginX, size_t endX,
                    size_t beginY, size_t endY, ArrayType2* output);


    //! Copies the 3D  \p input array to \p output array with \p sizeX, \p sizeY and \p sizeZ.
    template<typename ArrayType1, typename ArrayType2>
    void CopyRange3(const ArrayType1& input, size_t sizeX, size_t sizeY, size_t sizeZ, ArrayType2* output);

    //! Copies the 3D \p input array to \p output array with ( \p beginX, \p beginY, \p beginZ) to
    //! ( \p endX, \p endY, \p endZ).
    template<typename ArrayType1, typename ArrayType2>
    void CopyRange3(const ArrayType1& input, size_t beginX, size_t endX,
                    size_t beginY, size_t endY, size_t beginZ, size_t endZ, ArrayType2* output);

    
    //! \brief Extrapolates 2D input data from 'valid' (1) to 'invalid' (0) region.
    //!
    //! This function extrapolates 2D input data from 'valid' (1) to 'invalid' (0)
    //! region. It iterates multiples times to propagate the 'valid' vlaues to nearby
    //! 'invalid' region. The maximum distance of the propagation is equal to numberOfIterations.
    //! The input parameters 'valid' and 'data' shoudl be collocated.
    template<typename T>
    void ExtrapolateToRegion(const ConstArrayAccessor2<T>& input, const ConstArrayAccessor2<char>& valid,
                            unsigned int numberOfIterations, ArrayAccessor2<T> output);

    
    //! \brief Extrapolates 3D input data from 'valid' (1) to 'invalid' (0) region.
    //!
    //! This function extrapolates 3D input data from 'valid' (1) to 'invalid' (0)
    //! region. It iterates multiples times to propagate the 'valid' vlaues to nearby
    //! 'invalid' region. The maximum distance of the propagation is equal to numberOfIterations.
    //! The input parameters 'valid' and 'data' shoudl be collocated.
    template<typename T>
    void ExtrapolateToRegion(const ConstArrayAccessor3<T>& input, const ConstArrayAccessor3<char>& valid,
                            unsigned int numberOfIterations, ArrayAccessor3<T> output);
    
    
    //! \brief Converts 2D to CSV stream.
    //!
    //! \param data - data to convert.
    //! \param strm - stream object to write CSV
    template<typename ArrayType>
    void ConvertToCSV(const ArrayType& data, std::ostream* strm);



    template<typename ArrayType, typename T>
    void SetRange1(size_t size, const T& value, ArrayType* output)
    {
        SetRange1(kZeroSize, size, value, output);
    }

    template<typename ArrayType, typename T>
    void SetRange1(size_t begin, size_t end, const T& value, ArrayType* output)
    {
        ParallelFor(begin, end, 
            [&](size_t i )
            {
                (*output)[i] = value;
            });
    }

    template<typename ArrayType1, typename ArrayType2>
    void CopyRange1(const ArrayType1& input, size_t size, ArrayType2* output)
    {
        CopyRange1(input, 0, size, output);
    }

    template<typename ArrayType1, typename ArrayType2>
    void CopyRange1(const ArrayType1& input, size_t begin, size_t end, ArrayType2* output)
    {
        ParallelFor(begin, end,
                    [&input, &output](size_t i){
                        (*output)[i] = input[i];
                    });
    }

    template<typename ArrayType1, typename ArrayType2>
    void CopyRange2(const ArrayType1& input, size_t sizeX, size_t sizeY, ArrayType2* output)
    {
        CopyRange2(input, kZeroSize, sizeX, kZeroSize, sizeY, output);
    }

    template<typename ArrayType1, typename ArrayType2>
    void CopyRange2(const ArrayType1& input, size_t beginX, size_t endX, size_t beginY, size_t endY, ArrayType2* output)
    {
        ParallelFor(beginX, endX, beginY, endY,
                    [&input, &output](size_t i, size_t j){
                        (*output)(i,j) = input(i,j);
                    });
    }

    template<typename ArrayType1, typename ArrayType2>
    void CopyRange3(const ArrayType1& input, size_t sizeX, size_t sizeY, size_t sizeZ, ArrayType2* output)
    {
        CopyRange3(input, kZeroSize, sizeX, kZeroSize, sizeY, kZeroSize, sizeZ, output);
    }

    template<typename ArrayType1, typename ArrayType2>
    void CopyRange3(const ArrayType1& input, size_t beginX, size_t endX, size_t beginY, size_t endY,
                        size_t beginZ, size_t endZ, ArrayType2* output)
    {
        ParallelFor(beginX, endX, beginY, endY, beginZ, endZ,
                    [&input, &output](size_t i, size_t j, size_t k){
                        (*output)(i,j,k) = input(i,j,k);
                    });
    }

    template <typename T>
    void ExtrapolateToRegion(
        const ConstArrayAccessor2<T>& input,
        const ConstArrayAccessor2<char>& valid,
        unsigned int numberOfIterations,
        ArrayAccessor2<T> output) {
        const Size2 size = input.Size();

        JET_ASSERT(size == valid.size());
        JET_ASSERT(size == output.size());

        Array2<char> valid0(size);
        Array2<char> valid1(size);

        valid0.ParallelForEachIndex([&](size_t i, size_t j) {
            valid0(i, j) = valid(i, j);
            output(i, j) = input(i, j);
        });

        for (unsigned int iter = 0; iter < numberOfIterations; ++iter) {
            valid0.ForEachIndex([&](size_t i, size_t j) {
                T sum = Zero<T>();
                unsigned int count = 0;

                if (!valid0(i, j)) {
                    if (i + 1 < size.x && valid0(i + 1, j)) {
                        sum += output(i + 1, j);
                        ++count;
                    }

                    if (i > 0 && valid0(i - 1, j)) {
                        sum += output(i - 1, j);
                        ++count;
                    }

                    if (j + 1 < size.y && valid0(i, j + 1)) {
                        sum += output(i, j + 1);
                        ++count;
                    }

                    if (j > 0 && valid0(i, j - 1)) {
                        sum += output(i, j - 1);
                        ++count;
                    }

                    if (count > 0) {
                        output(i, j)
                            = sum
                            / static_cast<typename ScalarType<T>::value>(count);
                        valid1(i, j) = 1;
                    }
                } else {
                    valid1(i, j) = 1;
                }
            });

            valid0.Swap(valid1);
        }
    }

    template <typename T>
    void ExtrapolateToRegion(
        const ConstArrayAccessor3<T>& input,
        const ConstArrayAccessor3<char>& valid,
        unsigned int numberOfIterations,
        ArrayAccessor3<T> output) {
        const Size3 size = input.Size();

        JET_ASSERT(size == valid.Size());
        JET_ASSERT(size == output.Size());

        Array3<char> valid0(size);
        Array3<char> valid1(size);

        valid0.ParallelForEachIndex([&](size_t i, size_t j, size_t k) {
            valid0(i, j, k) = valid(i, j, k);
            output(i, j, k) = input(i, j, k);
        });

        for (unsigned int iter = 0; iter < numberOfIterations; ++iter) {
            valid0.ForEachIndex([&](size_t i, size_t j, size_t k) {
                T sum = Zero<T>();
                unsigned int count = 0;

                if (!valid0(i, j, k)) {
                    if (i + 1 < size.x && valid0(i + 1, j, k)) {
                        sum += output(i + 1, j, k);
                        ++count;
                    }

                    if (i > 0 && valid0(i - 1, j, k)) {
                        sum += output(i - 1, j, k);
                        ++count;
                    }

                    if (j + 1 < size.y && valid0(i, j + 1, k)) {
                        sum += output(i, j + 1, k);
                        ++count;
                    }

                    if (j > 0 && valid0(i, j - 1, k)) {
                        sum += output(i, j - 1, k);
                        ++count;
                    }

                    if (k + 1 < size.z && valid0(i, j, k + 1)) {
                        sum += output(i, j, k + 1);
                        ++count;
                    }

                    if (k > 0 && valid0(i, j, k - 1)) {
                        sum += output(i, j, k - 1);
                        ++count;
                    }

                    if (count > 0) {
                        output(i, j, k)
                            = sum
                            / static_cast<typename ScalarType<T>::value>(count);
                        valid1(i, j, k) = 1;
                    }
                } else {
                    valid1(i, j, k) = 1;
                }
            });

            valid0.Swap(valid1);
        }
    }

    template <typename ArrayType>
    void ConvertToCSV(const ArrayType& data, std::ostream* strm) {
        Size2 size = data.Size();

        for (size_t j = 0; j < size.y; ++j) {
            for (size_t i = 0; i < size.x; ++i) {
                auto val = data(i, j);

                // TODO(doyubkim): Hack to handle char and unsigned char
                if (sizeof(decltype(val)) == 1) {
                    (*strm) << static_cast<int>(val);
                } else {
                    (*strm) << val;
                }

                if (i + 1 < size.x) {
                    (*strm) << ", ";
                }
            }

            (*strm) << std::endl;
        }
    }
}