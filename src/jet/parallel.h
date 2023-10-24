#pragma once

#include <constants.h>
#include <macros.h>

#include<algorithm>
#include<functional>
#include<thread>
#include<vector>

namespace jet
{
    //!
    //! \brief Fills from \p begin to \p end with \p value in parallel.
    //!
    //! This function fills a container specified by begin and end
    //! iterator in parallel. The order of the filling ins not
    //! guaranteed due to the nature of parallel execution.
    //!
    //! \param[in] begin The begin iterator of a container.
    //! \param[in] end The end iterator of a container.
    //! \param[in] value The value to fill a container with
    //!
    //! \tparam RandomIterator RandomIteratorType
    //! \tparam T Value type of the container
    template<typename RandomIterator, typename T>
    void ParallelFill(
        const RandomIterator& begin,
        const RandomIterator& end,
        const T& value);
    

    //! \brief Makes a for-loop from \p beginIndex to \p endIndex in parallel.
    //!
    //! This function makes a for-loop specified by begin and end indices
    //! in parallel. The order of the visit is not guaranteed due to the
    //! nature of parallel of execution.
    //!
    //! \param[in] beginIndex The begin index.
    //! \param[in] endIndex The end index.
    //! \param[in] function The function to call for each index.
    //!
    //! \tparam IndexType Index Type
    //! \tparam Function function type
    template<typename IndexType, typename Function>
    void ParallelFor(IndexType beginIndex, IndexType endIndex, const Function& function);

    //! \brief      Makes a 2D nested for-loop in parallel.
    //!
    //! This function makes a 2D nested for-loop specified by begin and end indices
    //! for each dimension. X will be the inner-most loop while Y is the outer-most.
    //! The order of the visit is not guaranteed due to the nature of parallel
    //! execution.
    //!
    //! \param[in]  beginIndexX The begin index in X dimension.
    //! \param[in]  endIndexX   The end index in X dimension.
    //! \param[in]  beginIndexY The begin index in Y dimension.
    //! \param[in]  endIndexY   The end index in Y dimension.
    //! \param[in]  function    The function to call for each index (i, j).
    //!
    //! \tparam     IndexType  Index type.
    //! \tparam     Function   Function type.
    //!
    template <typename IndexType, typename Function>
    void ParallelFor(
        IndexType beginIndexX,
        IndexType endIndexX,
        IndexType beginIndexY,
        IndexType endIndexY,
        const Function& function);

    //!
    //! \brief      Makes a 3D nested for-loop in parallel.
    //!
    //! This function makes a 3D nested for-loop specified by begin and end indices
    //! for each dimension. X will be the inner-most loop while Z is the outer-most.
    //! The order of the visit is not guaranteed due to the nature of parallel
    //! execution.
    //!
    //! \param[in]  beginIndexX The begin index in X dimension.
    //! \param[in]  endIndexX   The end index in X dimension.
    //! \param[in]  beginIndexY The begin index in Y dimension.
    //! \param[in]  endIndexY   The end index in Y dimension.
    //! \param[in]  beginIndexZ The begin index in Z dimension.
    //! \param[in]  endIndexZ   The end index in Z dimension.
    //! \param[in]  function    The function to call for each index (i, j, k).
    //!
    //! \tparam     IndexType   Index type.
    //! \tparam     Function    Function type.
    //!
    template <typename IndexType, typename Function>
    void ParallelFor(
        IndexType beginIndexX,
        IndexType endIndexX,
        IndexType beginIndexY,
        IndexType endIndexY,
        IndexType beginIndexZ,
        IndexType endIndexZ,
        const Function& function);

    //! \brief      Sorts a container in parallel.
    //!
    //! This function sorts a container specified by begin and end iterators.
    //!
    //! \param[in]  begin          The begin random access iterator.
    //! \param[in]  end            The end random access iterator.
    //!
    //! \tparam     RandomIterator Iterator type.
    //!
    template<typename RandomIterator>
    void ParallelSort(RandomIterator begin, RandomIterator end);

    //! \brief      Sorts a container in parallel.
    //!
    //! This function sorts a container specified by begin and end iterators.
    //!
    //! \param[in]  begin          The begin random access iterator.
    //! \param[in]  end            The end random access iterator.
    //!
    //! \tparam     RandomIterator Iterator type.
    //!
    // template<typename RandomIterator>
    // void ParallelSort(RandomIterator begin, RandomIterator end);

    //!
    //! \brief      Sorts a container in parallel with a custom compare function.
    //!
    //! This function sorts a container specified by begin and end iterators. It
    //! takes extra compare function which returns true if the first argument is
    //! less than the second argument.
    //!
    //! \param[in]  begin           The begin random access iterator.
    //! \param[in]  end             The end random access iterator.
    //! \param[in]  compare         The compare function.
    //!
    //! \tparam     RandomIterator  Iterator type.
    //! \tparam     CompareFunction Compare function type.
    //!
    template<typename RandomIterator, typename CompareFunction>
    void ParallelSort(
        RandomIterator begin,
        RandomIterator end,
        CompareFunction compare);

    namespace internal {

