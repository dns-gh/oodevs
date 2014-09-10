// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_KS_DirectFire.h $
// $Author: Jvt $
// $Modtime: 5/04/05 13:34 $
// $Revision: 5 $
// $Workfile: DEC_KS_DirectFire.h $
//
// *****************************************************************************

#ifndef __DEC_KS_Fire_h_
#define __DEC_KS_Fire_h_

#include "DEC_KnowledgeSource_ABC.h"
#include <tools/Set.h>

class DEC_KnowledgeBlackBoard_AgentPion;
class MIL_Agent_ABC;
class MIL_Population;

// =============================================================================
/** @class  DEC_KS_DirectFire
    @brief  This knowledge source (KS) is an 'IN/OUT', which is used to manage
            the direct fires relatives knowledges
*/
// Created: NLD 2004-03-11
// =============================================================================
class DEC_KS_DirectFire : public DEC_KnowledgeSource_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit DEC_KS_DirectFire( DEC_KnowledgeBlackBoard_AgentPion& blackBoard );
             DEC_KS_DirectFire();
    virtual ~DEC_KS_DirectFire();
    //@}

    //! @name Operations
    //@{
    virtual void Prepare();
    virtual void Clean();
    virtual void CleanDeletedAgentKnowledges();
    virtual void Talk( int currentTimeStep );
    void Clear();

    bool IsAttacked() const;
    //@}

    //! @name Events
    //@{
    void NotifyAttackedBy( const MIL_Agent_ABC&  attacker );
    void NotifyAttackedBy( const MIL_Population& attacker );
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}

private:
    DEC_KnowledgeBlackBoard_AgentPion* pBlackBoard_;
    tools::Set< const MIL_Agent_ABC* > pionsAttacking_;
    tools::Set< const MIL_Population* > populationsAttacking_;
};

BOOST_CLASS_EXPORT_KEY( DEC_KS_DirectFire )

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_AgentPion::serialize
// Created: NLD 2006-04-12
// -----------------------------------------------------------------------------
template< typename Archive >
void DEC_KS_DirectFire::serialize( Archive& archive, const unsigned int )
{
    archive & boost::serialization::base_object< DEC_KnowledgeSource_ABC >( *this );
    archive & pBlackBoard_;
}

#endif // __DEC_KS_Fire_h_
