#pragma once

#include <Arrays/array1_accessor.h>
#include <Arrays/array2_accessor.h>
#include <Arrays/array3_accessor.h>
#include <Geometry/TriangleMesh/triangle3_mesh.h>

#include<cnpy/cnpy.h>
#include<pystring/pystring.h>
#include<gtest/gtest.h>

#include<fstream>
#include<memory>
#include<string>
#include<vector>

#ifdef JET_WINDOWS
#include <direct.h>
#else
#include <sys/stat.h>
#endif


#define JET_TESTS_OUTPUT_DIR "manual_tests_output"

inline void CreateDir(const std::string& dirname)
{
    std::vector<std::string> tokens;
    pystring::split(dirname, tokens, "/");
    std::string PartialDir;
    for (const auto& token : tokens)
    {
        PartialDir = pystring::os::path::join(PartialDir, token);

        #ifdef JET_WINDOWS
            _mkdir(PartialDir.c_str());
        #else
            mkdir(PartialDir.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
        #endif
    }
}

#define JET_TESTS(testSetName) \
    class testSetName##Tests : public ::testing::Test { \
     private: \
        std::string _testCollectionDir; \
        std::string _currentTestCaseName; \
        std::string _currentTestDir; \
     protected: \
        void SetUp() override { \
            _testCollectionDir \
                = pystring::os::path::join( \
                    JET_TESTS_OUTPUT_DIR, #testSetName); \
            CreateDir(_testCollectionDir); \
        } \
        void CreateTestDirectory(const std::string& name) { \
            _currentTestDir = GetTestDirectoryName(name); \
            CreateDir(_currentTestDir); \
        } \
        std::string GetFullFilePath(const std::string& name) { \
            if (!_currentTestDir.empty()) { \
                return pystring::os::path::join(_currentTestDir, name); \
            } else { \
                return name; \
            } \
        } \
        template <typename T> \
        void SaveData( \
            const ConstArrayAccessor1<T>& data, \
            const std::string& name) { \
            std::string filename = GetFullFilePath(name); \
            unsigned int dim[1] = { \
                static_cast<unsigned int>(data.Size()) \
            }; \
            cnpy::npy_save(filename, data.Data(), dim, 1, "w"); \
        } \
        template <typename T> \
        void SaveData( \
            const ConstArrayAccessor1<T>& data, \
            size_t size, const std::string& name) { \
            std::string filename = GetFullFilePath(name); \
            unsigned int dim[1] = { \
                static_cast<unsigned int>(size) \
            }; \
            cnpy::npy_save(filename, data.Data(), dim, 1, "w"); \
        } \
        template <typename T> \
        void SaveData( \
            const ConstArrayAccessor2<T>& data, \
            const std::string& name) { \
            std::string filename = GetFullFilePath(name); \
            unsigned int dim[2] = { \
                static_cast<unsigned int>(data.Height()), \
                static_cast<unsigned int>(data.Width()) \
            }; \
            cnpy::npy_save(filename, data.Data(), dim, 2, "w"); \
        } \
        template <typename T> \
        void SaveData( \
            const ConstArrayAccessor2<T>& data, \
            unsigned int frameNum) { \
            char filename[256]; \
            snprintf( \
                filename, \
                sizeof(filename), \
                "data.#grid2,%04d.npy", \
                frameNum); \
            SaveData(data, filename); \
        } \
        template <typename T> \
        void SaveData( \
            const ConstArrayAccessor3<T>& data, \
            const std::string& name) { \
            std::string filename = GetFullFilePath(name); \
            unsigned int dim[3] = { \
                static_cast<unsigned int>(data.Depth()), \
                static_cast<unsigned int>(data.Height()), \
                static_cast<unsigned int>(data.Width()) \
            }; \
            cnpy::npy_save(filename, data.Data(), dim, 3, "w"); \
        } \
        template <typename T> \
        void SaveData( \
            const ConstArrayAccessor3<T>& data, \
            unsigned int frameNum) { \
            char filename[256]; \
            snprintf( \
                filename, \
                sizeof(filename), \
                "data.#grid3,%04d.npy", \
                frameNum); \
            SaveData(data, filename); \
        } \
        template <typename ParticleSystem> \
        void SaveParticleDataXy( \
            const std::shared_ptr<ParticleSystem>& particles, \
            unsigned int frameNum) { \
            size_t n = particles->NumberOfParticles(); \
            Array1<double> x(n); \
            Array1<double> y(n); \
            auto positions = particles->Positions(); \
            for (size_t i = 0; i < n; ++i) { \
                x[i] = positions[i].x; \
                y[i] = positions[i].y; \
            } \
            char filename[256]; \
            snprintf( \
                filename, \
                sizeof(filename), \
                "data.#point2,%04d,x.npy", \
                frameNum); \
            SaveData(x.ConstAccessor(), filename); \
            snprintf( \
                filename, \
                sizeof(filename), \
                "data.#point2,%04d,y.npy", \
                frameNum); \
            SaveData(y.ConstAccessor(), filename); \
        } \
        void SaveTriangleMeshData( \
            const TriangleMesh3& data, \
            const std::string& name) { \
            std::string filename = GetFullFilePath(name); \
            std::ofstream file(filename.c_str()); \
            if (file) { \
                data.WriteObj(&file); \
                file.close(); \
            } \
        } \
        std::string GetTestDirectoryName(const std::string& name) { \
            return pystring::os::path::join(_testCollectionDir, name); \
        } \
    }; \


#define JET_BEGIN_TEST_F(testSetName, testCaseName) \
    TEST_F(testSetName##Tests, testCaseName) { \
        CreateTestDirectory(#testCaseName);

#define JET_END_TEST_F }