        // Adopted from:
        // Radenski, A.
        // Shared Memory, Message Passing, and Hybrid Merge Sorts for Standalone and
        // Clustered SMPs. Proc PDPTA'11, the  2011 International Conference on Parallel
        // and Distributed Processing Techniques and Applications, CSREA Press
        // (H. Arabnia, Ed.), 2011, pp. 367 - 373.
        template <
            typename RandomIterator,
            typename RandomIterator2,
            typename CompareFunction>
        void Merge(
            RandomIterator a,
            size_t size,
            RandomIterator2 temp,
            CompareFunction compareFunction) {
            size_t i1 = 0;
            size_t i2 = size / 2;
            size_t tempi = 0;

            while (i1 < size / 2 && i2 < size) {
                if (compareFunction(a[i1], a[i2])) {
                    temp[tempi] = a[i1];
                    i1++;
                } else {
                    temp[tempi] = a[i2];
                    i2++;
                }
                tempi++;
            }

            while (i1 < size / 2) {
                temp[tempi] = a[i1];
                i1++;
                tempi++;
            }

            while (i2 < size) {
                temp[tempi] = a[i2];
                i2++;
                tempi++;
            }

            // Copy sorted temp array into main array, a
            ParallelFor(kZeroSize, size, [&](size_t i) {
                a[i] = temp[i];
            });
        }

        template <
            typename RandomIterator,
            typename RandomIterator2,
            typename CompareFunction>
        void ParallelMergeSort(
            RandomIterator a,
            size_t size,
            RandomIterator2 temp,
            unsigned int numThreads,
            CompareFunction compareFunction) {
            if (numThreads == 1) {
                std::sort(a, a + size, compareFunction);
            } else if (numThreads > 1) {
                std::vector<std::thread> pool;
                pool.reserve(2);

                auto launchRange = [compareFunction] (
                    RandomIterator begin,
                    size_t k2,
                    RandomIterator2 temp,
                    unsigned int numThreads) {
                    ParallelMergeSort(begin, k2, temp, numThreads, compareFunction);
                };

                pool.emplace_back(launchRange, a, size / 2, temp, numThreads / 2);
                pool.emplace_back(
                    launchRange,
                    a + size / 2,
                    size - size / 2,
                    temp + size / 2,
                    numThreads - numThreads / 2);

                // Wait for jobs to finish
                for (std::thread &t : pool) {
                    if (t.joinable()) {
                        t.join();
                    }
                }

                Merge(a, size, temp, compareFunction);
            }
        }

    }  // namespace internal
    

    template<typename RandomIterator, typename T>
    void ParallelFill(const RandomIterator& begin, const RandomIterator& end, const T& value)
    {
        auto diff = end-begin;
        if (diff <= 0)
            return ;
    
        size_t size = static_cast<size_t>(diff);
        ParallelFor(kZeroSize, size, [begin, value](size_t i){
            begin[i] = value;
        });
    }

    template<typename IndexType, typename Function>
    void ParallelFor
        (IndexType start, IndexType end,
            const Function& func)
        {
            if (start > end)
                return;
            
            //Estimate the number of threads in the pool
            static const unsigned int NumThreadsHint = std::thread::hardware_concurrency();
            static const unsigned int NumThreads = (NumThreadsHint == 0u ? 8u : NumThreadsHint);

            //Size of a slice for the range functions
            IndexType n = end - start + 1;
            IndexType slice = (IndexType) std::round(n / static_cast<double>(NumThreads));
            slice = std::max(slice, IndexType(1));

            //Inner Loop
            auto launchRange = [&func] (IndexType k1, IndexType k2){
                for (IndexType k = k1; k< k2; k++)
                    func(k);
            };

            //Create pool and launch jobs
            std::vector<std::thread> pool;
            pool.reserve(NumThreads);
            IndexType i1 = start;
            IndexType i2 = std::min(start+slice, end);
            for (unsigned int i = 0; i < NumThreads - 1 && i1 < end; ++i)
            {
                pool.emplace_back(launchRange, i1, i2);
                i1 = i2;
                i2 = std::min(i2 + slice, end);
            }

            if(i1<end)
                pool.emplace_back(launchRange, i1, end);
            
            //Wait for jobs to finish
            for (std::thread &t : pool)
            {
                if (t.joinable())
                    t.join();
            }
        }
    
    template <typename IndexType, typename Function>
    void ParallelFor(
        IndexType beginIndexX,
        IndexType endIndexX,
        IndexType beginIndexY,
        IndexType endIndexY,
        const Function& function)
    {
        ParallelFor( beginIndexY, endIndexY, [&](size_t j){
            for (IndexType i = beginIndexX; i<endIndexX; ++i)
                function(i,j);
        });
    }


    template <typename IndexType, typename Function>
    void ParallelFor(
        IndexType beginIndexX,
        IndexType endIndexX,
        IndexType beginIndexY,
        IndexType endIndexY,
        IndexType beginIndexZ,
        IndexType endIndexZ,
        const Function& function)
    {
        ParallelFor( beginIndexZ, endIndexZ, [&](size_t k){
            for (IndexType j = beginIndexY; j < endIndexY; ++j){
                for (IndexType i = beginIndexX; i < endIndexX; ++i){
                    function(i,j,k);
                }
            }
        });
    }


    template<typename RandomIterator, typename CompareFunction>
    void ParallelSort(
        RandomIterator begin,
        RandomIterator end,
        CompareFunction compareFunction) {
        if (end < begin) {
            return;
        }

        size_t size = static_cast<size_t>(end - begin);

        typedef typename std::iterator_traits<RandomIterator>::value_type
            value_type;
        std::vector<value_type> temp(size);

        // Estimate number of threads in the pool
        static const unsigned int numThreadsHint
            = std::thread::hardware_concurrency();
        static const unsigned int numThreads
            = (numThreadsHint == 0u ? 8u : numThreadsHint);

        internal::ParallelMergeSort(
            begin, size, temp.begin(), numThreads, compareFunction);
    }

    template<typename RandomIterator>
    void ParallelSort(RandomIterator begin, RandomIterator end) {
        ParallelSort(
            begin,
            end,
            std::less<typename std::iterator_traits<RandomIterator>::value_type>());
    }

}