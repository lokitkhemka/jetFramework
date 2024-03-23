#pragma once

#include <macros.h>
#include <algorithm>
#include <functional>
#include <vector>

namespace jet
{
    //! \brief Fills from \p begins to \p end with \p value.
    //!
    //! This function fills a container specified by begin and end iterators with
    //! single thread. The order of the filling is deterministic.
    //! \param[in] begin The begin iterator of a container
    //! \param[in] end The end iterator of a container
    //! \param[in] value The value to fill a container
    //!
    //! \tparam RandomIterator - Random Iterator Type
    //! \tparam T - Value Type of a container
    template<typename RandomIterator, typename T>
    void SerialFill(const RandomIterator& begin, const RandomIterator& end, const T& value);


    //! \brief Makes a for loop from \p beginIndex to \p endIndex.
    //!
    //! This function makes a for-loop specified by begin and end indices with
    //! single thread. The order of the visit is deterministic.
    //! \param[in] beginIndex The begin Index
    //! \param[in] endIndex The end Index
    //! \param[in] function The function to call for each index
    //!
    //! \tparam IndexType - Index type.
    //! \tparam Function - Function type
    template<typename IndexType, typename Function>
    void SerialFor(const IndexType& beginIndex, const IndexType& endIndex, const Function& function);

    //! \brief Makes a 2D nested for-loop.
    //! This function makes a 2D nested for-loop specified by begin and end indices
    //! for each dimension. X will be the inner-most loop while Y is the outer-most.
    //! The order of the visit is deterministic.
    //!
    //! \param[in] beginIndexX - The begin index in X dimension.
    //! \param[in] endIndexX - The end index in X dimension.
    //! \param[in] beginIndexY - The begin index in Y dimension.
    //! \param[in] endIndexY - The end index in Y dimension.
    //! \param[in] function - The function to call for each index (i,j).
    //!
    //! \tparam IndexType - Index Type.
    //! \tparam Function - Function Type.
    template<typename IndexType, typename Function>
    void SerialFor(IndexType beginIndexX, IndexType endIndexX, IndexType beginIndexY,
                        IndexType endIndexY, const Function& function);

    //! \brief Makes a 3D nested for-loop.
    //! This function makes a 3D nested for-loop specified by begin and end indices
    //! for each dimension. X will be the inner-most loop while Z is the outer-most.
    //! The order of the visit is deterministic.
    //!
    //! \param[in] beginIndexX - The begin index in X dimension.
    //! \param[in] endIndexX - The end index in X dimension.
    //! \param[in] beginIndexY - The begin index in Y dimension.
    //! \param[in] endIndexY - The end index in Y dimension.
    //! \param[in] beginIndexZ - The end index in Z dimension.
    //! \param[in] endIndexZ - The end index in Z dimension.
    //! \param[in] function - The function to call for each index (i,j, k).
    //!
    //! \tparam IndexType - Index Type.
    //! \tparam Function - Function Type.
    template<typename IndexType, typename Function>
    void SerialFor(IndexType beginIndexX, IndexType endIndexX, IndexType beginIndexY,
                        IndexType endIndexY, IndexType beginIndexZ, IndexType endIndexZ, const Function& function);


    //! \brief Sorts a container.

    template<typename RandomIterator>
    void SerialSort(RandomIterator begin, RandomIterator end);

    //! \brief Sorts a container with a custom compare function
    template<typename RandomIterator, typename SortingFunction>
    void SerialSort(RandomIterator begin, RandomIterator end, const SortingFunction& sortingFunction);




    template<typename RandomIterator, typename T>
    void SerialFill(const RandomIterator& begin, const RandomIterator& end, const T& value)
    {
        size_t size = static_cast<size_t>(end - begin);
        SerialFor(size_t(0), size, [begin, value](size_t i){
            begin[i] = value;
        });
    }

    template<typename IndexType, typename Function>
    void SerialFor(IndexType BeginIndex, IndexType EndIndex, const Function& function)
    {
        for(IndexType i = BeginIndex; i < EndIndex; ++i)
        {
            function(i);
        }
    }

    template<typename IndexType, typename Function>
    void SerialFor(IndexType beginIndexX, IndexType endIndexX,
                    IndexType beginIndexY, IndexType endIndexY,
                    const Function& function)
    {
        for (IndexType j = beginIndexY; j < endIndexY; ++j)
        {
            for (IndexType i = beginIndexX; i < endIndexY; ++i)
            {
                function(i,j);
            }
        }
    }

    template<typename IndexType, typename Function>
    void SerialFor(IndexType beginIndexX, IndexType endIndexX,
                    IndexType beginIndexY, IndexType endIndexY,
                    IndexType beginIndexZ, IndexType endIndexZ,
                    const Function& function)
    {
        for (IndexType k = beginIndexZ; k < endIndexZ; ++k)
        {
            for (IndexType j = beginIndexY; j < endIndexY; ++j)
            {
                for (IndexType i = beginIndexX; i < endIndexY; ++i)
                {
                    function(i, j, k);
                }
            }
        }
    }

    template<typename RandomIterator, typename SortingFunction>
    void SerialSort(RandomIterator begin, RandomIterator end,
                    const SortingFunction& sortingFunction)
    {
        std::sort(begin, end, sortingFunction);
    }

    template<typename RandomIterator>
    void SerialSort(RandomIterator begin, RandomIterator end)
    {
        SerialSort(begin, end, std::less<typename RandomIterator::value_type>());
    }
}