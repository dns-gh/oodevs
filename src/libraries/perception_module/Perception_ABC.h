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
    typedef std::vector< wrapper::View > T_ObjectVector;
    typedef std::vector< wrapper::View > T_ConstPopulationFlowVector;
    typedef std::vector< wrapper::View > T_ConstPopulationConcentrationVector;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             Perception_ABC() {}
    virtual ~Perception_ABC() {}
    //@}

    //! @name Execution
    //@{
    virtual const PerceptionLevel& ComputePoint( const wrapper::View& perceiver, const SurfacesAgent_ABC& surfaces, const MT_Vector2D& vPoint ) const;

    virtual void                   ExecuteAgents( const wrapper::View& model, const wrapper::View& perceiver, const SurfacesAgent_ABC& surfaces, const T_AgentPtrVector& perceivableAgents );
    virtual const PerceptionLevel& ComputeAgent( const wrapper::View& model, const wrapper::View& perceiver, const SurfacesAgent_ABC& surfaces, const wrapper::View& target ) const;

    virtual void                   ExecuteObjects( const wrapper::View& model, const wrapper::View& perceiver, const SurfacesObject_ABC& surfaces, const T_ObjectVector& perceivableObjects );
    virtual const PerceptionLevel& ComputeObject( const wrapper::View& perceiver, const SurfacesObject_ABC& surfaces, const wrapper::View& knowledgeObject ) const;

    virtual void                   ExecuteFlows( const wrapper::View& perceiver, const SurfacesAgent_ABC& surfaces, const T_ConstPopulationFlowVector& perceivableFlows );

    virtual void                   ExecuteConcentrations( const wrapper::View& perceiver, const SurfacesAgent_ABC& surfaces, const T_ConstPopulationConcentrationVector& perceivableConcentrations );
    virtual const PerceptionLevel& ComputeConcentration( const wrapper::View& perceiver, const SurfacesAgent_ABC& surfaces, const wrapper::View& concentration ) const;

    virtual void FinalizePerception( const wrapper::View& perceiver, const SurfacesAgent_ABC& surfaces );
    //@}
};

}
}

#endif // SWORD_PERCEPTION_PERCEPTION_ABC_H
