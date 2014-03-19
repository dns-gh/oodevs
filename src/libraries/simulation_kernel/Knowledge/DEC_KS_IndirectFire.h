// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __DEC_KS_IndirectFire_h_
#define __DEC_KS_IndirectFire_h_

#include "DEC_KnowledgeSource_ABC.h"
#include <tools/Set.h>

class DEC_KnowledgeBlackBoard_AgentPion;
class MIL_Agent_ABC;

// =============================================================================
/** @class  DEC_KS_IndirectFire
    @brief  This knowledge source (KS) is an 'IN/OUT', which is used to manage
            the indirect fires relatives knowledges
*/
// Created: MCO 2014-03-14
// =============================================================================
class DEC_KS_IndirectFire : public DEC_KnowledgeSource_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit DEC_KS_IndirectFire( DEC_KnowledgeBlackBoard_AgentPion& blackBoard );
             DEC_KS_IndirectFire();
    virtual ~DEC_KS_IndirectFire();
    //@}

    //! @name Operations
    //@{
    virtual void Prepare();
    virtual void Clean();
    virtual void CleanDeletedAgentKnowledges();
    virtual void Talk( int currentTimeStep );
    //@}

    //! @name Events
    //@{
    void NotifyAttackedBy( const MIL_Agent_ABC&  attacker );
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}

private:
    DEC_KnowledgeBlackBoard_AgentPion* pBlackBoard_;
    tools::Set< const MIL_Agent_ABC* > pionsAttacking_;
};

BOOST_CLASS_EXPORT_KEY( DEC_KS_IndirectFire )

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_AgentPion::serialize
// Created: NLD 2006-04-12
// -----------------------------------------------------------------------------
template< typename Archive >
void DEC_KS_IndirectFire::serialize( Archive& archive, const unsigned int )
{
    archive & boost::serialization::base_object< DEC_KnowledgeSource_ABC >( *this )
            & pBlackBoard_;
}

#endif // __DEC_KS_IndirectFire_h_
