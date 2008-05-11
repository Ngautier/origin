#include <sofa/component/odesolver/RungeKutta4Solver.h>
#include <sofa/simulation/tree/MechanicalVisitor.h>
#include <sofa/core/ObjectFactory.h>
#include <math.h>


namespace sofa
{

namespace component
{

namespace odesolver
{

using namespace core::componentmodel::behavior;
using namespace sofa::defaulttype;

int RungeKutta4SolverClass = core::RegisterObject("A popular explicit time integrator")
        .add< RungeKutta4Solver >()
        .addAlias("RungeKutta4")
        ;

SOFA_DECL_CLASS(RungeKutta4);


void RungeKutta4Solver::solve(double dt)
{
    //std::cout << "RK4 Init\n";
    MultiVector pos(this, VecId::position());
    MultiVector vel(this, VecId::velocity());
    MultiVector k1a(this, VecId::V_DERIV);
    MultiVector k2a(this, VecId::V_DERIV);
    MultiVector k3a(this, VecId::V_DERIV);
    MultiVector k4a(this, VecId::V_DERIV);
    MultiVector& k1v = vel; //(this, VecId::V_DERIV);
    MultiVector k2v(this, VecId::V_DERIV);
    MultiVector k3v(this, VecId::V_DERIV);
    MultiVector k4v(this, VecId::V_DERIV);
    MultiVector newX(this, VecId::V_COORD);
    //MultiVector newV(this, VecId::V_DERIV);
    const bool log = this->f_printLog.getValue();

    double stepBy2 = double(dt / 2.0);
    double stepBy3 = double(dt / 3.0);
    double stepBy6 = double(dt / 6.0);

    double startTime = this->getTime();

    addSeparateGravity(dt);	// v += dt*g . Used if mass wants to added G separately from the other forces to v.

    //First step
    if (log) std::cout << "RK4 Step 1\n";
    //k1v = vel;
    computeAcc (startTime, k1a, pos, vel);

    //Step 2
    if (log) std::cout << "RK4 Step 2\n";
#ifdef SOFA_NO_VMULTIOP // unoptimized version
    newX = pos;
    k2v = vel;
    newX.peq(k1v, stepBy2);
    k2v.peq(k1a, stepBy2);
#else // single-operation optimization
    {
        simulation::tree::MechanicalVMultiOpVisitor vmop;
        vmop.ops.resize(2);
        vmop.ops[0].first = (VecId)newX;
        vmop.ops[0].second.push_back(std::make_pair((VecId)pos,1.0));
        vmop.ops[0].second.push_back(std::make_pair((VecId)k1v,stepBy2));
        vmop.ops[1].first = (VecId)k2v;
        vmop.ops[1].second.push_back(std::make_pair((VecId)vel,1.0));
        vmop.ops[1].second.push_back(std::make_pair((VecId)k1a,stepBy2));
        vmop.execute(this->getContext());
    }
#endif

    computeAcc ( startTime+stepBy2, k2a, newX, k2v );

    // step 3
    if (log) std::cout << "RK4 Step 3\n";
#ifdef SOFA_NO_VMULTIOP // unoptimized version
    newX = pos;
    k3v = vel;
    newX.peq(k2v, stepBy2);
    k3v.peq(k2a, stepBy2);
#else // single-operation optimization
    {
        simulation::tree::MechanicalVMultiOpVisitor vmop;
        vmop.ops.resize(2);
        vmop.ops[0].first = (VecId)newX;
        vmop.ops[0].second.push_back(std::make_pair((VecId)pos,1.0));
        vmop.ops[0].second.push_back(std::make_pair((VecId)k2v,stepBy2));
        vmop.ops[1].first = (VecId)k3v;
        vmop.ops[1].second.push_back(std::make_pair((VecId)vel,1.0));
        vmop.ops[1].second.push_back(std::make_pair((VecId)k2a,stepBy2));
        vmop.execute(this->getContext());
    }
#endif

    computeAcc ( startTime+stepBy2, k3a, newX, k3v );

    // step 4
    if (log) std::cout << "RK4 Step 4\n";
#ifdef SOFA_NO_VMULTIOP // unoptimized version
    newX = pos;
    k4v = vel;
    newX.peq(k3v, dt);
    k4v.peq(k3a, dt);
#else // single-operation optimization
    {
        simulation::tree::MechanicalVMultiOpVisitor vmop;
        vmop.ops.resize(2);
        vmop.ops[0].first = (VecId)newX;
        vmop.ops[0].second.push_back(std::make_pair((VecId)pos,1.0));
        vmop.ops[0].second.push_back(std::make_pair((VecId)k3v,dt));
        vmop.ops[1].first = (VecId)k4v;
        vmop.ops[1].second.push_back(std::make_pair((VecId)vel,1.0));
        vmop.ops[1].second.push_back(std::make_pair((VecId)k3a,dt));
        vmop.execute(this->getContext());
    }
#endif

    computeAcc( startTime+dt, k4a, newX, k4v);

    if (log) std::cout << "RK4 Final\n";
#ifdef SOFA_NO_VMULTIOP // unoptimized version
    pos.peq(k1v,stepBy6);
    vel.peq(k1a,stepBy6);
    pos.peq(k2v,stepBy3);
    vel.peq(k2a,stepBy3);
    pos.peq(k3v,stepBy3);
    vel.peq(k3a,stepBy3);
    pos.peq(k4v,stepBy6);
    vel.peq(k4a,stepBy6);
#else // single-operation optimization
    {
        simulation::tree::MechanicalVMultiOpVisitor vmop;
        vmop.ops.resize(2);
        vmop.ops[0].first = (VecId)pos;
        vmop.ops[0].second.push_back(std::make_pair((VecId)pos,1.0));
        vmop.ops[0].second.push_back(std::make_pair((VecId)k1v,stepBy6));
        vmop.ops[0].second.push_back(std::make_pair((VecId)k2v,stepBy3));
        vmop.ops[0].second.push_back(std::make_pair((VecId)k3v,stepBy3));
        vmop.ops[0].second.push_back(std::make_pair((VecId)k4v,stepBy6));
        vmop.ops[1].first = (VecId)vel;
        vmop.ops[1].second.push_back(std::make_pair((VecId)vel,1.0));
        vmop.ops[1].second.push_back(std::make_pair((VecId)k1a,stepBy6));
        vmop.ops[1].second.push_back(std::make_pair((VecId)k2a,stepBy3));
        vmop.ops[1].second.push_back(std::make_pair((VecId)k3a,stepBy3));
        vmop.ops[1].second.push_back(std::make_pair((VecId)k4a,stepBy6));
        vmop.execute(this->getContext());
    }
#endif
}



} // namespace odesolver

} // namespace component

} // namespace sofa

