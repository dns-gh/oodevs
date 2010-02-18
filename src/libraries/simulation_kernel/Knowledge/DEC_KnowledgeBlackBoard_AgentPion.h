// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_KnowledgeBlackBoard_AgentPion.h $
// $Author: Jvt $
// $Modtime: 6/04/05 18:47 $
// $Revision: 3 $
// $Workfile: DEC_KnowledgeBlackBoard_AgentPion.h $
//
// *****************************************************************************

#ifndef __DEC_KnowledgeBlackBoard_AgentPion_h_
#define __DEC_KnowledgeBlackBoard_AgentPion_h_

#include "DEC_KnowledgeBlackBoard_ABC.h"

class MIL_AgentPion;
class DEC_BlackBoard_CanContainKnowledgeAgentPerception;
class DEC_BlackBoard_CanContainKnowledgeObjectCollision;
class DEC_BlackBoard_CanContainKnowledgeObjectPerception;
class DEC_BlackBoard_CanContainKnowledgePopulationPerception;
class DEC_BlackBoard_CanContainKnowledgePopulationCollision;
class DEC_BlackBoard_CanContainKnowledgeUrbanPerception;
class DEC_Knowledge_RapForLocal;
class DEC_KS_ObjectInteraction; 
class DEC_KS_PopulationInteraction;
class DEC_KS_Fire;            
class DEC_KS_Perception;
class MIL_Object_ABC;
class MIL_PopulationConcentration;
class MIL_Population;

namespace urban
{
    class TerrainObject_ABC;
}

