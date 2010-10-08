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

    class MsgCrowdOrder;
    class MsgUnitOrder;
    class MsgAutomatOrder;
    class MsgAutomatChangeKnowledgeGroup;
    class MsgAutomatChangeLogisticLinks;
    class MsgAutomatChangeSuperior;
    class MsgUnitChangeSuperior;
    class Tasker;
}

namespace MsgsSimToClient
{
    class MsgKnowledgeGroupCreation;
    class MsgKnowledgeGroupUpdate;
}

namespace MsgsClientToSim
{
    class MsgFragOrder;
    class MsgObjectMagicAction;
    class MsgSetAutomatMode;
    class MsgUnitCreationRequest;
    class MsgUnitMagicAction;
    class MsgKnowledgeMagicAction;
    class MsgMagicAction;
}

namespace urban
{
    class TerrainObject_ABC;
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
class MIL_AgentTypePion;
class MIL_AutomateType;
class MIL_EffectManager;
class MIL_ObjectManager;
class MIL_Army;
class MIL_Army_ABC;
class MIL_AgentPion;
class MIL_Automate;
class MIL_Config;
class MIL_Entity_ABC;
class MIL_Formation;
class MIL_IDManager;
class MIL_Intelligence;
class MIL_KnowledgeGroup;
class MIL_Object_ABC;
class MIL_ObjectType_ABC;
class MIL_ObjectBuilder_ABC;
class MIL_Population;
class MIL_ProfilerMgr;
class MIL_Time_ABC;
class PopulationFactory_ABC;
class TER_Localisation;
class UrbanModel;

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
             MIL_EntityManager( const MIL_Time_ABC& time, MIL_EffectManager& effects, MIL_ProfilerMgr& profiler, HLA_Federate* hla, DEC_DataBase& database, unsigned int gcPause, unsigned int gcMult );
    virtual ~MIL_EntityManager();

    //! @name Factory
    //@{
    void CreateAutomat     ( xml::xistream& xis, MIL_Entity_ABC&  parent );
    void CreateAutomat     ( const MIL_AutomateType& type, unsigned int knowledgeGroup, const std::string& name, MIL_Entity_ABC& parent );
    void CreateIntelligence( xml::xistream& xis, MIL_Formation& formation );
    MIL_AgentPion&  CreatePion( const MIL_AgentTypePion& type, MIL_Automate&  automate , xml::xistream& xis );
    MIL_AgentPion&  CreatePion( const MIL_AgentTypePion& type, MIL_Automate& automate, const MT_Vector2D& vPosition );

    void            CreateObject( xml::xistream& xis, MIL_Army_ABC& army );
    MIL_Object_ABC* CreateObject( MIL_Army_ABC& army, const std::string& type, const TER_Localisation* pLocalisation, Common::ObstacleType_DemolitionTargetType obstacleType );
    MIL_Object_ABC* CreateObject( const std::string& type, MIL_Army_ABC& army, const TER_Localisation& localisation );
    MIL_Object_ABC* CreateObject( MIL_Army_ABC& army, const MIL_ObjectBuilder_ABC& builder );
    MIL_Object_ABC* CreateObject( const std::string& type, MIL_Army_ABC& army, const TER_Localisation& localisation, const std::string& strOption, const std::string& strExtra, double rCompletion, double rMining, double rBypass );
    MIL_Object_ABC* CreateDistantObject( const std::string& type, MIL_Army_ABC& army, const TER_Localisation& localisation, const std::string& name );
    //@}

    //! @name Accessors
    //@{
    virtual       MIL_Automate*       FindAutomate  ( unsigned int nID ) const;
    virtual       MIL_Population*     FindPopulation( unsigned int nID ) const;
    virtual       MIL_Formation*      FindFormation ( unsigned int nID ) const;
    virtual       MIL_KnowledgeGroup* FindKnowledgeGroup( unsigned int nID ) const; // LTO
    virtual       MIL_AgentPion*      FindAgentPion ( unsigned int nID ) const;
                  MIL_Object_ABC*     FindObject    ( unsigned int nID ) const;
    virtual const MIL_ObjectType_ABC& FindObjectType( const std::string& type ) const;

    const tools::Resolver< MIL_Army_ABC >& MIL_EntityManager::GetArmies() const;
    const bool HasInfiniteDotations() const;
    //@}

    //! @name Stats
    //@{
    double GetKnowledgesTime         () const;
    double GetDecisionsTime          () const;
    double GetAutomatesDecisionTime  () const;
    double GetPionsDecisionTime      () const;
    double GetPopulationsDecisionTime() const;
    double GetActionsTime            () const;
    double GetEffectsTime            () const;
    double GetStatesTime             () const;
    //@}

