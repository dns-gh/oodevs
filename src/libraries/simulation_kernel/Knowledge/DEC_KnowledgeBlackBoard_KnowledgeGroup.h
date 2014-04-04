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

class MIL_KnowledgeGroup;
class DEC_BlackBoard_CanContainKnowledgeAgent;
class DEC_BlackBoard_CanContainKnowledgeObject;
class DEC_BlackBoard_CanContainKnowledgePopulation;
class DEC_KS_KnowledgeSynthetizer;
class DEC_KS_ObjectKnowledgeSynthetizer;
class DEC_KS_Sharing;
class DEC_Knowledge_Object;
class DEC_Knowledge_PopulationCollision;
class DEC_Knowledge_PopulationPerception;
class DEC_Knowledge_AgentPerception;
class MIL_Agent_ABC;
class MIL_Army_ABC;
class MIL_Object_ABC;
class MIL_ObjectFilter;
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
    explicit DEC_KnowledgeBlackBoard_KnowledgeGroup( MIL_KnowledgeGroup* group );
             DEC_KnowledgeBlackBoard_KnowledgeGroup();
    virtual ~DEC_KnowledgeBlackBoard_KnowledgeGroup();
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}

    //! @name Accessors
    //@{
    void SetKnowledgeGroup( MIL_KnowledgeGroup* group );
    boost::shared_ptr< MIL_KnowledgeGroup > GetKnowledgeGroup() const;
    DEC_KS_Sharing& GetKsSharing() const;
    DEC_BlackBoard_CanContainKnowledgeAgent& GetKnowledgeAgentContainer() const;
    DEC_BlackBoard_CanContainKnowledgePopulation& GetKnowledgePopulationContainer() const;
    DEC_BlackBoard_CanContainKnowledgeObject& GetKnowledgeObjectContainer() const;
    DEC_KS_ObjectKnowledgeSynthetizer& GetKsObjectKnowledgeSynthetizer() const;
    void Accept( KnowledgesVisitor_ABC& visitor ) const;
    //@}

    //! @name Operations
    //@{
    virtual void Update( int currentTimeStep );
    //@}

    //! @name Network
    //@{
    virtual void SendFullState( unsigned int nCtx ) const;
    virtual void SendChangedState() const;
    virtual void SendObjectChangedState() const;
    //@}

    //! @name tools::Resolver
    //@{
    virtual boost::shared_ptr< DEC_Knowledge_Agent > ResolveKnowledgeAgent( const sword::Id& asn ) const;
    virtual boost::shared_ptr< DEC_Knowledge_Agent > ResolveKnowledgeAgent( const MIL_Agent_ABC& agent ) const;
    virtual boost::shared_ptr< DEC_Knowledge_Agent > ResolveKnowledgeAgent( unsigned int nID ) const;

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
    // Knowledge agents
    DEC_Knowledge_Agent& CreateKnowledgeAgent( const MIL_KnowledgeGroup& group, const MIL_Agent_ABC& agentKnown );
    DEC_Knowledge_Population& CreateKnowledgePopulation( const MIL_KnowledgeGroup& group, MIL_Population& perceived );
    bool IsKnown( const MIL_Agent_ABC& agent ) const;
    boost::shared_ptr< DEC_Knowledge_Agent > GetKnowledgeAgent( const DEC_Knowledge_AgentPerception& perception ) const;
    boost::shared_ptr< DEC_Knowledge_Agent > GetKnowledgeAgent( const MIL_Agent_ABC& agent ) const;
    boost::shared_ptr< DEC_Knowledge_Agent > GetKnowledgeAgentFromID( unsigned int nID ) const;
	void GetAllAgentsInZone( T_ConstKnowledgeAgentVector& container, const TER_Localisation& zone ) const;
    void GetDetectedAgentsInZone( T_ConstKnowledgeAgentVector& container, const TER_Polygon& zone ) const;
    void GetDetectedAgentsInZone( T_ConstKnowledgeAgentVector& container, const TER_Localisation& zone ) const;
    void GetEnemyAgentsInZone( T_ConstKnowledgeAgentVector& container, const TER_Localisation& zone ) const;

    const T_KnowledgeAgentVector& GetFriends() const;
    void GetFriendsInZone( T_ConstKnowledgeAgentVector& container, const TER_Localisation& zone ) const;
    const T_KnowledgeAgentVector& GetEnemies() const;
    void GetLivingEnemiesInZone( T_ConstKnowledgeAgentVector& container, const TER_Polygon& zone ) const;
    void GetLivingEnemiesInZone( T_ConstKnowledgeAgentVector& container, const TER_Localisation& zone ) const;
    void GetWoundedUnitsInZone( T_ConstKnowledgeAgentVector& container, const TER_Localisation& zone ) const;
    void GetCiviliansInZone( T_ConstKnowledgeAgentVector& container, const TER_Localisation& zone ) const;
    void GetLivingEnemiesInCircle( T_ConstKnowledgeAgentVector& container, const MT_Vector2D& center, double rRadius ) const;
    void GetWoundedOrDeadUnitsInCircle( T_ConstKnowledgeAgentVector& container, const MT_Vector2D& center, double rRadius ) const;
    bool EnemyPresenceInCircle( const MT_Vector2D& center, double rRadius ) const;

    void GetRefugeesInCircle( T_ConstKnowledgeAgentVector& container, const MT_Vector2D& center, double rRadius ) const;
    void GetTerroristsInCircle( T_ConstKnowledgeAgentVector& container, const MT_Vector2D& center, double rRadius ) const;
    void GetSurrenderedAgentsInCircle( T_ConstKnowledgeAgentVector& container, const MT_Vector2D& center, double rRadius ) const;

    // Knowledge populations
    boost::shared_ptr< DEC_Knowledge_Population > GetKnowledgePopulation( const DEC_Knowledge_PopulationCollision& collision ) const;
    boost::shared_ptr< DEC_Knowledge_Population > GetKnowledgePopulation( const DEC_Knowledge_PopulationPerception& perception ) const;
    boost::shared_ptr< DEC_Knowledge_Population > GetKnowledgePopulation( const MIL_Population& population ) const;
    boost::shared_ptr< DEC_Knowledge_Population > GetKnowledgePopulationFromID( unsigned int nID ) const;
    void GetPopulations( T_KnowledgePopulationDiaIDVector& container ) const;
    void GetPopulations( T_KnowledgePopulationVector& container ) const;

    // Knowledge objects
