// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_PERCEPTION_PERCEPTION_ABC_H
#define SWORD_PERCEPTION_PERCEPTION_ABC_H

#include <vector>
#include <boost/noncopyable.hpp>

class DEC_Knowledge_Object;
class MIL_Object_ABC;
class MIL_PopulationFlow;
class MIL_PopulationConcentration;
class MT_Vector2D;

namespace sword
{
namespace wrapper
{
    class View;
}
namespace perception
{
    class PerceptionLevel;
    class SurfacesAgent_ABC;
    class SurfacesObject_ABC;

//*****************************************************************************
// Created: DFT 02-02-26
// Last modified: JVT 03-09-01
//*****************************************************************************
class Perception_ABC : private boost::noncopyable
{
public:
    //! @name Types
    //@{
    typedef std::vector< wrapper::View > T_AgentPtrVector;
    typedef std::vector< MIL_Object_ABC* > T_ObjectVector;
    typedef std::vector< const MIL_PopulationFlow* > T_ConstPopulationFlowVector;
    typedef std::vector< const MIL_PopulationConcentration* > T_ConstPopulationConcentrationVector;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             Perception_ABC() {}
    virtual ~Perception_ABC() {}
    //@}

    //! @name Execution
    //@{
    virtual const PerceptionLevel& Compute( const wrapper::View& perceiver, const SurfacesAgent_ABC& surfaces, const MT_Vector2D& vPoint ) const;

    virtual void                   Execute( const wrapper::View& perceiver, const SurfacesAgent_ABC& surfaces, const T_AgentPtrVector& perceivableAgents );
    virtual const PerceptionLevel& Compute( const wrapper::View& perceiver, const SurfacesAgent_ABC& surfaces, const wrapper::View& target );

    virtual void                   Execute( const wrapper::View& perceiver, const SurfacesObject_ABC& surfaces, const T_ObjectVector& perceivableObjects );
    virtual const PerceptionLevel& Compute( const wrapper::View& perceiver, const SurfacesObject_ABC& surfaces, const DEC_Knowledge_Object& knowledge ) const;

    virtual const PerceptionLevel& Compute( const wrapper::View& perceiver, const SurfacesObject_ABC& surfaces, const MIL_Object_ABC& object ) const;
    virtual void                   Execute( const wrapper::View& perceiver, const SurfacesAgent_ABC& surfaces, const T_ConstPopulationFlowVector& perceivableFlows );

    virtual void                   Execute( const wrapper::View& perceiver, const SurfacesAgent_ABC& surfaces, const T_ConstPopulationConcentrationVector& perceivableConcentrations );
    virtual const PerceptionLevel& Compute( const wrapper::View& perceiver, const SurfacesAgent_ABC& surfaces, const MIL_PopulationConcentration& concentration ) const;

    virtual void FinalizePerception( const wrapper::View& perceiver, const SurfacesAgent_ABC& surfaces ) = 0;
    //@}
};

}
}

#endif // SWORD_PERCEPTION_PERCEPTION_ABC_H