    //! @name Operations
    //@{
    void ReadODB             ( const MIL_Config& config );
    void CreateUrbanObjects  ( UrbanModel& urbanModel, const MIL_Config& config );
    void SendStateToNewClient() const;
    void Update              ();
    void Clean               ();
    //@}

    //! @external helper
    //@{
    void SetToTasker( Common::Tasker& tasker, unsigned int id ) const;
    //@}

    //! @name Network
    //@{
    void OnReceiveMsgUnitMagicAction             ( const MsgsClientToSim::MsgUnitMagicAction&               message, unsigned int nCtx );
    void OnReceiveMsgObjectMagicAction           ( const MsgsClientToSim::MsgObjectMagicAction&             message, unsigned int nCtx );
    void OnReceiveMsgUnitOrder                   ( const Common::MsgUnitOrder&                              message, unsigned int nCtx );
    void OnReceiveMsgAutomatOrder                ( const Common::MsgAutomatOrder&                           message, unsigned int nCtx );
    void OnReceiveMsgCrowdOrder                  ( const Common::MsgCrowdOrder&                             message, unsigned int nCtx );
    void OnReceiveMsgFragOrder                   ( const MsgsClientToSim::MsgFragOrder&                     message, unsigned int nCtx );
    void OnReceiveMsgSetAutomateMode             ( const MsgsClientToSim::MsgSetAutomatMode&                message, unsigned int nCtx );
    void OnReceiveMsgUnitCreationRequest         ( const MsgsClientToSim::MsgUnitCreationRequest&           message, unsigned int nCtx );
    void OnReceiveMsgKnowledgeMagicAction        ( const MsgsClientToSim::MsgKnowledgeMagicAction&          message, unsigned int nCtx );
    void OnReceiveMsgChangeDiplomacy             ( const MsgsClientToSim::MsgMagicAction&                   message, unsigned int nCtx );
    void OnReceiveMsgChangeResourceLinks         ( const MsgsClientToSim::MsgMagicAction&                   message, unsigned int nCtx );
    // LTO begin
    void OnReceiveMsgKnowledgeGroupCreation      ( const MsgsClientToSim::MsgMagicAction&                   message, unsigned int nCtx );
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
    //! @name Helpers
    //@{
    void ProcessMsgAutomateChangeKnowledgeGroup( const MsgsClientToSim::MsgUnitMagicAction&      message, unsigned int nCtx );
    void ProcessMsgAutomateChangeLogisticLinks ( const MsgsClientToSim::MsgUnitMagicAction&      message, unsigned int nCtx );
    void ProcessMsgAutomateChangeSuperior      ( const MsgsClientToSim::MsgUnitMagicAction&      message, unsigned int nCtx );
    void ProcessMsgUnitChangeSuperior          ( const MsgsClientToSim::MsgUnitMagicAction&      message, unsigned int nCtx );
    void ProcessMsgLogSupplyChangeQuotas       ( const MsgsClientToSim::MsgUnitMagicAction&      message, unsigned int nCtx );
    void ProcessMsgLogSupplyPushFlow           ( const MsgsClientToSim::MsgUnitMagicAction&      message, unsigned int nCtx );
    void ProcessMsgMagicActionMoveTo           ( const MsgsClientToSim::MsgUnitMagicAction&      message, unsigned int nCtx );
    // LTO begin
    void ProcessMsgKnowledgeGroupUpdate        ( const MsgsClientToSim::MsgKnowledgeMagicAction& message, unsigned int nCtx );
    void ProcessMsgMagicActionCreateFireOrder  ( const MsgsClientToSim::MsgUnitMagicAction&      message, unsigned int nCtx );
    // LTO end
    void ProcessMsgAutomatCreationRequest      ( const MsgsClientToSim::MsgUnitMagicAction&      message, MIL_Entity_ABC& entity );
    //@}

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
    void InitializeDotations  ( xml::xistream& xis );
    void InitializePopulations( xml::xistream& xis );
    void ReadDiplomacy        ( xml::xistream& xis );
    //@}

    //! @name Urban
    //@{
    void CreateUrbanObject( const urban::TerrainObject_ABC& object );
    void LoadUrbanStates( const MIL_Config& config );
    void NotifyPionsInsideUrbanObject();
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
    class UrbanWrapperVisitor;

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
    double      rKnowledgesTime_;
    double      rAutomatesDecisionTime_;
    double      rPionsDecisionTime_;
    double      rPopulationsDecisionTime_;
    double      rActionsTime_;
    double      rEffectsTime_;
    double      rStatesTime_;

    unsigned int  nRandomBreakdownsNextTimeStep_;
    unsigned int  gcPause_;
    unsigned int  gcMult_;
    bool          infiniteDotations_;
};

BOOST_CLASS_EXPORT_KEY( MIL_EntityManager )

#endif // __MIL_EntityManager_h_


