#include "serialization.h"
#include<cstring>
#include<vector>

#include<jet.h>
#include "generated/flat_data_generated.h"


namespace jet
{
    template<typename T>
    void Serialize(const Array1<T>& array, std::vector<uint8_t>* buffer)
    {
        size_t size = sizeof(T) * array.Size();
        Serialize(reinterpret_cast<const uint8_t*>(array.Data()), size, buffer);
    }

    template<typename T>
    void Deserialize(const std::vector<uint8_t>& buffer, Array1<T>* array)
    {
        std::vector<uint8_t> data;
        Deserialize(buffer, &data);
        array->Resize(data.size()/sizeof(T));
        memcpy(array->Data(), data.data(), data.size());
    }

    void Serialize(const Serializable* serializable, std::vector<uint8_t>* buffer)
    {
        serializable->Serialize(buffer);
    }

    void Serialize(const uint8_t* data, size_t size, std::vector<uint8_t>* buffer)
    {
        flatbuffers::FlatBufferBuilder builder(1024);

        auto fbsData = fbs::CreateFlatData(builder, builder.CreateVector(data, size));

        builder.Finish(fbsData);

        uint8_t *buf = builder.GetBufferPointer();
        size_t sz = builder.GetSize();

        buffer->resize(sz);

        memcpy(buffer->data(), buf, sz);
    }

    void Deserialize(const std::vector<uint8_t>& buffer, Serializable* serializable)
    {
        serializable->Deserialize(buffer);
    }

    void Deserialize(const std::vector<uint8_t>& buffer, std::vector<uint8_t>* data)
    {
        auto fbsData = fbs::GetFlatData(buffer.data());
        data->resize(fbsData->data()->size());

        std::copy(fbsData->data()->begin(), fbsData->data()->end(), data->begin());
    }
}