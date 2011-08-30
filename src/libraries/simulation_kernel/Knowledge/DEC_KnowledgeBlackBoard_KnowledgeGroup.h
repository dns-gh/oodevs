// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_KnowledgeBlackBoard_KnowledgeGroup.h $
// $Author: Jvt $
// $Modtime: 6/04/05 18:47 $
// $Revision: 3 $
// $Workfile: DEC_KnowledgeBlackBoard_KnowledgeGroup.h $
//
// *****************************************************************************

#ifndef __DEC_KnowledgeBlackBoard_KnowledgeGroup_h_
#define __DEC_KnowledgeBlackBoard_KnowledgeGroup_h_

#include "DEC_KnowledgeBlackBoard_ABC.h"

namespace sword
{
    class ObjectKnowledgeId;
    class CrowdKnowledgeId;
    class UnitKnowledgeId;
}

class MIL_KnowledgeGroup;
class DEC_BlackBoard_CanContainKnowledgeAgent;
class DEC_BlackBoard_CanContainKnowledgeObject;
class DEC_BlackBoard_CanContainKnowledgePopulation;
class DEC_KS_KnowledgeSynthetizer;
class DEC_KS_Sharing;
class DEC_Knowledge_Object;
class DEC_Knowledge_PopulationCollision;
class DEC_Knowledge_PopulationPerception;
class DEC_Knowledge_AgentPerception;
class MIL_Agent_ABC;
class MIL_Army_ABC;
class MIL_Object_ABC;
class MIL_Population;
class TER_Localisation;
class TER_Polygon;
class KnowledgesVisitor_ABC;

