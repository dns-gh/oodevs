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

class MIL_Agent_ABC;
class DEC_BlackBoard_CanContainKnowledgeAgentPerception;
class DEC_BlackBoard_CanContainKnowledgeObjectCollision;
class DEC_BlackBoard_CanContainKnowledgeObjectPerception;
class DEC_BlackBoard_CanContainKnowledgePopulationPerception;
class DEC_BlackBoard_CanContainKnowledgePopulationCollision;
class DEC_BlackBoard_CanContainKnowledgeUrbanPerception;
class DEC_Knowledge_RapForLocal;
class DEC_KS_ObjectInteraction;
class DEC_KS_PopulationInteraction;
class DEC_KS_DirectFire;
class DEC_KS_IndirectFire;
class DEC_KS_Perception;
class MIL_Object_ABC;
class MIL_PopulationConcentration;
class MIL_Population;
class TER_Localisation;
class MIL_UrbanObject_ABC;

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
    explicit DEC_KnowledgeBlackBoard_AgentPion( MIL_Agent_ABC& pion );
             DEC_KnowledgeBlackBoard_AgentPion();
    virtual ~DEC_KnowledgeBlackBoard_AgentPion();
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}

    //! @name Accessors
    //@{
    MIL_Agent_ABC& GetPion() const;

    DEC_KS_DirectFire& GetKsDirectFire() const;
    DEC_KS_IndirectFire& GetKsIndirectFire() const;
    DEC_KS_Perception& GetKsPerception() const;
    DEC_KS_ObjectInteraction& GetKsObjectInteraction() const;
    DEC_KS_PopulationInteraction& GetKsPopulationInteraction() const;
    DEC_BlackBoard_CanContainKnowledgeAgentPerception& GetKnowledgeAgentPerceptionContainer() const;
    DEC_BlackBoard_CanContainKnowledgeObjectCollision& GetKnowledgeObjectCollisionContainer() const;
    DEC_BlackBoard_CanContainKnowledgeObjectPerception& GetKnowledgeObjectPerceptionContainer() const;
    DEC_BlackBoard_CanContainKnowledgePopulationPerception& GetKnowledgePopulationPerceptionContainer() const;
    DEC_BlackBoard_CanContainKnowledgePopulationCollision& GetKnowledgePopulationCollisionContainer() const;
    DEC_BlackBoard_CanContainKnowledgeUrbanPerception& GetKnowledgeUrbanPerceptionContainer() const;
    //@}

    //! @name Network
    //@{
    virtual void SendFullState( unsigned int nCtx ) const;
    virtual void SendChangedState() const;
    //@}

    //! @name Operations
    //@{
    virtual void CleanDeletedAgentKnowledges();
    //@}

    //! @name tools::Resolver
    //@{
    virtual boost::shared_ptr< DEC_Knowledge_Agent > ResolveKnowledgeAgent ( const sword::Id&  asn ) const;
    virtual boost::shared_ptr< DEC_Knowledge_Agent > ResolveKnowledgeAgent ( const MIL_Agent_ABC& agent ) const;
    virtual boost::shared_ptr< DEC_Knowledge_Agent > ResolveKnowledgeAgent ( unsigned int nID ) const;
    virtual boost::shared_ptr< DEC_Knowledge_Object > ResolveKnowledgeObject( const sword::Id& asn ) const;
    virtual boost::shared_ptr< DEC_Knowledge_Object > ResolveKnowledgeObject( const MIL_Object_ABC& object ) const;
    virtual boost::shared_ptr< DEC_Knowledge_Object > ResolveKnowledgeObject( unsigned int nID ) const;
    virtual boost::shared_ptr< DEC_Knowledge_Object > ResolveKnowledgeObjectByObjectID( unsigned int nID ) const;
    virtual boost::shared_ptr< DEC_Knowledge_Population > ResolveKnowledgePopulation( const sword::Id& asn ) const;
    virtual boost::shared_ptr< DEC_Knowledge_Population > ResolveKnowledgePopulation( const MIL_Population& population ) const;
    virtual boost::shared_ptr< DEC_Knowledge_Population > ResolveKnowledgePopulation( unsigned int nID ) const;
    //@}

    //! @name Queries
    //@{
    // Agents
    bool IsIdentified( const MIL_Agent_ABC& agent ) const;
    bool IsPerceived( const MIL_Agent_ABC& agent ) const;
    bool WasPerceived( const MIL_Agent_ABC& agent ) const;
    void GetLivingEnemiesPerceived( T_ConstKnowledgeAgentVector& container ) const;
    void GetLivingEnemiesPerceivedInZone( T_ConstKnowledgeAgentVector& container, const TER_Localisation& zone ) const;
    const T_ConstKnowledgeAgentVector& GetDangerousEnemies() const;
    void GetAgentsAttacking( T_ConstKnowledgeAgentVector& container ) const;
    double GetRapForLocalValue() const;

    // Objects
    bool IsIdentified( const MIL_Object_ABC& object ) const;
    bool IsPerceived( const MIL_Object_ABC& object ) const;
    void GetObjectsColliding( T_KnowledgeObjectCollisionVector& container ) const;
    void GetObjectsColliding( T_KnowledgeObjectDiaIDVector& container ) const;

    // Population
    bool IsIdentified( const MIL_PopulationConcentration& concentration ) const;
    bool IsPerceived( const MIL_Population& population ) const;
    void GetPopulationsColliding( T_KnowledgePopulationCollisionVector& container ) const;
    void GetPopulationsColliding( T_KnowledgePopulationDiaIDVector& container ) const;
    void GetPopulationsAttacking( T_KnowledgePopulationDiaIDVector& container ) const;
    bool HasCollision() const;
    bool HasCollisionWithCrowd( const MIL_Population& population ) const;
    bool IsPopulationAttacking( const DEC_Knowledge_Population& knowledge ) const;

    //Urban
    bool IsIdentified( const MIL_UrbanObject_ABC& object ) const;
    //@}

private:
    MIL_Agent_ABC* pPion_;

    // Containers
    DEC_BlackBoard_CanContainKnowledgeAgentPerception* pKnowledgeAgentPerceptionContainer_;
    DEC_BlackBoard_CanContainKnowledgeObjectCollision* pKnowledgeObjectCollisionContainer_;
    DEC_BlackBoard_CanContainKnowledgeObjectPerception* pKnowledgeObjectPerceptionContainer_;
    DEC_BlackBoard_CanContainKnowledgePopulationPerception* pKnowledgePopulationPerceptionContainer_;
    DEC_BlackBoard_CanContainKnowledgePopulationCollision* pKnowledgePopulationCollisionContainer_;
    DEC_BlackBoard_CanContainKnowledgeUrbanPerception* pKnowledgeUrbanPerceptionContainer_;
    DEC_Knowledge_RapForLocal* pKnowledgeRapForLocal_;
    // Knowledge sources
    DEC_KS_ObjectInteraction* pKsObjectInteraction_;
    DEC_KS_PopulationInteraction* pKsPopulationInteraction_;
    DEC_KS_DirectFire* pKsDirectFire_;
    DEC_KS_IndirectFire* pKsIndirectFire_;
    DEC_KS_Perception* pKsPerception_;
};

BOOST_CLASS_EXPORT_KEY( DEC_KnowledgeBlackBoard_AgentPion )

#endif // __DEC_KnowledgeBlackBoard_AgentPion_h_