// =============================================================================
/** @class  DEC_KnowledgeBlackBoard_AgentPion
    @brief  The base class representing all the knowledges of an entity
*/
// Created: NLD 2004-03-11
// =============================================================================
class DEC_KnowledgeBlackBoard_AgentPion : public DEC_KnowledgeBlackBoard_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit DEC_KnowledgeBlackBoard_AgentPion( MIL_AgentPion& pion );
             DEC_KnowledgeBlackBoard_AgentPion();
    virtual ~DEC_KnowledgeBlackBoard_AgentPion();
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    //@}

    //! @name Accessors
    //@{
    MIL_AgentPion& GetPion() const;

    DEC_KS_Fire&                                            GetKsFire                 () const;
    DEC_KS_Perception&                                      GetKsPerception           () const;
    DEC_KS_ObjectInteraction&                               GetKsObjectInteraction    () const;
    DEC_KS_PopulationInteraction&                           GetKsPopulationInteraction() const;

    DEC_BlackBoard_CanContainKnowledgeAgentPerception&      GetKnowledgeAgentPerceptionContainer     () const;
    DEC_BlackBoard_CanContainKnowledgeObjectCollision&      GetKnowledgeObjectCollisionContainer     () const;
    DEC_BlackBoard_CanContainKnowledgeObjectPerception&     GetKnowledgeObjectPerceptionContainer    () const;
    DEC_BlackBoard_CanContainKnowledgePopulationPerception& GetKnowledgePopulationPerceptionContainer() const;
    DEC_BlackBoard_CanContainKnowledgePopulationCollision&  GetKnowledgePopulationCollisionContainer () const;
    DEC_BlackBoard_CanContainKnowledgeUrbanPerception&      GetKnowledgeUrbanPerceptionContainer() const;
    //@}

    //! @name Network
    //@{
    virtual void SendFullState   () const;
    virtual void SendChangedState() const;
    //@}

    //! @name tools::Resolver
    //@{
    virtual boost::shared_ptr< DEC_Knowledge_Agent > ResolveKnowledgeAgent ( const ASN1T_UnitKnowledge&  asn ) const;
    virtual boost::shared_ptr< DEC_Knowledge_Agent > ResolveKnowledgeAgent (       uint                   nID ) const;

    virtual boost::shared_ptr< DEC_Knowledge_Object > ResolveKnowledgeObject( const ASN1T_ObjectKnowledge& asn ) const;
    virtual boost::shared_ptr< DEC_Knowledge_Object > ResolveKnowledgeObject(       uint                   nID ) const;

    virtual DEC_Knowledge_Population* ResolveKnowledgePopulation( const ASN1T_PopulationKnowledge& asn ) const;
    virtual DEC_Knowledge_Population* ResolveKnowledgePopulation(       uint                       nID ) const;

    virtual boost::shared_ptr< DEC_Knowledge_Urban > ResolveKnowledgeUrban( const ASN1T_UrbanKnowledge& asn ) const;
    virtual boost::shared_ptr< DEC_Knowledge_Urban > ResolveKnowledgeUrban(       uint                   nID ) const;

    //@}

    //! @name Queries
    //@{
    // Agents
          bool                         IsIdentified                   ( const MIL_Agent_ABC& agent ) const;
          bool                         IsPerceived                    ( const MIL_Agent_ABC& agent ) const;
          bool                         WasPerceived                   ( const MIL_Agent_ABC& agent ) const;
          void                         GetLivingEnemiesPerceived      ( T_ConstKnowledgeAgentVector& container ) const;
          void                         GetLivingEnemiesPerceivedInZone( T_ConstKnowledgeAgentVector& container, const TER_Localisation& zone ) const;
    const T_ConstKnowledgeAgentVector& GetDangerousEnemies            () const;
          void                         GetDangerousEnemiesInZone      ( T_ConstKnowledgeAgentVector& container, const TER_Localisation& zone ) const;
          void                         GetAgentsAttacking             ( T_ConstKnowledgeAgentVector& container ) const;
          MT_Float                     GetRapForLocalValue            () const;

    // Objects
    bool IsIdentified       ( const MIL_Object_ABC& object ) const;
    bool IsPerceived        ( const MIL_Object_ABC& object ) const;
    void GetObjectsColliding( T_KnowledgeObjectCollisionVector& container ) const;
    void GetObjectsColliding( T_KnowledgeObjectDiaIDVector&     container ) const;

    // Population
    bool IsIdentified           ( const MIL_PopulationConcentration& concentration ) const;
    bool IsPerceived            ( const MIL_Population&              population    ) const;    
    void GetPopulationsColliding( T_KnowledgePopulationCollisionVector& container ) const;
    void GetPopulationsColliding( T_KnowledgePopulationDiaIDVector&     container ) const;
    void GetPopulationsAttacking( T_KnowledgePopulationDiaIDVector&     container ) const;

    //Urban
    bool IsIdentified( const urban::TerrainObject_ABC& object ) const;
    //@}   

private:
    MIL_AgentPion* pPion_;

    // Containers
    DEC_BlackBoard_CanContainKnowledgeAgentPerception*      pKnowledgeAgentPerceptionContainer_;
    DEC_BlackBoard_CanContainKnowledgeObjectCollision*      pKnowledgeObjectCollisionContainer_;
    DEC_BlackBoard_CanContainKnowledgeObjectPerception*     pKnowledgeObjectPerceptionContainer_;
    DEC_BlackBoard_CanContainKnowledgePopulationPerception* pKnowledgePopulationPerceptionContainer_;
    DEC_BlackBoard_CanContainKnowledgePopulationCollision*  pKnowledgePopulationCollisionContainer_;
    DEC_BlackBoard_CanContainKnowledgeUrbanPerception*      pKnowledgeUrbanPerceptionContainer_;
    DEC_Knowledge_RapForLocal*                              pKnowledgeRapForLocal_;

    // Knowledge sources
    DEC_KS_ObjectInteraction*     pKsObjectInteraction_;
    DEC_KS_PopulationInteraction* pKsPopulationInteraction_;
    DEC_KS_Fire*                  pKsFire_;
    DEC_KS_Perception*            pKsPerception_;
};

BOOST_CLASS_EXPORT_KEY( DEC_KnowledgeBlackBoard_AgentPion )

#endif // __DEC_KnowledgeBlackBoard_AgentPion_h_
