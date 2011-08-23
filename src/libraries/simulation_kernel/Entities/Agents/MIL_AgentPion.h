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
#include "tools/Resolver.h"
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
class MIL_AffinitiesMap;
class MIL_Army;
class MIL_Automate;
class MIL_DictionaryExtensions;
class MIL_Color;
class MIL_EntityManager;
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
             MIL_AgentPion( const MIL_AgentTypePion& type, MIL_Automate& automate, const AlgorithmsFactories& algorithmFactories, xml::xistream& xis );
             MIL_AgentPion( const MIL_AgentTypePion& type, MIL_Automate& automate, const AlgorithmsFactories& algorithmFactories );
             MIL_AgentPion( const MIL_AgentTypePion& type, MIL_Automate& automate, const AlgorithmsFactories& algorithmFactories, const std::string name );
             MIL_AgentPion( const MIL_AgentTypePion& type, const AlgorithmsFactories& algorithmFactories );
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
    //@}

    //! @name Accessors
    //@{
    virtual DEC_KnowledgeBlackBoard_AgentPion& GetKnowledge() const;
    const MIL_PionOrderManager& GetOrderManager() const;
    MIL_PionOrderManager& GetOrderManager();
    virtual DEC_Decision_ABC& GetDecision(); //$$$ Dérolifier DEC_Decision_ABC
    virtual const DEC_Decision_ABC& GetDecision() const; //$$$ Dérolifier DEC_Decision_ABC
    virtual const AlgorithmsFactories& GetAlgorithms() const;

    virtual MIL_Army_ABC& GetArmy() const;
    virtual MIL_KnowledgeGroup& GetKnowledgeGroup() const;
    int GetNumberOfFireHoses( int bestExtinguisherAgent );
    virtual const MIL_Automate& GetAutomate() const;
    virtual MIL_Automate& GetAutomate();
    virtual const MIL_AgentType_ABC&  GetType() const;
    virtual bool IsDead() const;
    virtual bool IsNeutralized() const;
    virtual bool IsPC() const;
    virtual void SetPionAsPostCommand();
    virtual bool BelongsTo( const MIL_KnowledgeGroup& group ) const;
    virtual double Distance( const MIL_Agent_ABC& pion ) const;
    virtual const std::string& GetCriticalIntelligence() const;
    virtual float GetAffinity( unsigned long teamID ) const;

    bool IsAutonomous() const; // Drones
    //@}

    //! @name logistic
    //@{
    virtual logistic::LogisticHierarchy_ABC& GetLogisticHierarchy() const;
            void Apply2( boost::function< void( PHY_Dotation& ) > visitor ) const;
            void Apply2( boost::function< void( PHY_DotationStock& ) > visitor ) const;
    //@}

    //! @name Operations
    //@{
    void PreprocessRandomBreakdowns( unsigned int nEndDayTimeStep ) const;
    void UpdateKnowledges( int currentTimeStep );
    void CleanKnowledges();
    void UpdateDecision( float duration );
    void UpdateState();
    void UpdateNetwork();
    void Clean();
    void Register( MissionController_ABC& pController );
    //@}

    //! @name Knowledge
    //@{
    virtual boost::shared_ptr< DEC_Knowledge_Agent > CreateKnowledge( const MIL_KnowledgeGroup& knowledgeGroup );
    virtual bool IsPerceived( const MIL_Agent_ABC& agent ) const; //$$$$ DEGUEU - VOIR AVEC MODELISATEURS
    //@}

    //! @name Operation on Knowledge
    //@{
    double GetDangerosity( boost::shared_ptr< DEC_Knowledge_Agent > pTargetKnowledge, boost::shared_ptr< MT_Vector2D > position  ) const;
    //@}

    //! @name Network
    //@{
    void SendCreation( unsigned int nCtx ) const;
    void SendFullState( unsigned int nCtx ) const;
    void SendKnowledge( unsigned int nCtx ) const;

    void OnReceiveUnitMagicAction( const sword::UnitMagicAction& msg, const tools::Resolver< MIL_Army_ABC >& armies );
    void OnReceiveMagicActionMoveTo( const sword::UnitMagicAction& asn );
    void OnReceiveOrder( const sword::UnitOrder& msg );
    void OnReceiveFragOrder( const sword::FragOrder& msg );
    void OnReceiveChangeSuperior( const MIL_EntityManager& manager, const sword::UnitMagicAction& msg );
    void OnReceiveMagicSurrender();
    void OnReceiveMagicCancelSurrender();
    void OnReceiveMagicActionMoveTo( const MT_Vector2D& vPosition ); // Magic move automate
    //@}

    //! @name Misc operations
    //@{
    virtual void MagicMove( const MT_Vector2D& vNewPos );
    void NotifyAttacking( MIL_Agent_ABC& target ) const;
    void NotifyAttacking( MIL_Population& target ) const;
    virtual void NotifyAttackedBy( MIL_AgentPion& pion );
    virtual void NotifyAttackedBy( MIL_Population& population );
    void ChangeSuperior( MIL_Automate& newAutomate );

    //@}

private:
    //! @name Operations
    //@{
    void UpdatePhysicalState();
    //@}

    //! @name Magic actions
    //@{
    void OnReceiveChangeHumanFactors( const sword::MissionParameters& msg );
    void OnReceiveResupplyHumans();
    void OnReceiveResupplyResources();
    void OnReceiveResupplyEquipement();
    void OnReceiveResupplyAll();
    void OnReceiveResupply( const sword::MissionParameters& msg );
    void OnReceiveDestroyAll();
    void OnReceiveDestroyComponent();
    void OnReceiveRecoverHumansTransporters();
    void OnReceiveCreateWound( const sword::MissionParameters& asn );
    void OnReceiveCriticalIntelligence( const sword::UnitMagicAction& msg );
    //@}

    //! @name Serialization
    //@{
    template< typename Archive > friend void save_construct_data( Archive& archive, const MIL_AgentPion* pion, const unsigned int version );
    template< typename Archive > friend void load_construct_data( Archive& archive, MIL_AgentPion* pion, const unsigned int version );
    //@}

private:
    //! @name Member data
    //@{
    const MIL_AgentTypePion*                  pType_;
    bool                                      bIsPC_;
    std::string                               criticalIntelligence_;
    MIL_Automate*                             pAutomate_;
    const AlgorithmsFactories&                algorithmFactories_;
    DEC_KnowledgeBlackBoard_AgentPion*        pKnowledgeBlackBoard_;
    std::auto_ptr< MIL_PionOrderManager >     pOrderManager_;
    std::auto_ptr< MIL_AffinitiesMap >        pAffinities_;
    std::auto_ptr< MIL_DictionaryExtensions > pExtensions_;
    std::auto_ptr< MIL_Color >                pColor_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( MIL_AgentPion )

#endif // __MIL_AgentPion_h_
