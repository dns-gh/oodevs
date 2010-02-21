// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/MIL_EntityManager.h $
// $Author: Nld $
// $Modtime: 10/06/05 10:17 $
// $Revision: 20 $
// $Workfile: MIL_EntityManager.h $
//
// *****************************************************************************

#ifndef __MIL_EntityManager_h_
#define __MIL_EntityManager_h_

#include "MIL.h"
#include "MT_Tools/MT_Profiler.h"
#include "MIL_EntityManager_ABC.h"
#include "MIL_EntityManagerStaticMethods.h"
#include "tools/Resolver.h"

namespace Common
{
    enum ObstacleType_DemolitionTargetType;

    class MsgPopulationOrder;
    class MsgUnitOrder;
    class MsgAutomatOrder;
    class MsgChangeDiplomacy;
    class MsgAutomatChangeKnowledgeGroup;
    class MsgAutomatChangeLogisticLinks;
    class MsgAutomatChangeSuperior;
    class MsgUnitChangeSuperior;
}

namespace MsgsSimToClient
{
    class MsgKnowledgeGroupCreation;
    class MsgKnowledgeGroupUpdate;
}

namespace MsgsClientToSim
{
    class MsgFragOrder;
    class MsgLogSupplyPushFlow;
    class MsgLogSupplyChangeQuotas;
    class MsgObjectMagicAction;
    class MsgPopulationMagicAction;
    class MsgSetAutomatMode;
    class MsgUnitCreationRequest;
    class MsgUnitMagicAction;    
    class MsgKnowledgeGroupCreationRequest;
    class MsgKnowledgeGroupUpdateRequest;
}

namespace xml
{
    class xostream;
    class xistream;
}

class ArmyFactory_ABC;
class AgentFactory_ABC;
class AutomateFactory_ABC;
class DEC_DataBase;
class FormationFactory_ABC;
class KnowledgeGroupFactory_ABC;
class MIL_EffectManager;
class MIL_ObjectManager;
class MIL_Army;
class MIL_Army_ABC;
class MIL_AgentPion;
class MIL_Automate;
class MIL_Formation;
class MIL_KnowledgeGroup;
class MIL_AgentTypePion;
class MIL_Object_ABC;
class MIL_ObjectType_ABC;
class MIL_ObjectBuilder_ABC;
class MIL_Population;
class TER_Localisation;
class MIL_Config;
class MIL_ProfilerMgr;
class MIL_Time_ABC;
class MIL_Intelligence;
class MIL_IDManager;
class PopulationFactory_ABC;


