// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/MIL_AgentPion.h $
// $Author: Jvt $
// $Modtime: 31/03/05 11:38 $
// $Revision: 18 $
// $Workfile: MIL_AgentPion.h $
//
// *****************************************************************************

#ifndef __MIL_AgentPion_h_
#define __MIL_AgentPion_h_

#include "MIL_Agent_ABC.h"
#include "MIL_AgentTypePion.h"
#include "Entities/Orders/MIL_PionOrderManager.h"
#include <tools/Resolver.h>
#include <map>

namespace logistic
{
    class LogisticHierarchy_ABC;
}

namespace sword
{
    class MissionParameters;
    class UnitOrder;
    class UnitMagicAction;
}

namespace xml
{
    class xostream;
    class xistream;
}

class DEC_Decision_ABC;
class DEC_KnowledgeBlackBoard_AgentPion;
class DEC_Agent_Path;
class MIL_AffinitiesMap;
class MIL_Army;
class MIL_Automate;
class MIL_Color;
class MIL_DictionaryExtensions;
class MIL_EntityManager;
class MIL_HumanRepartition;
class PHY_Dotation;
class PHY_DotationStock;

// =============================================================================
/** @class  MIL_AgentPion
    @brief  MIL_AgentPion
*/
// Created: JVT 2004-08-03
// =============================================================================
class MIL_AgentPion : public MIL_Agent_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_AgentPion( const MIL_AgentTypePion& type, MissionController_ABC& controller, MIL_Automate& automate, xml::xistream& xis );
             MIL_AgentPion( const MIL_AgentTypePion& type, MissionController_ABC& controller );
    virtual ~MIL_AgentPion();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;

    virtual void WriteODB( xml::xostream& xos ) const;
    //@}

    //! @name Init
    //@{
    void ReadOverloading( xml::xistream& xis );
    void Finalize();
    //@}

    //! @name Accessors
    //@{
    virtual DEC_KnowledgeBlackBoard_AgentPion& GetKnowledge() const;
    const MIL_PionOrderManager& GetOrderManager() const;
    MIL_PionOrderManager& GetOrderManager();
    virtual DEC_Decision_ABC& GetDecision(); //$$$ Dérolifier DEC_Decision_ABC
    virtual const DEC_Decision_ABC& GetDecision() const; //$$$ Dérolifier DEC_Decision_ABC

    virtual MIL_Army_ABC& GetArmy() const;
    virtual boost::shared_ptr< MIL_KnowledgeGroup > GetKnowledgeGroup() const;
    int GetNumberOfFireHoses( int bestExtinguisherAgent );
    virtual const MIL_Automate& GetAutomate() const;
    virtual MIL_Automate& GetAutomate();
    virtual const MIL_AgentType_ABC&  GetType() const;
    virtual bool IsMarkedForDestruction() const;
    virtual bool IsDead() const;
    virtual bool IsNeutralized() const;
    virtual bool UpdateUnderIndirectFire();
    virtual bool IsPC() const;
    virtual void SetPionAsCommandPost( bool pc );
    virtual bool BelongsTo( const MIL_KnowledgeGroup& group ) const;
    virtual double Distance( const MIL_Agent_ABC& pion ) const;
    virtual const std::string& GetCriticalIntelligence() const;
    virtual float GetAffinity( unsigned long teamID ) const;
    virtual const MIL_HumanRepartition& GetHumanRepartition() const;
    virtual bool IsCivilian() const;
    virtual bool CanEmitReports() const;
    virtual bool CanBeDeleted() const;
    bool CanTransportDestroyed() const;

    virtual bool IsImmobilized() const;
    virtual bool IsAutonomous() const; // Drones
    virtual bool IsJammed() const;
    virtual bool IsLogisticJammed() const;
    virtual bool HasBeenTeleported() const;
            bool IsMasaLife() const;
    //@}

    //! @name logistic
    //@{
    virtual logistic::LogisticHierarchy_ABC& GetLogisticHierarchy() const;
            void Apply2( std::function< void( PHY_Dotation& ) > visitor ) const;
            void Apply2( std::function< void( PHY_DotationStock& ) > visitor ) const;
    //@}

    //! @name Operations
    //@{
    void PreprocessRandomBreakdowns( unsigned int nEndDayTimeStep ) const;
    void UpdateKnowledges( int currentTimeStep );
    void CleanKnowledges();
    void CleanDeletedAgentKnowledges();
    void UpdateDecision( float duration );
    void UpdateState();
    void UpdateNetwork();
    void Clean();
    virtual void RegisterPath( const DEC_Agent_Path& agentPath );
    virtual void UnregisterPath( const DEC_Agent_Path& agentPath );
    virtual bool HasPath() const;
    void DeleteUnit( unsigned int nCtx, unsigned int clientId );
    //@}

    //! @name Knowledge
    //@{
    virtual boost::shared_ptr< DEC_Knowledge_Agent > CreateKnowledge( const MIL_KnowledgeGroup& group );
    virtual bool IsPerceived( const MIL_Agent_ABC& agent ) const; //$$$$ DEGUEU - VOIR AVEC MODELISATEURS
    //@}

    //! @name Operation on Knowledge
    //@{
    double GetDangerosity( boost::shared_ptr< DEC_Knowledge_Agent > pTargetKnowledge, boost::shared_ptr< MT_Vector2D > position, bool bUseAmmo ) const;
    //@}

    //! @name Network
    //@{
    void SendCreation( unsigned int nCtx ) const;
    void SendFullState( unsigned int nCtx ) const;
    void SendKnowledge( unsigned int nCtx ) const;

    void OnReceiveUnitMagicAction( const sword::UnitMagicAction& msg, const tools::Resolver< MIL_Army_ABC >& armies, unsigned int nCtx,
                                   unsigned int clientId );
    void OnReceiveMagicActionMoveTo( const sword::UnitMagicAction& asn );
    uint32_t OnReceiveOrder( const sword::UnitOrder& msg );
    void OnReceiveFragOrder( const sword::FragOrder& msg, const std::function< void( uint32_t ) >& sendAck );
    void OnReceiveChangeSuperior( const MIL_EntityManager& manager, unsigned int automatId );
    void OnReceiveMagicActionMoveTo( const MT_Vector2D& vPosition ); // Magic move automate
    void SetExtensions( const MIL_DictionaryExtensions& ext );
    //@}

    //! @name Misc operations
    //@{
    void UpdateSurrenderedState();
    virtual void MagicMove( const MT_Vector2D& vNewPos );
    void NotifyAttacking( MIL_Agent_ABC& target, bool mustReport ) const;
    void NotifyAttacking( MIL_Population& target ) const;
    virtual void NotifyAttackedBy( MIL_AgentPion& pion, bool mustReport );
    virtual void NotifyAttackedBy( MIL_Population& population );
    virtual bool CanInteractWithTraffic() const;
    virtual bool CanBeImpactedByTraffic() const;
    virtual void InteractWithTraffic( const std::vector< TER_Agent_ABC* >& agents );
    virtual void InteractWithTraffic( const MIL_Agent_ABC& agent );
    void ChangeSuperior( MIL_Automate& newAutomate );
    void NotifySendHeadquarters();
    //@}

