// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_KS_KnowledgeGroupQuerier.h $
// $Author: Nld $
// $Modtime: 11/05/05 15:58 $
// $Revision: 6 $
// $Workfile: DEC_KS_KnowledgeGroupQuerier.h $
//
// *****************************************************************************

#ifndef __DEC_KS_KnowledgeGroupQuerier_h_
#define __DEC_KS_KnowledgeGroupQuerier_h_

#include "MIL.h"

#include "DEC_Knowledge_Def.h"
#include "DEC_KnowledgeSource_ABC.h"
#include "DEC_KnowledgeBlackBoard.h"

class DEC_Knowledge_Object;
class DEC_Knowledge_Agent;
class PHY_UnitViewable_ABC;
class MIL_RealObject_ABC;
class MIL_KnowledgeGroup;
class MIL_Agent_ABC;

// =============================================================================
/** @class  DEC_KS_KnowledgeGroupQuerier
    @brief  This knowledge source (KS) is an 'OUT', which is used as a black board
            interrogator from the outside
*/
// Created: NLD 2004-03-11
// =============================================================================
class DEC_KS_KnowledgeGroupQuerier : public DEC_KnowledgeSource_ABC
{
    MT_COPYNOTALLOWED( DEC_KS_KnowledgeGroupQuerier ) 
public:
    //! @name Constructors/Destructor
    //@{
             DEC_KS_KnowledgeGroupQuerier( DEC_KnowledgeBlackBoard& blackBoard, const MIL_KnowledgeGroup& knowledgeGroup );
    virtual ~DEC_KS_KnowledgeGroupQuerier();
    //@}

    //! @name Queries / Operations
    //@{
    bool                  IsKnown                      ( const MIL_Agent_ABC& agent ) const;
    DEC_Knowledge_Agent*  GetKnowledgeAgent            ( const DEC_Knowledge_AgentPerception& perception ) const;
    DEC_Knowledge_Agent*  GetKnowledgeAgentFromMosID   ( uint nID ) const;
    DEC_Knowledge_Agent*  GetKnowledgeAgentFromDiaID   ( uint nID ) const;

    void                  GetPotentiallyDangerousAgents( T_KnowledgeAgentDiaIDVector& container ) const;
    void                  GetMilitiaAgents             ( T_KnowledgeAgentDiaIDVector& container ) const;
    void                  GetRefugeeAgents             ( T_KnowledgeAgentDiaIDVector& container ) const;
    void                  GetSurrenderedAgents         ( T_KnowledgeAgentDiaIDVector& container ) const;
    void                  GetEnemies                   ( T_KnowledgeAgentVector&      container ) const;
    void                  GetFriends                   ( T_KnowledgeAgentVector&      container ) const;
    void                  GetFriendsInZone             ( T_KnowledgeAgentDiaIDVector& container, const TER_Localisation& zone ) const;
    //@}

    //! @name 
    //@{
    DEC_Knowledge_Object* GetKnowledgeObjectFromDiaID( uint nID ) const;
    DEC_Knowledge_Object* GetKnowledgeObjectFromMosID( uint nID ) const;
    DEC_Knowledge_Object* GetKnowledgeObject         ( MIL_RealObject_ABC& object ) const;
    //@}

private:
    const MIL_KnowledgeGroup* pKnowledgeGroup_;
};

#include "DEC_KS_KnowledgeGroupQuerier.inl"

#endif // __DEC_KS_KnowledgeGroupQuerier_h_