//     void GetObjects( T_KnowledgeObjectVector& container ) const; // only for population
//     void GetObjectsInZone( T_KnowledgeObjectDiaIDVector& container, const MIL_ObjectFilter& filter, const TER_Localisation& zone ) const; // only for population
//     boost::shared_ptr< DEC_Knowledge_Object > GetKnowledgeObject( const MIL_Object_ABC& object ) const; // only for population
//     void GetKnowledgesObject( T_KnowledgeObjectVector& container ) const;
    //@}

    //! @name Tools
    //@{
    void TranslateKnowledges( const T_ConstKnowledgeAgentVector& sourceKnowledges, const boost::shared_ptr< MIL_KnowledgeGroup >& sourceKnowledgeGroup, T_ConstKnowledgeAgentVector& translatedKnowledges ) const;
    boost::shared_ptr< DEC_Knowledge_Object > CreateKnowledgeObject( MIL_Object_ABC& objectKnown );
    boost::shared_ptr< DEC_Knowledge_Object > CreateKnowledgeObject( boost::shared_ptr< DEC_Knowledge_Object >& object );
    void ApplyOnKnowledgesPerception( int currentTimeStep );
    void UpdateUniversalObjects();

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

    template < class UnaryFunction >
    void ApplyOnKnowledgesObject( UnaryFunction& fct ) const
    {
        GetKnowledgeObjectContainer().ApplyOnKnowledgesObject( fct );
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
    DEC_KS_ObjectKnowledgeSynthetizer* pKsObjectKnowledgeSynthetizer_;
    DEC_KS_Sharing* pKsSharing_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( DEC_KnowledgeBlackBoard_KnowledgeGroup )

#endif // __DEC_KnowledgeBlackBoard_KnowledgeGroup_h_