class HLA_Federate;
// =============================================================================
// @class  MIL_EntityManager
// Created: JVT 2004-08-03
// =============================================================================
class MIL_EntityManager : public MIL_EntityManager_ABC,
                          public MIL_EntityManagerStaticMethods,
                          private boost::noncopyable
{

public:
             MIL_EntityManager( const MIL_Time_ABC& time, MIL_EffectManager& effects, MIL_ProfilerMgr& profiler, HLA_Federate* hla, DEC_DataBase& database );
    virtual ~MIL_EntityManager();

    //! @name Factory
    //@{
    void CreateAutomat     ( xml::xistream& xis, MIL_Automate&  parent );
    void CreateAutomat     ( xml::xistream& xis, MIL_Formation& formation );
    void CreateIntelligence( xml::xistream& xis, MIL_Formation& formation );
    MIL_AgentPion&  CreatePion( const MIL_AgentTypePion& type, MIL_Automate&  automate , xml::xistream& xis );
    MIL_AgentPion&  CreatePion( const MIL_AgentTypePion& type, MIL_Automate& automate, const MT_Vector2D& vPosition );

    void                        CreateObject( xml::xistream& xis, MIL_Army_ABC& army ); 
    MIL_Object_ABC*             CreateObject( MIL_Army_ABC& army, const std::string& type, const TER_Localisation* pLocalisation, Common::ObstacleType_DemolitionTargetType obstacleType );    
    MIL_Object_ABC*             CreateObject( const std::string& type, MIL_Army_ABC& army, const TER_Localisation& localisation );
    MIL_Object_ABC*             CreateObject( MIL_Army_ABC& army, const MIL_ObjectBuilder_ABC& builder );
    MIL_Object_ABC*             CreateObject( const std::string& type, MIL_Army_ABC& army, const TER_Localisation& localisation, const std::string& strOption, const std::string& strExtra, double rCompletion, double rMining, double rBypass );        
    //@}

    //! @name Accessors
    //@{
    virtual       MIL_Automate*       FindAutomate  ( uint nID ) const;
    virtual       MIL_KnowledgeGroup* FindKnowledgeGroup( uint nID ) const; // LTO
    virtual       MIL_AgentPion*      FindAgentPion ( uint nID ) const;
                  MIL_Object_ABC*     FindObject    ( uint nID ) const;
    virtual const MIL_ObjectType_ABC& FindObjectType( const std::string& type ) const;
        
    const tools::Resolver< MIL_Army_ABC >& MIL_EntityManager::GetArmies() const;
    
    MIL_EffectManager& GetEffectManager() const;
    //@}

    //! @name Stats
    //@{
    MT_Float GetKnowledgesTime         () const;
    MT_Float GetDecisionsTime          () const;
    MT_Float GetAutomatesDecisionTime  () const;
    MT_Float GetPionsDecisionTime      () const;
    MT_Float GetPopulationsDecisionTime() const;
    MT_Float GetActionsTime            () const;
    MT_Float GetEffectsTime            () const;
    MT_Float GetStatesTime             () const;
    //@}

    //! @name Operations
    //@{
    void ReadODB             ( const MIL_Config& config );
    void SendStateToNewClient() const;
    void Update              ();
    void Clean               ();
    //@}

    //! @name Network
    //@{
    void OnReceiveMsgUnitMagicAction             ( const MsgsClientToSim::MsgUnitMagicAction&               message, unsigned int nCtx );
    void OnReceiveMsgPopulationOrder             ( const Common::MsgPopulationOrder&                        message, unsigned int nCtx );     
    void OnReceiveMsgUnitOrder                   ( const Common::MsgUnitOrder&                              message, unsigned int nCtx ); 
    void OnReceiveMsgAutomatOrder                ( const Common::MsgAutomatOrder&                           message, unsigned int nCtx );
    void OnReceiveMsgSetAutomateMode             ( const MsgsClientToSim::MsgSetAutomatMode&                message, unsigned int nCtx );
    void OnReceiveMsgUnitCreationRequest         ( const MsgsClientToSim::MsgUnitCreationRequest&           message, unsigned int nCtx );
    void OnReceiveMsgFragOrder                   ( const MsgsClientToSim::MsgFragOrder&                     message, unsigned int nCtx );
    void OnReceiveMsgObjectMagicAction           ( const MsgsClientToSim::MsgObjectMagicAction&             message, unsigned int nCtx );
    void OnReceiveMsgPopulationMagicAction       ( const MsgsClientToSim::MsgPopulationMagicAction&         message, unsigned int nCtx );
    void OnReceiveMsgChangeDiplomacy             ( const Common::MsgChangeDiplomacy&                        message, unsigned int nCtx );
    void OnReceiveMsgAutomateChangeKnowledgeGroup( const Common::MsgAutomatChangeKnowledgeGroup&            message, unsigned int nCtx );
    void OnReceiveMsgAutomateChangeLogisticLinks ( const Common::MsgAutomatChangeLogisticLinks&             message, unsigned int nCtx );
    void OnReceiveMsgAutomateChangeSuperior      ( const Common::MsgAutomatChangeSuperior&                  message, unsigned int nCtx );
    void OnReceiveMsgUnitChangeSuperior          ( const Common::MsgUnitChangeSuperior&                     message, unsigned int nCtx );
    void OnReceiveMsgLogSupplyChangeQuotas       ( const MsgsClientToSim::MsgLogSupplyChangeQuotas&         message, unsigned int nCtx );
    void OnReceiveMsgLogSupplyPushFlow           ( const MsgsClientToSim::MsgLogSupplyPushFlow&             message, unsigned int nCtx );
    // LTO begin
    void OnReceiveMsgKnowledgeGroupCreation      ( const MsgsClientToSim::MsgKnowledgeGroupCreationRequest& message, unsigned int nCtx );
    void OnReceiveMsgKnowledgeGroupUpdate        ( const MsgsClientToSim::MsgKnowledgeGroupUpdateRequest&   message, unsigned int nCtx );
    // LTO end
    //@}

    //! @name Population channeling
    //@{
    void ChannelPopulations( const TER_Localisation& localisation );
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    template< typename Archive > friend  void save_construct_data( Archive& archive, const MIL_EntityManager* role, const unsigned int /*version*/ );
    template< typename Archive > friend  void load_construct_data( Archive& archive, MIL_EntityManager* role, const unsigned int /*version*/ );

    void WriteODB( xml::xostream& xos ) const;
    //@}
    
private:
    //! @name types
    //@{
    typedef std::map< unsigned int, MIL_Intelligence* > T_IntelligenceMap;
    typedef T_IntelligenceMap::const_iterator   CIT_IntelligenceMap;
    //@}

private:
    //! @name Init
    //@{

    // ODB
    void InitializeArmies     ( xml::xistream& xis );
    void InitializeDiplomacy  ( xml::xistream& xis );
    void InitializePopulations( xml::xistream& xis );
    void ReadDiplomacy        ( xml::xistream& xis );
    //@}

    //! @name Update
    //@{
    void UpdateKnowledges();
    void UpdateDecisions ();
    void UpdateActions   ();
    void UpdateEffects   ();
    void UpdateStates    ();
    void UpdateKnowledgeGroups(); // LTO

    void PreprocessRandomBreakdowns();
    //@}

private:
    const MIL_Time_ABC& time_;
    HLA_Federate*       hla_;
    MIL_ProfilerMgr&    profilerManager_;
    MIL_EffectManager&  effectManager_;
    MIL_ObjectManager*  pObjectManager_;

    T_IntelligenceMap intelligences_;

    // ID Manager
    std::auto_ptr< MIL_IDManager > idManager_;
    // Factories
    std::auto_ptr< PopulationFactory_ABC >      populationFactory_;
    std::auto_ptr< AgentFactory_ABC >           agentFactory_;
    std::auto_ptr< AutomateFactory_ABC >        automateFactory_;
    std::auto_ptr< FormationFactory_ABC >       formationFactory_;
    std::auto_ptr< KnowledgeGroupFactory_ABC >  knowledgeGroupFactory_;
    std::auto_ptr< ArmyFactory_ABC >            armyFactory_;

    // Profiling
    MT_Profiler   profiler_;
    MT_Float      rKnowledgesTime_;
    MT_Float      rAutomatesDecisionTime_;
    MT_Float      rPionsDecisionTime_;    
    MT_Float      rPopulationsDecisionTime_;
    MT_Float      rActionsTime_;
    MT_Float      rEffectsTime_;
    MT_Float      rStatesTime_;

    unsigned int          nRandomBreakdownsNextTimeStep_;
};

BOOST_CLASS_EXPORT_KEY( MIL_EntityManager )

#endif // __MIL_EntityManager_h_

