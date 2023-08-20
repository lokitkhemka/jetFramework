#include <gtest/gtest.h>
#include <Arrays/array1.h>



TEST(Array1, Constructors){
    {
        jet::Array1<float> arr;
        EXPECT_EQ(0u, arr.Size());
    }

    {
        jet::Array1<float> arr(9, 1.5f);
        EXPECT_EQ(9u, arr.Size());
        for (size_t i = 0; i<9; ++i)
        {
            EXPECT_FLOAT_EQ(1.5f, arr[i]);
        }
    }

    {
        jet::Array1<float> arr({1.f, 2.f, 3.f, 4.f});
        EXPECT_EQ(4u, arr.Size());
        for(size_t i =0; i<4; ++i)
        {
            EXPECT_FLOAT_EQ((float)i + 1.f, arr[i]);
        }
    }
    {
        jet::Array1<float> arr({1.f,  2.f,  3.f,  4.f});
        jet::Array1<float> arr1(arr);
        EXPECT_EQ(4u, arr1.Size());
        for (size_t i = 0; i < 4; ++i) {
            EXPECT_FLOAT_EQ((float)i + 1.f, arr1[i]);
        }
    }
}

TEST(Array1, SetMethods)
{
    jet::Array1<float> arr1(12,-1.f);
    arr1.Set(3.5f);
    for (float a: arr1)
    {
        EXPECT_EQ(3.5f, a);
    }

    jet::Array1<float> arr2;
    arr2.Set(arr1);
    EXPECT_EQ(arr1.Size(), arr2.Size());
    for(size_t i=0; i<arr2.Size(); ++i)
    {
        EXPECT_EQ(arr1[i],arr2[2]);
    }

    arr2 = {2.f, 5.f, 9.f, -1.f};
    EXPECT_EQ(4u, arr2.Size());
    EXPECT_EQ(2.f, arr2[0]);
    EXPECT_EQ(5.f, arr2[1]);
    EXPECT_EQ(9.f, arr2[2]);
    EXPECT_EQ(-1.f, arr2[3]);
}

TEST(Array1, Clear)
{
    jet::Array1<float> arr1 = {2.f, 5.f, 9.f, -1.f};
    arr1.Clear();
    EXPECT_EQ(0u, arr1.Size());
}

TEST(Array1, Resize)
{
    jet::Array1<float> arr;
    arr.Resize(9);
    EXPECT_EQ(9u, arr.Size());
    for(size_t i=0; i<0; ++i)
    {
        EXPECT_FLOAT_EQ(0.f, arr[i]);
    }

    arr.Resize(12, 4.f);
    EXPECT_EQ(12u, arr.Size());
    for (size_t i=0; i<12; ++i)
    {
        if(i<9){
            EXPECT_FLOAT_EQ(0.f, arr[i]);
        }
        else
        {
            EXPECT_FLOAT_EQ(4.f, arr[i]);
        }
    }
}

TEST(Array1, Iterators)
{
    jet::Array1<float> arr1 = {6.f, 4.f, 1.f, -5.f};

    size_t i = 0;
    for(float& f:arr1)
    {
        EXPECT_FLOAT_EQ(arr1[i], f);
        ++i;
    }

    i=0;
    for(const float& elem:arr1)
    {
        EXPECT_FLOAT_EQ(arr1[i], elem);
        ++i;
    }
}

TEST(Array1, ForEach)
{
    jet::Array1<float> arr1 = {6.f,4.f, 1.f, -5.f};
    size_t i = 0;
    arr1.ForEach([&](float val){
        EXPECT_FLOAT_EQ(arr1[i], val);
        ++i;
    });
}

TEST(Array1, ForEachIndex)
{
    jet::Array1<float> arr1 = {6.f,4.f, 1.f, -5.f};
    size_t count = 0;
    arr1.ForEachIndex([&](size_t i){
        EXPECT_EQ(count, i);
        ++count;
    });
}


