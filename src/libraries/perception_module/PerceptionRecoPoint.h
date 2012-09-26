// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_PERCEPTION_PERCEPTION_RECO_POINT_H
#define SWORD_PERCEPTION_PERCEPTION_RECO_POINT_H

#include "PerceptionWithLocation.h"
#include "PerceptionLocalisation.h"
#include "MT_Tools/MT_Vector2D.h"

namespace sword
{
namespace perception
{
    class PerceptionObserver_ABC;

class PerceptionRecoPointReco : public PerceptionLocalisation
{
public:
    const MT_Vector2D vCenter_;
    double          rCurrentSize_;
    const double    rFinalSize_;
    const double    rGrowthSpeed_;
    bool            bProcessed_;

    PerceptionRecoPointReco( const wrapper::View& perception, const wrapper::View& entity );
private:
    PerceptionRecoPointReco& operator = ( const PerceptionRecoPointReco& );
};

// =============================================================================
// @class  PerceptionRecoPoint
// Created: JVT 2004-10-21
// =============================================================================
class PerceptionRecoPoint : public PerceptionWithLocation< PerceptionRecoPointReco >
{
public:
    //! @name Constructors/Destructor
    //@{
             PerceptionRecoPoint( const wrapper::View& model, const wrapper::View& entity, PerceptionObserver_ABC& observer );
    virtual ~PerceptionRecoPoint();
    //@}

    //! @name Execution
    //@{
    virtual const PerceptionLevel& ComputePoint( const wrapper::View& perceiver, const SurfacesAgent_ABC& surfaces, const MT_Vector2D& vPoint ) const;

    virtual void                   ExecuteAgents( const wrapper::View& model, const wrapper::View& perceiver, const SurfacesAgent_ABC& surfaces, const T_AgentPtrVector& perceivableAgents );
    virtual const PerceptionLevel& ComputeAgent( const wrapper::View& perceiver, const SurfacesAgent_ABC& surfaces, const wrapper::View& target ) const;

    virtual void                   ExecuteObjects( const wrapper::View& perceiver, const SurfacesObject_ABC& surfaces, const T_ObjectVector& perceivableObjects );
    virtual const PerceptionLevel& ComputeObject( const wrapper::View& perceiver, const SurfacesObject_ABC& surfaces, const wrapper::View& knowledge ) const;
    //@}

private:
    //! @name Helpers
    //@{
    void AddLocalisation( const wrapper::View& perception, const wrapper::View& entity );
    //@}

private:
    //! @name Member data
    //@{
    PerceptionObserver_ABC& observer_;
    //@}
};

}
}

#endif // SWORD_PERCEPTION_PERCEPTION_RECO_POINT_H