private:
    //! @name Operations
    //@{
    void UpdatePhysicalState();
    void CancelCurrentMission();
    void ResetTeleported();
    //@}

    //! @name Magic actions
    //@{
    void OnReceiveChangeHumanFactors( const sword::MissionParameters& msg );
    void OnReceiveResupplyHumans( bool withLog );
    void OnReceiveResupplyResources( bool withLog );
    void OnReceiveResupplyEquipement( bool withLog );
    void OnReceiveResupplyAll( bool withLog );
    void OnReceiveResupply( const sword::MissionParameters& msg );
    void OnReceiveDestroyAll();
    void OnReceiveDestroyComponent();
    void OnReceiveDeleteUnit( unsigned int nCtx, unsigned int clientId );
    void OnReceiveRecoverHumansTransporters();
    void OnReceiveCreateWound( const sword::MissionParameters& asn );
    void OnReceiveCriticalIntelligence( const sword::MissionParameters& msg );
    void OnReceiveCreateBreakdowns( const sword::MissionParameters& msg );
    void OnReceiveCreateWounds( const sword::MissionParameters& msg );
    void OnReceiveChangeEquipmentState( const sword::MissionParameters& msg );
    void OnReceiveChangeHumanState( const sword::MissionParameters& msg );
    void OnReceiveChangeDotation( const sword::MissionParameters& msg );
    void OnReceiveCreateDirectFireOrder( const sword::MissionParameters& msg );
    void OnReceiveLoadUnit( const sword::MissionParameters& msg, bool load );
    void OnReloadBrain( const sword::MissionParameters& msg );
    void OnChangeBrainDebug( const sword::MissionParameters& msg );
    void OnChangePosture( const sword::MissionParameters& msg );
    void OnReceiveFinishLogisticHandlings();
    //@}

    //! @name Serialization
    //@{
    template< typename Archive > friend void save_construct_data( Archive& archive, const MIL_AgentPion* pion, const unsigned int version );
    template< typename Archive > friend void load_construct_data( Archive& archive, MIL_AgentPion* pion, const unsigned int version );
    //@}

protected:
    //! @name Accessors
    //@{
    void SetColor( std::unique_ptr< MIL_Color > color );
    MIL_Color* const GetColor() const;
    const std::string& GetLevel() const;
    void SetLevel( const std::string& level );
    MIL_DictionaryExtensions* const GetExtensions() const;
    const std::string& GetSymbol() const;
    void SetSymbol( const std::string& symbol );
    MIL_AffinitiesMap* const GetAffinities() const;
    void SetAffinities( std::unique_ptr< MIL_AffinitiesMap > affinities );
    void SetKnowledge( DEC_KnowledgeBlackBoard_AgentPion* knowledge );
    void SetAutomate( MIL_Automate* automate );
    const MissionController_ABC& GetController() const;
    //@}

private:
    //! @name Member data
    //@{
    const MIL_AgentTypePion*                  pType_;
    bool                                      bHasChanged_;
    bool                                      markedForDestruction_;
    bool                                      brainDeleted_;
    bool                                      updateDecisionsDead_;
    std::string                               app6Symbol_;
    std::string                               level_;
    std::string                               criticalIntelligence_;
    MIL_Automate*                             pAutomate_;
    DEC_KnowledgeBlackBoard_AgentPion*        pKnowledgeBlackBoard_;
    std::unique_ptr< MIL_PionOrderManager >     pOrderManager_;
    std::unique_ptr< MIL_AffinitiesMap >        pAffinities_;
    std::unique_ptr< MIL_DictionaryExtensions > pExtensions_;
    std::unique_ptr< MIL_Color >                pColor_;
    std::set< const DEC_Agent_Path* >         agentPaths_;
    bool                                      teleported_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( MIL_AgentPion )

#endif // __MIL_AgentPion_h_
