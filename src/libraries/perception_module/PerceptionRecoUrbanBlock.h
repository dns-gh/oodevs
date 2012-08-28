// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef SWORD_PERCEPTION_PERCEPTION_RECO_URBAN_BLOCK_H
#define SWORD_PERCEPTION_PERCEPTION_RECO_URBAN_BLOCK_H

#include "PerceptionWithLocation.h"
#include "PerceptionLocalisation.h"
#include <boost/shared_ptr.hpp>

class MIL_UrbanObject_ABC;
class TER_Localisation;

namespace sword
{
namespace perception
{
    class PerceptionObserver_ABC;

class PerceptionRecoUrbanBlockReco : public PerceptionLocalisation
{
public:
    explicit PerceptionRecoUrbanBlockReco( const wrapper::View& perception );

    bool IsInside( const MT_Vector2D& vPoint ) const;
    void GetAgentsInside( const wrapper::View&, Perception_ABC::T_AgentPtrVector& ) const;
    bool CanSeeIt( const wrapper::View& perceiver ) const;

private:
    PerceptionRecoUrbanBlockReco& operator = ( const PerceptionRecoUrbanBlockReco& );

private:
    const MIL_UrbanObject_ABC* pUrbanBlock_;
    const TER_Localisation* localisation_;
};

// =============================================================================
// @class  PerceptionRecoUrbanBlock
// Created: MGD 2010-02-11
// =============================================================================
class PerceptionRecoUrbanBlock : public PerceptionWithLocation< PerceptionRecoUrbanBlockReco >
{
public:
             PerceptionRecoUrbanBlock( const wrapper::View& model, const wrapper::View& entity, PerceptionObserver_ABC& observer );
    virtual ~PerceptionRecoUrbanBlock();

    //! @name Execution
    //@{
    virtual const PerceptionLevel& Compute( const wrapper::View& perceiver, const SurfacesAgent_ABC& surfaces, const MT_Vector2D& vPoint ) const;

    virtual void                   Execute( const wrapper::View& model, const wrapper::View& perceiver, const SurfacesAgent_ABC& surfaces, const T_AgentPtrVector& perceivableAgents );
    virtual const PerceptionLevel& Compute( const wrapper::View& perceiver, const SurfacesAgent_ABC& surfaces, const wrapper::View& target ) const;
    //@}

private:
    //! @name Helpers
    //@{
    void AddLocalisation( const std::string& /*key*/, const wrapper::View& perception );
    //@}

private:
    //! @name Member data
    //@{
    PerceptionObserver_ABC& observer_;
    //@}
};

}
}

#endif // SWORD_PERCEPTION_PERCEPTION_RECO_URBAN_BLOCK_H
