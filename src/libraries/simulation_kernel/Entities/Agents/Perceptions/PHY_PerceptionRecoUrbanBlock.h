// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PHY_PerceptionRecoUrbanBlock_h_
#define __PHY_PerceptionRecoUrbanBlock_h_

#include "PHY_PerceptionWithLocation.h"
#include "PHY_PerceptionLocalisation.h"
#include <boost/shared_ptr.hpp>

class MIL_UrbanObject_ABC;
class DEC_Knowledge_Urban;

class PHY_PerceptionRecoUrbanBlockReco : public PHY_PerceptionLocalisation
{
public:
    PHY_PerceptionRecoUrbanBlockReco( const MIL_UrbanObject_ABC* pUrbanBlock, const boost::shared_ptr< DEC_Knowledge_Urban > pKnowledgeUrbanBlock );

    bool IsInside( const PHY_RoleInterface_Perceiver& perceiver, const MT_Vector2D& vPoint ) const;
    void GetAgentsInside( const PHY_RoleInterface_Perceiver& perceiver, TER_Agent_ABC::T_AgentPtrVector& ) const;
    bool CanSeeIt() const;
    const MIL_UrbanObject_ABC* GeturbanBlock() const;

private:
    PHY_PerceptionRecoUrbanBlockReco& operator = ( const PHY_PerceptionRecoUrbanBlockReco& );

private:
    const MIL_UrbanObject_ABC* pUrbanBlock_;
    const boost::shared_ptr< DEC_Knowledge_Urban > pKnowledgeUrbanBlock_;
    TER_Localisation localisation_;
};

// =============================================================================
// @class  PHY_PerceptionRecoUrbanBlock
// Created: MGD 2010-02-11
// =============================================================================
class PHY_PerceptionRecoUrbanBlock : public PHY_PerceptionWithLocation< PHY_PerceptionRecoUrbanBlockReco >
{
public:
    explicit PHY_PerceptionRecoUrbanBlock( PHY_RoleInterface_Perceiver& perceiver );
    virtual ~PHY_PerceptionRecoUrbanBlock();

    //! @name Add/Remove Points
    //@{
    int AddUrbanBlock( const MIL_UrbanObject_ABC* urbanBlock );
    void RemoveUrbanBlock( int );

    bool HasLocalisationToHandle() const;
    bool IsReconnoitering( const MIL_UrbanObject_ABC* urbanBlock ) const;
    //@}

    //! @name Execution
    //@{
    virtual const PHY_PerceptionLevel& Compute( const MT_Vector2D& vPoint ) const;

    virtual void Execute( const TER_Agent_ABC::T_AgentPtrVector& perceivableAgents );
    virtual const PHY_PerceptionLevel& Compute( const MIL_Agent_ABC& agent ) const;
    //@}
};

#endif // __PHY_PerceptionRecoUrbanBlock_h_
