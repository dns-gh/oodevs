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
#include "DEC_Knowledge_RapForLocal.h"
#include "DEC_Knowledge_Def.h"

namespace Common
{
    class MsgObjectKnowledge;
    class MsgPopulationKnowledge;
    class MsgUnitKnowledge;
}

class MIL_KnowledgeGroup;
class DEC_BlackBoard_CanContainKnowledgeAgent;
class DEC_BlackBoard_CanContainKnowledgeObject;
class DEC_BlackBoard_CanContainKnowledgePopulation;
class DEC_KS_AgentKnowledgeSynthetizer;
class DEC_KS_PopulationKnowledgeSynthetizer;
class DEC_KS_Sharing;
class DEC_Knowledge_Object;
class DEC_Knowledge_PopulationCollision;
class DEC_Knowledge_PopulationPerception;
class MIL_Army_ABC;
class MIL_Object_ABC;
class TER_Polygon;

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
    MIL_KnowledgeGroup&                           GetKnowledgeGroup              () const;
    DEC_KS_Sharing&                               GetKsSharing                   () const;
    DEC_BlackBoard_CanContainKnowledgeAgent&      GetKnowledgeAgentContainer     () const;
    DEC_BlackBoard_CanContainKnowledgePopulation& GetKnowledgePopulationContainer() const;
    //@}

    //! @name Operations
    //@{
    void Jam();
    //@}

    //! @name Network
    //@{
    virtual void SendFullState   () const;
    virtual void SendChangedState() const;
    //@}


    //! @name tools::Resolver
    //@{
    virtual boost::shared_ptr< DEC_Knowledge_Agent > ResolveKnowledgeAgent ( const Common::MsgUnitKnowledge&  asn ) const;
    virtual boost::shared_ptr< DEC_Knowledge_Agent > ResolveKnowledgeAgent (       uint                   nID ) const;

    virtual boost::shared_ptr< DEC_Knowledge_Object > ResolveKnowledgeObject( const Common::MsgObjectKnowledge& asn ) const;
    virtual boost::shared_ptr< DEC_Knowledge_Object > ResolveKnowledgeObject(       uint                   nID ) const;
    virtual boost::shared_ptr< DEC_Knowledge_Object > ResolveKnowledgeObject(       MIL_Object_ABC&        object ) const;

    virtual DEC_Knowledge_Population* ResolveKnowledgePopulation( const Common::MsgPopulationKnowledge& asn ) const;
    virtual DEC_Knowledge_Population* ResolveKnowledgePopulation(       unsigned int                       nID ) const;

    virtual boost::shared_ptr< DEC_Knowledge_Urban > ResolveKnowledgeUrban( const Common::MsgUrbanKnowledge& asn ) const;
    virtual boost::shared_ptr< DEC_Knowledge_Urban > ResolveKnowledgeUrban(       uint                   nID ) const;
    //@}

    //! @name Queries
    //@{

    // Knowledge agents
          bool                         IsKnown                     ( const MIL_Agent_ABC& agent ) const;
          boost::shared_ptr< DEC_Knowledge_Agent > GetKnowledgeAgent           ( const DEC_Knowledge_AgentPerception& perception ) const;
          boost::shared_ptr< DEC_Knowledge_Agent > GetKnowledgeAgentFromID     ( unsigned int nID ) const;
          void                         GetDetectedAgentsInZone     ( T_ConstKnowledgeAgentVector& container, const TER_Polygon&      zone ) const;          
          void                         GetDetectedAgentsInZone     ( T_ConstKnowledgeAgentVector& container, const TER_Localisation& zone ) const;          

    const T_KnowledgeAgentVector&      GetFriends                  () const;
          void                         GetFriendsInZone            ( T_ConstKnowledgeAgentVector& container, const TER_Localisation& zone ) const;
    const T_KnowledgeAgentVector&      GetEnemies                  () const;
          void                         GetLivingEnemiesInZone      ( T_ConstKnowledgeAgentVector& container, const TER_Polygon&      zone ) const;    
          void                         GetLivingEnemiesInZone      ( T_ConstKnowledgeAgentVector& container, const TER_Localisation& zone ) const;    
          void                         GetLivingEnemiesInCircle    ( T_ConstKnowledgeAgentVector& container, const MT_Vector2D& center, MT_Float rRadius ) const;
          bool                         EnemyPresenceInCircle       ( const MT_Vector2D& center, MT_Float rRadius ) const;

          void                         GetRefugeesInCircle         ( T_ConstKnowledgeAgentVector& container, const MT_Vector2D& center, MT_Float rRadius ) const;
          void                         GetSurrenderedAgentsInCircle( T_ConstKnowledgeAgentVector& container, const MT_Vector2D& center, MT_Float rRadius ) const;

    // Knowledge populations
    DEC_Knowledge_Population*    GetKnowledgePopulation      ( const DEC_Knowledge_PopulationCollision&  collision  ) const;
    DEC_Knowledge_Population*    GetKnowledgePopulation      ( const DEC_Knowledge_PopulationPerception& perception ) const;
    DEC_Knowledge_Population*    GetKnowledgePopulationFromID( unsigned int nID ) const;  
    void                         GetPopulations              ( T_KnowledgePopulationDiaIDVector& container ) const;
    void                         GetPopulations              ( T_KnowledgePopulationVector&      container ) const;
     //@}


    //! @name Tools
    //@{
    void TranslateKnowledges( const T_ConstKnowledgeAgentVector& sourceKnowledges, const MIL_KnowledgeGroup& sourceKnowledgeGroup, T_ConstKnowledgeAgentVector& translatedKnowledges ) const;
    boost::shared_ptr< DEC_Knowledge_Object > CreateKnowledgeObject ( const MIL_Army_ABC& teamKnowing, MIL_Object_ABC& objectKnown );
    void ApplyOnKnowledgesPopulationPerception();
    void ApplyOnKnowledgesAgentPerception( int currentTimeStep );
    //@}

private:
    MIL_KnowledgeGroup* pKnowledgeGroup_;
    
    // Containers
    DEC_BlackBoard_CanContainKnowledgeAgent*       pKnowledgeAgentContainer_;
    DEC_BlackBoard_CanContainKnowledgePopulation*  pKnowledgePopulationContainer_;
    DEC_BlackBoard_CanContainKnowledgeObject*      pKnowledgeObjectContainer_;

    // Knowledge sources
    DEC_KS_AgentKnowledgeSynthetizer*       pKsAgentKnowledgeSynthetizer_;
    DEC_KS_PopulationKnowledgeSynthetizer*  pKsPopulationKnowledgeSynthetizer_;
    DEC_KS_Sharing*                         pKsSharing_;

    //! @name Queries caches
    //@{
    unsigned int nLastCacheUpdateTick_;

    T_KnowledgeAgentVector  detectedContainer_;
    T_KnowledgeAgentVector  friendsContainer_;
    T_KnowledgeAgentVector  enemiesContainer_;
    T_KnowledgeAgentVector  militiasContainer_;
    T_KnowledgeAgentVector  terroristsContainer_;
    T_KnowledgeAgentVector  refugeesContainer_;
    T_KnowledgeAgentVector  surrenderedAgentsContainer_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( DEC_KnowledgeBlackBoard_KnowledgeGroup )

#endif // __DEC_KnowledgeBlackBoard_KnowledgeGroup_h_
