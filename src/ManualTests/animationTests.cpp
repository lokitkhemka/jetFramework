#include "manual_tests.h"

#include <Animation/animation.h>
#include <Arrays/array1.h>

using namespace jet;

class SineAnimation : public Animation
{
public:
    double x = 0.0;

protected:
    void OnUpdate(const Frame& frame) override
    {
        x =std::sin(10.0 * frame.TimeInSeconds());
    }
};


class SineWithDecayAnimation : public Animation
{
public:
    double x = 0.0;

protected:
    void OnUpdate(const Frame& frame) override
    {
        double decay = exp(- frame.TimeInSeconds());
        x = std::sin ( 10.0 * frame.TimeInSeconds()) * decay;
    }
};

JET_TESTS(Animation);

JET_BEGIN_TEST_F(Animation, Sine)
{
    Array1<double> t(240);
    Array1<double> data(240);

    SineAnimation sinAnim;

    char filename[256];
    snprintf(filename, sizeof(filename), "data.#line2,0000,x.npy");
    SaveData(t.ConstAccessor(), 0 , filename);
    snprintf(filename, sizeof(filename), "data.#line2,0000,y.npy");
    SaveData(data.ConstAccessor(), 0, filename);
    
    for (Frame frame; frame.Index < 240; frame.Advance()) {
        sinAnim.Update(frame);

        t[frame.Index] = frame.TimeInSeconds();
        data[frame.Index] = sinAnim.x;

        snprintf(
            filename,
            sizeof(filename),
            "data.#line2,%04d,x.npy",
            frame.Index);
        SaveData(t.ConstAccessor(), frame.Index, filename);
        snprintf(
            filename,
            sizeof(filename),
            "data.#line2,%04d,y.npy",
            frame.Index);
        SaveData(data.ConstAccessor(), frame.Index, filename);
    }

    SaveData(t.ConstAccessor(), "data.#line2,x.npy");
    SaveData(data.ConstAccessor(), "data.#line2,y.npy");

}
JET_END_TEST_F


JET_BEGIN_TEST_F(Animation, SineDecay) {
    Array1<double> t(240);
    Array1<double> data(240);

    SineWithDecayAnimation SinDecay;

    char filename[256];
    snprintf(filename, sizeof(filename), "data.#line2,0000,x.npy");
    SaveData(t.ConstAccessor(), 0, filename);
    snprintf(filename, sizeof(filename), "data.#line2,0000,y.npy");
    SaveData(data.ConstAccessor(), 0, filename);

    for (Frame frame; frame.Index < 240; frame.Advance()) {
        SinDecay.Update(frame);

        t[frame.Index] = frame.TimeInSeconds();
        data[frame.Index] = SinDecay.x;

        snprintf(
            filename,
            sizeof(filename),
            "data.#line2,%04d,x.npy",
            frame.Index);
        SaveData(t.ConstAccessor(), frame.Index, filename);
        snprintf(
            filename,
            sizeof(filename),
            "data.#line2,%04d,y.npy",
            frame.Index);
        SaveData(data.ConstAccessor(), frame.Index, filename);
    }

    SaveData(t.ConstAccessor(), "data.#line2,x.npy");
    SaveData(data.ConstAccessor(), "data.#line2,y.npy");
}
JET_END_TEST_F