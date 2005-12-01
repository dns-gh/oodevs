// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_KS_AgentQuerier.h $
// $Author: Nld $
// $Modtime: 11/05/05 15:58 $
// $Revision: 8 $
// $Workfile: DEC_KS_AgentQuerier.h $
//
// *****************************************************************************

#ifndef __DEC_KS_AgentQuerier_h_
#define __DEC_KS_AgentQuerier_h_

#include "MIL.h"

#include "DEC_Knowledge_Def.h"
#include "DEC_KnowledgeSource_ABC.h"

class DEC_KnowledgeBlackBoard;
class DEC_Knowledge_RapForLocal;
class DEC_Knowledge_RapForGlobal;
class DEC_Knowledge_Object;
class DEC_Knowledge_Agent;
class DEC_Knowledge_Population;
class MIL_RealObject_ABC;
class MIL_AgentPion;
class MIL_Agent_ABC;
class MIL_RealObjectTypeFilter;
class MIL_PopulationConcentration;

// =============================================================================
/** @class  DEC_KS_AgentQuerier
    @brief  This knowledge source (KS) is an 'OUT', which is used as a black board
            interrogator from the outside
*/
// Created: NLD 2004-03-11
// =============================================================================
class DEC_KS_AgentQuerier : public DEC_KnowledgeSource_ABC
{
    MT_COPYNOTALLOWED( DEC_KS_AgentQuerier ) 
public:
    //! @name Constructors/Destructor
    //@{
             DEC_KS_AgentQuerier( DEC_KnowledgeBlackBoard& blackBoard, const MIL_AgentPion& agent );
    virtual ~DEC_KS_AgentQuerier();
    //@}

    //! @name Agent level
    //@{
    bool IsKnown     ( const MIL_Agent_ABC& agent ) const;
    bool IsIdentified( const MIL_Agent_ABC& agent ) const;
    bool IsPerceived ( const MIL_Agent_ABC& agent ) const;
    bool WasPerceived( const MIL_Agent_ABC& agent ) const;

    bool IsKnown     ( const MIL_RealObject_ABC& object ) const;
    bool IsIdentified( const MIL_RealObject_ABC& object ) const;
    bool IsPerceived ( const MIL_RealObject_ABC& object ) const;

    bool IsIdentified( const MIL_PopulationConcentration& concentration ) const;
    

    void                  GetPerceivedAgents           ( T_KnowledgeAgentDiaIDVector&          container ) const;
    void                  GetAgentsAttacking           ( T_KnowledgeAgentDiaIDVector&          container ) const;
    void                  GetPotentiallyDangerousAgents( T_KnowledgeAgentDiaIDVector&          container ) const;
    void                  GetMilitiaAgents             ( T_KnowledgeAgentDiaIDVector&          container ) const;
    void                  GetRefugeeAgents             ( T_KnowledgeAgentDiaIDVector&          container ) const;
    void                  GetSurrenderedAgents         ( T_KnowledgeAgentDiaIDVector&          container ) const;
    void                  GetEnemies                   ( T_KnowledgeAgentVector&               container ) const;
    void                  GetFriendsInZone             ( T_KnowledgeAgentDiaIDVector&          container, const TER_Localisation& zone ) const;

    void                  GetObjects                   ( T_KnowledgeObjectVector&              container ) const;
    void                  GetObjects                   ( T_KnowledgeObjectVector&              container, const MIL_RealObjectTypeFilter& filter ) const;
    void                  GetObjects                   ( T_KnowledgeObjectDiaIDVector&         container, const MIL_RealObjectTypeFilter& filter ) const;
    void                  GetObjectsColliding          ( T_KnowledgeObjectCollisionVector&     container ) const;
    void                  GetObjectsColliding          ( T_KnowledgeObjectDiaIDVector&         container ) const;
    DEC_Knowledge_Object* GetClosestObject             ( const MT_Vector2D& vPos, const MIL_RealObjectTypeFilter& filter  ) const;

    void                  GetPopulationsColliding      ( T_KnowledgePopulationCollisionVector& container ) const;
    void                  GetPopulationsColliding      ( T_KnowledgePopulationDiaIDVector&     container ) const;
    void                  GetPopulationsAttacking      ( T_KnowledgePopulationDiaIDVector&     container ) const;
    
    DEC_Knowledge_RapForLocal&  GetKnowledgeRapForLocal () const;
    MT_Float                    GetRapForLocalValue     () const;
    //@}

    //! @name Knowledge group level
    //@{
    DEC_Knowledge_Agent*      GetKnowledgeAgentFromID     ( uint nID ) const;
    DEC_Knowledge_Population* GetKnowledgePopulationFromID( uint nID ) const;
    //@}

    //! @name Team level
    //@{
    DEC_Knowledge_Object* GetKnowledgeObjectFromID( uint nID ) const;
    DEC_Knowledge_Object* GetKnowledgeObject      ( MIL_RealObject_ABC& object ) const;
    //@}

private:
    const MIL_AgentPion* pPion_;
};

#include "DEC_KS_AgentQuerier.inl"

#endif // __DEC_KS_AgentQuerier_h_