// =============================================================================
/** @class  DEC_KnowledgeBlackBoard_KnowledgeGroup
    @brief  The base class representing all the knowledges of an entity
*/
// Created: NLD 2004-03-11
// =============================================================================
class DEC_KnowledgeBlackBoard_KnowledgeGroup : public DEC_KnowledgeBlackBoard_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit DEC_KnowledgeBlackBoard_KnowledgeGroup( MIL_KnowledgeGroup& knowledgeGroup );
             DEC_KnowledgeBlackBoard_KnowledgeGroup();
    virtual ~DEC_KnowledgeBlackBoard_KnowledgeGroup();
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}

    //! @name Accessors
    //@{
    MIL_KnowledgeGroup& GetKnowledgeGroup() const;
    DEC_KS_Sharing& GetKsSharing() const;
    DEC_BlackBoard_CanContainKnowledgeAgent& GetKnowledgeAgentContainer() const;
    DEC_BlackBoard_CanContainKnowledgePopulation& GetKnowledgePopulationContainer() const;
    DEC_BlackBoard_CanContainKnowledgeObject* GetKnowledgeObjectContainer() const;
    void UpdateKnowledgeObjectContainer();
    void Accept( KnowledgesVisitor_ABC& visitor ) const;
    //@}

    //! @name Operations
    //@{
    virtual void Update( int currentTimeStep );
    void Jam();
    //@}

    //! @name Network
    //@{
    virtual void SendFullState( unsigned int nCtx ) const;
    virtual void SendChangedState() const;
    virtual void SendObjectChangedState() const;
    //@}

    //! @name tools::Resolver
    //@{
    virtual boost::shared_ptr< DEC_Knowledge_Agent > ResolveKnowledgeAgent( const sword::UnitKnowledgeId& asn ) const;
    virtual boost::shared_ptr< DEC_Knowledge_Agent > ResolveKnowledgeAgent( const MIL_Agent_ABC& agent ) const;
    virtual boost::shared_ptr< DEC_Knowledge_Agent > ResolveKnowledgeAgent( unsigned int nID ) const;

    virtual boost::shared_ptr< DEC_Knowledge_Object > ResolveKnowledgeObject( const sword::ObjectKnowledgeId& asn ) const;
    virtual boost::shared_ptr< DEC_Knowledge_Object > ResolveKnowledgeObject( const MIL_Object_ABC& object ) const;
    virtual boost::shared_ptr< DEC_Knowledge_Object > ResolveKnowledgeObject( unsigned int nID ) const;

    virtual DEC_Knowledge_Population* ResolveKnowledgePopulation( const sword::CrowdKnowledgeId& asn ) const;
    virtual DEC_Knowledge_Population* ResolveKnowledgePopulation( const MIL_Population& population ) const;
    virtual DEC_Knowledge_Population* ResolveKnowledgePopulation( unsigned int nID ) const;
    //@}

    //! @name Queries
    //@{
    // Knowledge agents
    DEC_Knowledge_Agent& CreateKnowledgeAgent( const MIL_KnowledgeGroup& knowledgeGroup, MIL_Agent_ABC& agentKnown );
    DEC_Knowledge_Population& CreateKnowledgePopulation( const MIL_KnowledgeGroup& knowledgeGroup, MIL_Population& perceived );
    bool IsKnown( const MIL_Agent_ABC& agent ) const;
    boost::shared_ptr< DEC_Knowledge_Agent > GetKnowledgeAgent( const DEC_Knowledge_AgentPerception& perception ) const;
    boost::shared_ptr< DEC_Knowledge_Agent > GetKnowledgeAgent( const MIL_Agent_ABC& agent ) const;
    boost::shared_ptr< DEC_Knowledge_Agent > GetKnowledgeAgentFromID( unsigned int nID ) const;
    void GetDetectedAgentsInZone( T_ConstKnowledgeAgentVector& container, const TER_Polygon& zone ) const;
    void GetDetectedAgentsInZone( T_ConstKnowledgeAgentVector& container, const TER_Localisation& zone ) const;
    void GetEnemyAgentsInZone( T_ConstKnowledgeAgentVector& container, const TER_Localisation& zone ) const;

    const T_KnowledgeAgentVector& GetFriends() const;
    void GetFriendsInZone( T_ConstKnowledgeAgentVector& container, const TER_Localisation& zone ) const;
    const T_KnowledgeAgentVector& GetEnemies() const;
    void GetLivingEnemiesInZone( T_ConstKnowledgeAgentVector& container, const TER_Polygon& zone ) const;
    void GetLivingEnemiesInZone( T_ConstKnowledgeAgentVector& container, const TER_Localisation& zone ) const;
    void GetLivingEnemiesInCircle( T_ConstKnowledgeAgentVector& container, const MT_Vector2D& center, double rRadius ) const;
    bool EnemyPresenceInCircle( const MT_Vector2D& center, double rRadius ) const;

    void GetRefugeesInCircle( T_ConstKnowledgeAgentVector& container, const MT_Vector2D& center, double rRadius ) const;
    void GetTerroristsInCircle( T_ConstKnowledgeAgentVector& container, const MT_Vector2D& center, double rRadius ) const;
    void GetSurrenderedAgentsInCircle( T_ConstKnowledgeAgentVector& container, const MT_Vector2D& center, double rRadius ) const;

    // Knowledge populations
    DEC_Knowledge_Population* GetKnowledgePopulation( const DEC_Knowledge_PopulationCollision& collision ) const;
    DEC_Knowledge_Population* GetKnowledgePopulation( const DEC_Knowledge_PopulationPerception& perception ) const;
    DEC_Knowledge_Population* GetKnowledgePopulation( const MIL_Population& population ) const;
    DEC_Knowledge_Population* GetKnowledgePopulationFromID( unsigned int nID ) const;
    void GetPopulations( T_KnowledgePopulationDiaIDVector& container ) const;
    void GetPopulations( T_KnowledgePopulationVector& container ) const;
     //@}

    //! @name Tools
    //@{
    void TranslateKnowledges( const T_ConstKnowledgeAgentVector& sourceKnowledges, const MIL_KnowledgeGroup& sourceKnowledgeGroup, T_ConstKnowledgeAgentVector& translatedKnowledges ) const;
    boost::shared_ptr< DEC_Knowledge_Object > CreateKnowledgeObject( const MIL_Army_ABC& teamKnowing, MIL_Object_ABC& objectKnown );
    void ApplyOnKnowledgesPerception( int currentTimeStep );

    template < class UnaryFunction >
    void ApplyOnKnowledgesAgent( UnaryFunction& fct ) const
    {
        GetKnowledgeAgentContainer().ApplyOnKnowledgesAgent( fct );
    }

    template < class UnaryFunction >
    void ApplyOnKnowledgesPopulation( UnaryFunction& fct ) const
    {
        GetKnowledgePopulationContainer().ApplyOnKnowledgesPopulation( fct );
    }
    //@}

private:
    //! @name Member data
    //@{
    MIL_KnowledgeGroup* pKnowledgeGroup_;
    // Containers
    DEC_BlackBoard_CanContainKnowledgeAgent* pKnowledgeAgentContainer_;
    DEC_BlackBoard_CanContainKnowledgePopulation* pKnowledgePopulationContainer_;
    DEC_BlackBoard_CanContainKnowledgeObject* pKnowledgeObjectContainer_;
    // Knowledge sources
    DEC_KS_KnowledgeSynthetizer* pKsKnowledgeSynthetizer_;
    DEC_KS_Sharing* pKsSharing_;
    //@}

    //! @name Queries caches
    //@{
    unsigned int nLastCacheUpdateTick_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( DEC_KnowledgeBlackBoard_KnowledgeGroup )

#endif // __DEC_KnowledgeBlackBoard_KnowledgeGroup_h_
