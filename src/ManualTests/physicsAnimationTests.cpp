// Copyright (c) 2016 Doyub Kim

#include "manual_tests.h"

#include <Arrays/array1.h>
#include <Field/VectorField/constant_vector_field3.h>
#include <Animation/physics_animation.h>
#include <Vector/vector3.h>

using namespace jet;

class SimpleMassSpringAnimation : public PhysicsAnimation
{
public:
    // To store connectivity between nodes
    struct Edge
    {
        size_t First;
        size_t Second;
    };
    //To Store the constraints on the nodes.
    struct Constraint
    {
        size_t PointIndex;
        Vector3D FixedPosition;
        Vector3D FixedVelocity;
    };

    std::vector<Vector3D> Positions;
    std::vector<Vector3D> Velocities;
    std::vector<Vector3D> Forces;
    std::vector<Edge> Edges;

    double Mass = 1.0; //in kg
    Vector3D Gravity = Vector3D(0.0, -9.8, 0.0);// 9.8 m/s^2 in -y direction.
    double Stiffness = 500.0; // In N/m^2
    double RestLength = 1.0; // In m
    double DampingCoefficient = 1.0; // In kg/s
    double DragCoefficient = 0.1;

    double FloorPosY = -7.0; // In m
    double RestitutionCoefficient = 0.3;

    VectorField3Ptr Wind;

    std::vector<Constraint> constraints;

    SimpleMassSpringAnimation()
    {}

    void MakeChain(size_t NumberOfPoints)
    {
        if (NumberOfPoints == 0)
            return;

        size_t NumberOfEdges = NumberOfPoints - 1;
        Positions.resize(NumberOfPoints);
        Velocities.resize(NumberOfPoints);
        Edges.resize(NumberOfEdges);
        Forces.resize(NumberOfPoints);

        for (size_t i = 0; i < NumberOfPoints; ++i)
            Positions[i].x = - static_cast<double>(i); // y and z values are initialized as 0 by default.

        for (size_t i = 0; i < NumberOfEdges; ++i)
            Edges[i] = Edge{i, i+1};
    }

    void ExportStates(Array1<double>& x, Array1<double>& y) const
    {
        x.Resize(Positions.size());
        y.Resize(Positions.size());

        for (size_t i = 0; i < Positions.size(); ++i)
        {
            x[i] = Positions[i].x;
            y[i] = Positions[i].y;
        }
    }

protected:
    void OnAdvanceSubTimeStep(double TimeIntervalInSeconds) override
    {
        size_t NumberOfPoints = Positions.size();
        size_t NumberOfEdges = Edges.size();

        //Compute Forces.
        for (size_t i = 0; i < NumberOfPoints; ++i)
        {
            //Gravity Force
            Forces[i] = Mass * Gravity;

            //Air Drag Force
            Vector3D RelVel = Velocities[i];
            if(Wind != nullptr)
            {
                RelVel -= Wind->Sample(Positions[i]);
            }

            Forces[i] += -DragCoefficient * RelVel;
        }

        //Compute Damping and Spring Forces.
        for (size_t i =0; i < NumberOfEdges; ++i)
        {
            size_t PointIndex0 = Edges[i].First;
            size_t PointIndex1 = Edges[i].Second;

            //Computing the Spring Force.
            Vector3D Pos0 = Positions[PointIndex0];
            Vector3D Pos1 = Positions[PointIndex1];
            Vector3D r = Pos0 - Pos1;
            double Distance = r.Length();

            if (Distance > 0.0)
            {
                Vector3D Force = -Stiffness * (Distance - RestLength) * r.Normalized();
                Forces[PointIndex0] += Force;
                Forces[PointIndex1] -= Force;
            }

            //Add Damping Force
            Vector3D Vel0 = Velocities[PointIndex0];
            Vector3D Vel1 = Velocities[PointIndex1];
            Vector3D RelVel0 = Vel0 - Vel1;
            Vector3D Damping = -DampingCoefficient * RelVel0;
            Forces[PointIndex0] += Damping;
            Forces[PointIndex1] -= Damping;
        }

        //Update states
        for (size_t i = 0; i < NumberOfPoints; ++i)
        {
            //Computing the new states
            Vector3D NewAcceleration = Forces[i] / Mass;
            Vector3D NewVelocity = Velocities[i] + TimeIntervalInSeconds * NewAcceleration;
            Vector3D NewPos =  Positions[i] + TimeIntervalInSeconds * NewVelocity;

            //Collisions
            if (NewPos.y < FloorPosY)
            {
                NewPos.y = FloorPosY;

                if(NewVelocity.y < 0.0)
                {
                    NewVelocity.y *= - RestitutionCoefficient;
                    NewPos.y += TimeIntervalInSeconds * NewVelocity.y;
                }
            }

            //Update States
            Velocities[i] = NewVelocity;
            Positions[i] = NewPos;
        }

        //Apply Constraints.
        for (size_t i = 0; i < constraints.size(); ++i)
        {
            size_t PointIndex = constraints[i].PointIndex;
            Positions[PointIndex] = constraints[i].FixedPosition;
            Velocities[PointIndex] = constraints[i].FixedVelocity;
        }
    }

};

JET_TESTS(PhysicsAnimation)

JET_BEGIN_TEST_F(PhysicsAnimation, SimpleMassSpringAnimation)
{
    Array1<double> x;
    Array1<double> y;

    SimpleMassSpringAnimation Anim;
    Anim.MakeChain(10);
    Anim.Wind = std::make_shared<ConstantVectorField3>(Vector3D(30.0, 0.0, 0.0));
    Anim.constraints.push_back(SimpleMassSpringAnimation::Constraint{0, Vector3D(), Vector3D()});
    Anim.ExportStates(x,y);

    char filename[256];
    snprintf(filename, sizeof(filename), "data.#line2,0000,x.npy");
    SaveData(x.ConstAccessor(), filename);
    snprintf(filename, sizeof(filename), "data.#line2,0000,y.npy");
    SaveData(y.ConstAccessor(), filename);

    Frame frame(1, 1.0 / 60.0);
    for( ; frame.Index < 360; frame.Advance())
    {
        Anim.Update(frame);
        Anim.ExportStates(x,y);

        snprintf(filename, sizeof(filename), "data.#line2,%04d,x.npy", frame.Index);
        SaveData(x.ConstAccessor(), filename);
        snprintf(filename, sizeof(filename), "data.#line2,%04d,y.npy", frame.Index);
        SaveData(y.ConstAccessor(), filename);
    }
}
JET_END_TEST_F
