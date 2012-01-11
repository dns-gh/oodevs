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

namespace sword
{
    enum ObstacleType_DemolitionTargetType;
    class CrowdOrder;
    class UnitOrder;
    class AutomatOrder;
    class Tasker;
    class FragOrder;
    class ObjectMagicAction;
    class SetAutomatMode;
    class UnitCreationRequest;
    class UnitMagicAction;
    class KnowledgeMagicAction;
    class MagicAction;
    class MissionParameter_Value;
    class BurningCellRequest;
    class ParentEntity;
}

namespace urban
{
    class Model;
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
class FormationFactory_ABC;
class KnowledgeGroupFactory_ABC;
class MIL_AgentTypePion;
class MIL_AutomateType;
class MIL_EffectManager;
class MIL_ObjectManager;
class MIL_Army_ABC;
class MIL_AgentPion;
class MIL_Automate;
class MIL_AutomateLOG;
class MIL_Config;
class MIL_Entity_ABC;
class MIL_Formation;
class MIL_IDManager;
class MIL_KnowledgeGroup;
class MIL_Object_ABC;
class MIL_ObjectType_ABC;
class MIL_ObjectBuilder_ABC;
class MIL_Population;
class MIL_Inhabitant;
class MIL_ProfilerMgr;
class MIL_Time_ABC;
class MissionController_ABC;
class InhabitantFactory_ABC;
class PopulationFactory_ABC;
class TER_Localisation;
class UrbanObjectWrapper;
class KnowledgesVisitor_ABC;

// =============================================================================
// @class  MIL_EntityManager
// Created: JVT 2004-08-03
// =============================================================================
class MIL_EntityManager : public MIL_EntityManager_ABC
                        , public MIL_EntityManagerStaticMethods
{
public:
             MIL_EntityManager( const MIL_Time_ABC& time, MIL_EffectManager& effects, MIL_ProfilerMgr& profiler, unsigned int gcPause, unsigned int gcMult );
    virtual ~MIL_EntityManager();

    //! @name Factory
    //@{
    void CreateAutomat( xml::xistream& xis, MIL_Entity_ABC&  parent );
    void CreateAutomat( const MIL_AutomateType& type, unsigned int knowledgeGroup, const std::string& name, MIL_Entity_ABC& parent, unsigned int nCtx );
    void CreateIntelligence( xml::xistream& xis, MIL_Formation& formation );
    MIL_AgentPion& CreatePion( const MIL_AgentTypePion& type, MIL_Automate&  automate , xml::xistream& xis );
    MIL_AgentPion& CreatePion( const MIL_AgentTypePion& type, MIL_Automate& automate, const MT_Vector2D& vPosition, unsigned int nCtx );
    MIL_AgentPion& CreatePion( const MIL_AgentTypePion& type, MIL_Automate& automate, const MT_Vector2D& vPosition, const std::string& name, unsigned int nCtx );
    void CreateObject( xml::xistream& xis, MIL_Army_ABC* army );
    void CreateUrbanObject( const urban::TerrainObject_ABC& object );
    MIL_Object_ABC* CreateObject( MIL_Army_ABC* army, const std::string& type, const TER_Localisation* pLocalisation, sword::ObstacleType_DemolitionTargetType obstacleType, unsigned int externalIdentifier = 0u );
    MIL_Object_ABC* CreateObject( const std::string& type, MIL_Army_ABC* army, const TER_Localisation& localisation );
    MIL_Object_ABC* CreateObject( const std::string& type, MIL_Army_ABC* army, const TER_Localisation& localisation, unsigned int id );
    MIL_Object_ABC* CreateObject( MIL_Army_ABC* army, const MIL_ObjectBuilder_ABC& builder );
    MIL_Object_ABC* CreateObject( const std::string& type, MIL_Army_ABC* army, const TER_Localisation& localisation, const std::string& strOption, const std::string& strExtra, double rCompletion, double rMining, double rBypass );
    MIL_Population* CreateCrowd( const std::string& type, const MT_Vector2D& point, int number, const std::string& name, MIL_Army_ABC& army, UrbanObjectWrapper* pUrbanObject = 0 );
    //@}

    //! @name Accessors
    //@{
            MIL_AutomateLOG*    FindBrainLogistic( unsigned int nID ) const;
            MIL_AutomateLOG*    FindBrainLogistic( const sword::MissionParameter_Value& parameter ) const;
            MIL_AutomateLOG*    FindBrainLogistic( const sword::ParentEntity& parameter ) const;
    virtual MIL_Automate*       FindAutomate      ( unsigned int nID ) const;
    virtual MIL_Population*     FindPopulation    ( unsigned int nID ) const;
    virtual MIL_Inhabitant*     FindInhabitant    ( unsigned int nID ) const;
    virtual MIL_Formation*      FindFormation     ( unsigned int nID ) const;
    virtual MIL_KnowledgeGroup* FindKnowledgeGroup( unsigned int nID ) const; // LTO
    virtual MIL_KnowledgeGroup* FindKnowledgeGroupFromParents( unsigned int nID ); // LTO
    virtual MIL_AgentPion*      FindAgentPion     ( unsigned int nID ) const;
    virtual MIL_Object_ABC*     FindObject        ( unsigned int nID ) const;
    virtual const MIL_ObjectType_ABC& FindObjectType( const std::string& type ) const;

    MIL_Population* FindPopulation( UrbanObjectWrapper* urbanObject ) const;
    const tools::Resolver< MIL_Army_ABC >& MIL_EntityManager::GetArmies() const;
    UrbanObjectWrapper& GetUrbanObjectWrapper( const urban::TerrainObject_ABC& object );
    unsigned int ConvertUrbanIdToSimId( unsigned int urbanId );
    void Accept( KnowledgesVisitor_ABC& visitor ) const;
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
    unsigned long GetObjectsCount    () const;
    unsigned long GetAutomatsCount   () const;
    unsigned long GetAgentsCount     () const;
    unsigned long GetCrowdsCount     () const;
    //@}

    //! @name Operations
    //@{
    void ReadODB( const MIL_Config& config );
    void CreateUrbanObjects( urban::Model& urbanModel, const MIL_Config& config );
    void SendStateToNewClient() const;
    void Update();
    void Clean();
    void Finalize();
    //@}

    //! @external helper
    //@{
    void SetToTasker( sword::Tasker& tasker, unsigned int id ) const;
    //@}

    //! @name Network
    //@{
    void OnReceiveUnitMagicAction          ( const sword::UnitMagicAction&      message, unsigned int nCtx );
    void OnReceiveObjectMagicAction        ( const sword::ObjectMagicAction&    message, unsigned int nCtx );
    void OnReceiveUnitOrder                ( const sword::UnitOrder&            message, unsigned int nCtx );
    void OnReceiveAutomatOrder             ( const sword::AutomatOrder&         message, unsigned int nCtx );
    void OnReceiveCrowdOrder               ( const sword::CrowdOrder&           message, unsigned int nCtx );
    void OnReceiveFragOrder                ( const sword::FragOrder&            message, unsigned int nCtx );
    void OnReceiveSetAutomateMode          ( const sword::SetAutomatMode&       message, unsigned int nCtx );
    void OnReceiveUnitCreationRequest      ( const sword::UnitCreationRequest&  message, unsigned int nCtx );
    void OnReceiveKnowledgeMagicAction     ( const sword::KnowledgeMagicAction& message, unsigned int nCtx );
    void OnReceiveChangeDiplomacy          ( const sword::MagicAction&          message, unsigned int nCtx );
    void OnReceiveChangeResourceLinks      ( const sword::MagicAction&          message, unsigned int nCtx );
    void OnReceiveCreateFireOrderOnLocation( const sword::MagicAction&          message, unsigned int nCtx );
    void OnReceiveBurningCellRequest       ( const sword::BurningCellRequest&   message, unsigned int nCtx );
    // LTO begin
    void OnReceiveKnowledgeGroupCreation    ( const sword::MagicAction&         message, unsigned int nCtx );
    // LTO end
    //@}

    //! @name Population channeling
    //@{
    void ChannelPopulations( const TER_Localisation& localisation );
    void AlertInhabitants( const TER_Localisation& localisation );
    void UndoAlertInhabitants( const TER_Localisation& localisation );
    bool IsInhabitantsAlerted( const TER_Localisation& localisation );
    bool IsInhabitantsConfined( const TER_Localisation& localisation );
    void ConfineInhabitants( const TER_Localisation& localisation );
    void UndoConfineInhabitants( const TER_Localisation& localisation );
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
    void ProcessAutomateChangeKnowledgeGroup( const sword::UnitMagicAction&      message, unsigned int nCtx );
    void ProcessAutomateChangeSuperior      ( const sword::UnitMagicAction&      message, unsigned int nCtx );
    void ProcessChangeLogisticLinks         ( const sword::UnitMagicAction&      message, unsigned int nCtx );
    void ProcessUnitChangeSuperior          ( const sword::UnitMagicAction&      message, unsigned int nCtx );
    void ProcessLogSupplyChangeQuotas       ( const sword::UnitMagicAction&      message, unsigned int nCtx );
    void ProcessLogSupplyPushFlow           ( const sword::UnitMagicAction&      message, unsigned int nCtx );
    void ProcessLogSupplyPullFlow           ( const sword::UnitMagicAction&      message, unsigned int nCtx );
    void ProcessMagicActionMoveTo           ( const sword::UnitMagicAction&      message, unsigned int nCtx );
    // LTO begin
    void ProcessKnowledgeGroupUpdate        ( const sword::KnowledgeMagicAction& message, unsigned int nCtx );
    void ProcessMagicActionCreateFireOrder  ( const sword::UnitMagicAction&      message, unsigned int nCtx );
    // LTO end
    void ProcessAutomatCreationRequest      ( const sword::UnitMagicAction&      message, MIL_Entity_ABC& entity, unsigned int nCtx );
    void ProcessFormationCreationRequest    ( const sword::UnitMagicAction&      message, MIL_Army_ABC* army, MIL_Formation* formation, unsigned int nCtx );
    void ProcessCrowdCreationRequest        ( const sword::UnitMagicAction&      message, MIL_Army_ABC& army, unsigned int context );
    void ProcessTransferEquipmentRequest    ( const sword::UnitMagicAction&      message, MIL_AgentPion& pion );
    //@}

private:
    //! @name Init
    //@{
    void ReadOrbat          ( xml::xistream& xis );
    void InitializeArmies   ( xml::xistream& xis );
    void InitializeDiplomacy( xml::xistream& xis );
    void ReadDiplomacy      ( xml::xistream& xis );
    //@}

    //! @name Urban
    //@{
    void ReadUrbanStates( xml::xistream& xis );
    void NotifyPionsInsideUrbanObject();
    //@}

    //! @name Update
    //@{
    void UpdateKnowledges();
    void UpdateDecisions ();
    void UpdateActions();
    void UpdateEffects();
    void UpdateStates();
    void UpdateKnowledgeGroups();
    void PreprocessRandomBreakdowns();
    //@}

private:
    //! @name Member data
    //@{
    const MIL_Time_ABC& time_;
    MIL_EffectManager& effectManager_;

    // Profiling
    MIL_ProfilerMgr& profilerManager_;
    MT_Profiler profiler_;
    unsigned int nRandomBreakdownsNextTimeStep_;
    double rKnowledgesTime_;
    double rAutomatesDecisionTime_;
    double rPionsDecisionTime_;
    double rPopulationsDecisionTime_;
    double rActionsTime_;
    double rEffectsTime_;
    double rStatesTime_;

    std::auto_ptr< MIL_IDManager > idManager_;
    std::auto_ptr< MIL_ObjectManager > pObjectManager_;

    // Factories
    std::auto_ptr< PopulationFactory_ABC >     populationFactory_;
    std::auto_ptr< InhabitantFactory_ABC >     inhabitantFactory_;
    std::auto_ptr< MissionController_ABC >     missionController_;
    std::auto_ptr< AgentFactory_ABC >          agentFactory_;
    std::auto_ptr< AutomateFactory_ABC >       automateFactory_;
    std::auto_ptr< FormationFactory_ABC >      formationFactory_;
    std::auto_ptr< KnowledgeGroupFactory_ABC > knowledgeGroupFactory_;
    std::auto_ptr< ArmyFactory_ABC >           armyFactory_;
    unsigned int  gcPause_;
    unsigned int  gcMult_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( MIL_EntityManager )

#endif // __MIL_EntityManager_h_
