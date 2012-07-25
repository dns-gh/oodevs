// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_PERCEPTION_PERCEPTION_RECO_SURVEILLANCE_H
#define SWORD_PERCEPTION_PERCEPTION_RECO_SURVEILLANCE_H

#include "PerceptionWithLocation.h"
#include "PerceptionLocalisation.h"

class TER_Localisation;

namespace xml
{
    class xistream;
}

namespace sword
{
namespace perception
{
    class PerceptionObserver_ABC;

class PerceptionRecoSurveillanceReco : public PerceptionLocalisation
{
public:
    PerceptionRecoSurveillanceReco( const TER_Localisation* localisation, unsigned int currentTimeStep );

    bool IsInside       ( const MT_Vector2D& ) const;
    void GetAgentsInside( const wrapper::View&, Perception_ABC::T_AgentPtrVector& ) const;

private:
    PerceptionRecoSurveillanceReco& operator = ( const PerceptionRecoSurveillanceReco& );

private:
    const TER_Localisation* localisation_;
    const unsigned int currentTimeStep_;
    const unsigned int nForestDetectionTimeStep_;
    const unsigned int nUrbanDetectionTimeStep_;
    const unsigned int nEmptyDetectionTimeStep_;
};

// =============================================================================
// @class  PerceptionRecoSurveillance
// Created: JVT 2004-10-21
// Modified: JVT 2004-10-28
// =============================================================================
class PerceptionRecoSurveillance : public PerceptionWithLocation< PerceptionRecoSurveillanceReco >
{
public:
             PerceptionRecoSurveillance( const wrapper::View& model, const wrapper::View& entity, PerceptionObserver_ABC& observer );
    virtual ~PerceptionRecoSurveillance();

    //! @name Initialization
    //@{
    static void Initialize( xml::xistream& xis );
    //@}

    //! @name Execution
    //@{
    virtual const PerceptionLevel& Compute( const wrapper::View& perceiver, const SurfacesAgent_ABC& surfaces, const MT_Vector2D& vPoint ) const;

    virtual void                   Execute( const wrapper::View& model, const wrapper::View& perceiver, const SurfacesAgent_ABC& surfaces, const T_AgentPtrVector& perceivableAgents );
    virtual const PerceptionLevel& Compute( const wrapper::View& perceiver, const SurfacesAgent_ABC& surfaces, const wrapper::View& target ) const;
    //@}

private:
    //! @name Helpers
    //@{
    struct LoadingWrapper;
    static void ReadAlatTime( xml::xistream& xis );
    void AddLocalisation( const std::string& /*key*/, const wrapper::View& perception, unsigned int currentTimeStep );
    //@}

private:
    //! @name Member data
    //@{
    PerceptionObserver_ABC& observer_;
    //@}
};

}
}

#endif // SWORD_PERCEPTION_PERCEPTION_RECO_SURVEILLANCE_H
