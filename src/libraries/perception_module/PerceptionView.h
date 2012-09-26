// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_PERCEPTION_PERCEPTION_VIEW_H
#define SWORD_PERCEPTION_PERCEPTION_VIEW_H

#include "Perception_ABC.h"
#include "MT_Tools/Mt_Vector2DTypes.h"
#include <map>

namespace sword
{
namespace perception
{
    class PerceptionObserver_ABC;

// =============================================================================
/** @class  PerceptionView
    @brief  Perception view
*/
// Created: SLI 2012-04-12
// =============================================================================
class PerceptionView : public Perception_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             PerceptionView( const wrapper::View& model, const wrapper::View& entity, PerceptionObserver_ABC& observer );
    virtual ~PerceptionView();
    //@}

    //! @name Execution
    //@{
    virtual const PerceptionLevel& ComputePoint( const wrapper::View& perceiver, const SurfacesAgent_ABC& surfaces, const MT_Vector2D& vPoint ) const;

    virtual void                   ExecuteAgents( const wrapper::View& model, const wrapper::View& perceiver, const SurfacesAgent_ABC& surfaces, const T_AgentPtrVector& perceivableAgents );
    virtual const PerceptionLevel& ComputeAgent( const wrapper::View& perceiver, const SurfacesAgent_ABC& surfaces, const wrapper::View& target ) const;

    virtual void                   ExecuteObjects( const wrapper::View& perceiver, const SurfacesObject_ABC& surfaces, const T_ObjectVector& perceivableObjects );
    virtual const PerceptionLevel& ComputeObject( const wrapper::View& perceiver, const SurfacesObject_ABC& surfaces, const wrapper::View& knowledgeObject ) const;

    virtual void                   ExecuteFlows( const wrapper::View& perceiver, const SurfacesAgent_ABC& surfaces, const T_ConstPopulationFlowVector& perceivableFlows );

    virtual void                   ExecuteConcentrations( const wrapper::View& perceiver, const SurfacesAgent_ABC& surfaces, const T_ConstPopulationConcentrationVector& perceivableConcentrations );
    virtual const PerceptionLevel& ComputeConcentration( const wrapper::View& perceiver, const SurfacesAgent_ABC& surfaces, const wrapper::View& concentration ) const;

    virtual void FinalizePerception( const wrapper::View& perceiver, const SurfacesAgent_ABC& surfaces );
    //@}

private:
    //! @name Helpers
    //@{
    void FinalizeSurfaceAgents();
    void TransferPerception( const wrapper::View& perceiver, const SurfacesAgent_ABC& surfaces ) const;
    bool IsEnabled( const wrapper::View& perceiver ) const;
    virtual const PerceptionLevel& ComputeFlow( const wrapper::View& perceiver, const SurfacesAgent_ABC& surfaces, const wrapper::View& flow, T_PointVector& shape ) const;
    //@}

    //! @name Types
    //@{
    typedef std::pair< unsigned int, double > T_PerceptionParameterPair;

    typedef std::map< std::size_t, T_PerceptionParameterPair > T_PerceptionTickMap;
    typedef T_PerceptionTickMap::const_iterator               CIT_PerceptionTickMap;
    typedef T_PerceptionTickMap::iterator                      IT_PerceptionTickMap;
    //@}

private:
    //! @name Member data
    //@{
    const size_t identifier_;
    PerceptionObserver_ABC& observer_;
    mutable T_PerceptionTickMap perceptionsBuffer_;
    mutable T_PerceptionTickMap perceptionsUnderway_;
    mutable bool wasInCity_;
    //@}
};

}
}

#endif // SWORD_PERCEPTION_PERCEPTION_VIEW_H
