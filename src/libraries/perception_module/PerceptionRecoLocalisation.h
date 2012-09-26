// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_PERCEPTION_PERCEPTION_RECO_LOCALISATION_H
#define SWORD_PERCEPTION_PERCEPTION_RECO_LOCALISATION_H

#include "PerceptionWithLocation.h"
#include "PerceptionLocalisation.h"
#include "wrapper/View.h"

namespace sword
{
namespace perception
{
    class PerceptionObserver_ABC;

class PerceptionRecoLocalisationReco : public PerceptionLocalisation
{
public:
    PerceptionRecoLocalisationReco( const wrapper::View& perception, const wrapper::View& entity );

    bool IsInside        ( const wrapper::View&, const MT_Vector2D& ) const;
    void GetAgentsInside ( const wrapper::View&, const wrapper::View&, Perception_ABC::T_AgentPtrVector& ) const;

private:
    PerceptionRecoLocalisationReco& operator = ( const PerceptionRecoLocalisationReco& );

private:
    const wrapper::View localisation_;
    const bool          bShouldUseRadius_;

public:
    double rRadius_;
    double rCurrentRadius_;
    double rGrowthSpeed_;
};

// =============================================================================
// @class  PerceptionRecoLocalisation
// Created: JVT 2004-10-21
// Modified: JVT 2004-10-28
// =============================================================================
class PerceptionRecoLocalisation : public PerceptionWithLocation< PerceptionRecoLocalisationReco >
{
public:
    //! @name Constructors/Destructor
    //@{
             PerceptionRecoLocalisation( const wrapper::View& model, const wrapper::View& entity, PerceptionObserver_ABC& observer );
    virtual ~PerceptionRecoLocalisation();
    //@}

    //! @name Execution
    //@{
    virtual const PerceptionLevel& ComputePoint( const wrapper::View& perceiver, const SurfacesAgent_ABC& surfaces, const MT_Vector2D& vPoint ) const;

    virtual void                   ExecuteAgents( const wrapper::View& model, const wrapper::View& perceiver, const SurfacesAgent_ABC& surfaces, const T_AgentPtrVector& perceivableAgents );
    virtual const PerceptionLevel& ComputeAgent( const wrapper::View& model, const wrapper::View& perceiver, const SurfacesAgent_ABC& surfaces, const wrapper::View& target ) const;
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

#endif // SWORD_PERCEPTION_PERCEPTION_RECO_LOCALISATION_H
