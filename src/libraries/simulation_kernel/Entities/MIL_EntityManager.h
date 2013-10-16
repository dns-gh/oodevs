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
#include "MIL_EntityManager_ABC.h"
#include "propagation/ElevationGetter_ABC.h"
#include <tools/Resolver.h>
#include <map>

namespace tools
{
    class PhyLoader;
}

namespace sword
{
    enum ObstacleType_DemolitionTargetType;
    enum MagicActionAck_ErrorCode;
    class CrowdOrder;
    class UnitOrder;
    class AutomatOrder;
    class Tasker;
    class FragOrder;
    class ObjectMagicAction;
    class ControlEnableVisionCones;
    class SetAutomatMode;
    class UnitCreationRequest;
    class UnitMagicAction;
    class UnitMagicActionAck;
    class KnowledgeMagicAction;
    class MagicAction;
    class MagicActionAck;
    class MissionParameter_Value;
    class BurningCellRequest;
    class ParentEntity;
    class Sink_ABC;
}

namespace client
{
    class MagicActionAck;
}

namespace xml
{
    class xostream;
    class xistream;
}

namespace propagation
{
    class FloodModel_ABC;
}

class ArmyFactory_ABC;
class AgentFactory_ABC;
class AutomateFactory_ABC;
class FormationFactory_ABC;
class KnowledgeGroupFactory;
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
class MIL_DictionaryExtensions;
class MIL_Formation;
class MIL_IDManager;
class MIL_KnowledgeGroup;
class MIL_Object_ABC;
class MIL_ObjectType_ABC;
class MIL_ObjectBuilder_ABC;
class MIL_ObjectFactory;
class MIL_UrbanObject_ABC;
class MIL_Population;
class MIL_Inhabitant;
class MIL_ProfilerManager;
class MIL_Time_ABC;
class MissionController_ABC;
class InhabitantFactory_ABC;
class PopulationFactory_ABC;
class TER_Localisation;
class MIL_UrbanObject_ABC;
class KnowledgesVisitor_ABC;
class MIL_DictionaryExtensions;

void TerminatePhysicalSingletons();
void TerminateMilitarySingletons();
void TerminateLogisticsSingletons();

// =============================================================================
// @class  MIL_EntityManager
// Created: JVT 2004-08-03
// =============================================================================
class MIL_EntityManager : public MIL_EntityManager_ABC,
                          private boost::noncopyable
{
public:
             MIL_EntityManager( const MIL_Time_ABC& time, MIL_EffectManager& effects, MIL_ObjectFactory& objectFactory, const MIL_Config& config );
    virtual ~MIL_EntityManager();

    static void Initialize( const tools::PhyLoader& loader, const MIL_Time_ABC& time,
        MIL_ObjectFactory& objectFactory );

    //! @name Factory
    //@{
    MIL_Automate& CreateAutomat( xml::xistream& xis, MIL_Entity_ABC&  parent );
    MIL_Automate& CreateAutomat( const MIL_AutomateType& type, unsigned int knowledgeGroup, const std::string& name, MIL_Entity_ABC& parent, unsigned int nCtx, const MIL_DictionaryExtensions& extensions );
    void CreateIntelligence( xml::xistream& xis, MIL_Formation& formation );
    MIL_AgentPion& CreatePion( const MIL_AgentTypePion& type, MIL_Automate&  automate , xml::xistream& xis );
    MIL_AgentPion& CreatePion( const MIL_AgentTypePion& type, MIL_Automate& automate, const MT_Vector2D& vPosition, unsigned int nCtx );
    MIL_AgentPion& CreatePion( const MIL_AgentTypePion& type, MIL_Automate& automate, const MT_Vector2D& vPosition, const std::string& name, unsigned int nCtx, const MIL_DictionaryExtensions& extensions );
    void CreateObject( xml::xistream& xis, MIL_Army_ABC* army );
    MIL_Object_ABC* CreateObject( MIL_Army_ABC* army, const std::string& type, const TER_Localisation* pLocalisation, bool activated, unsigned int externalIdentifier = 0u, const std::string& name = std::string(), double density = 0. );
    MIL_Object_ABC* CreateObject( const std::string& type, MIL_Army_ABC* army, const TER_Localisation& localisation );
    MIL_Object_ABC* CreateObject( const std::string& type, MIL_Army_ABC* army, const TER_Localisation& localisation, unsigned int id );
    MIL_Object_ABC* CreateObject( MIL_Army_ABC* army, const MIL_ObjectBuilder_ABC& builder );
    MIL_Population* CreateCrowd( const std::string& type, const MT_Vector2D& point, int number, const std::string& name, MIL_Army_ABC& army, MIL_UrbanObject_ABC* pUrbanObject = 0 );
    //@}

    //! @name Accessors
    //@{
            MIL_AutomateLOG*    FindBrainLogistic( unsigned int nID ) const;
            MIL_AutomateLOG*    FindBrainLogistic( const sword::MissionParameter_Value& parameter ) const;
            MIL_AutomateLOG*    FindBrainLogistic( const sword::ParentEntity& parameter ) const;
    virtual MIL_Automate*       FindAutomate     ( unsigned int nID ) const;
    virtual MIL_Population*     FindPopulation   ( unsigned int nID ) const;
    virtual MIL_Inhabitant*     FindInhabitant   ( unsigned int nID ) const;
    virtual MIL_Formation*      FindFormation    ( unsigned int nID ) const;
    virtual MIL_AgentPion*      FindAgentPion    ( unsigned int nID ) const;
    virtual MIL_Object_ABC*     FindObject       ( unsigned int nID ) const;

    void VisitUniversalObjects( const boost::function< void( MIL_Object_ABC& ) >& visitor ) const;

    MIL_Population* FindPopulation( MIL_UrbanObject_ABC* urbanObject ) const;
    const tools::Resolver< MIL_Army_ABC >& GetArmies() const;
    unsigned int ConvertUrbanIdToSimId( unsigned int urbanId );
    bool ConvertIdToUrbanId( unsigned int& id ) const;
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
    unsigned long GetModelCount      () const;
    //@}

    //! @name Operations
    //@{
    void ReadODB( const MIL_Config& config );
    void LoadUrbanModel( const MIL_Config& config );
    void SendStateToNewClient() const;
    void Update();
    void Clean();
    void CleanDeletedAgentKnowledges();
    void Finalize();
    void Synchronize();
    void LogInfo();
    //@}

    //! @external helper
    //@{
    void SetToTasker( sword::Tasker& tasker, unsigned int id ) const;
    //@}

    //! @name Network
    //@{
    void OnReceiveUnitMagicAction          ( const sword::UnitMagicAction&      message, unsigned int nCtx, unsigned int clientId );
    void OnReceiveObjectMagicAction        ( const sword::ObjectMagicAction&    message, unsigned int nCtx );
    void OnReceiveControlToggleVisionCones ( const sword::ControlEnableVisionCones& message );
    void OnReceiveUnitOrder                ( const sword::UnitOrder&            message, unsigned int nCtx, unsigned int clientId );
    void OnReceiveAutomatOrder             ( const sword::AutomatOrder&         message, unsigned int nCtx, unsigned int clientId );
    void OnReceiveCrowdOrder               ( const sword::CrowdOrder&           message, unsigned int nCtx, unsigned int clientId );
    void OnReceiveFragOrder                ( const sword::FragOrder&            message, unsigned int nCtx, unsigned int clientId );
    void OnReceiveSetAutomateMode          ( const sword::SetAutomatMode&       message, unsigned int nCtx, unsigned int clientId );
    void OnReceiveUnitCreationRequest      ( const sword::UnitCreationRequest&  message, unsigned int nCtx );
    void OnReceiveKnowledgeMagicAction     ( const sword::KnowledgeMagicAction& message, unsigned int nCtx, unsigned int clientId );
    void OnReceiveChangeDiplomacy          ( const sword::MagicAction&          message, unsigned int nCtx, unsigned int clientId );
    void OnReceiveChangeResourceLinks      ( const sword::MagicAction&          message, unsigned int nCtx, unsigned int clientId );
    void OnReceiveCreateFireOrderOnLocation( const sword::MagicAction&          message, unsigned int nCtx, unsigned int clientId );
    void OnReceiveBurningCellRequest       ( const sword::BurningCellRequest&   message, unsigned int nCtx );
    void OnReceiveKnowledgeGroupCreation   ( const sword::MagicAction&          message, sword::MagicActionAck& ack );
    //@}

    //! @name Population channeling
    //@{
    void ChannelPopulations( const TER_Localisation& localisation );
    void AlertInhabitants( const TER_Localisation& localisation );
    void UndoAlertInhabitants( const TER_Localisation& localisation );
    bool IsInhabitantsAlerted( const TER_Localisation& localisation );
    bool IsInhabitantsConfined( const TER_Localisation& localisation );
    bool IsInhabitantsEvacuated( const TER_Localisation& localisation );
    void ConfineInhabitants( const TER_Localisation& localisation );
    void UndoConfineInhabitants( const TER_Localisation& localisation );
    void EvacuateInhabitants( const TER_Localisation& localisation );
    void UndoEvacuateInhabitants( const TER_Localisation& localisation );
    //@}

    bool SendVisionCones() const;

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    MIL_EntityManager( const MIL_Time_ABC& time, MIL_EffectManager& effects, std::auto_ptr< sword::Sink_ABC > sink, const MIL_Config& config );

    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    template< typename Archive > friend  void save_construct_data( Archive& archive, const MIL_EntityManager* role, const unsigned int /*version*/ );
    template< typename Archive > friend  void load_construct_data( Archive& archive, MIL_EntityManager* role, const unsigned int /*version*/ );

    void WriteODB       ( xml::xostream& xos ) const;
    void WriteKnowledges( xml::xostream& xos ) const;
    void WriteUrban     ( xml::xostream& xos ) const;
    //@}

private:
    //! @name Helpers
    //@{
    boost::shared_ptr< MIL_KnowledgeGroup > FindKnowledgeGroup( unsigned int nID ) const;
    void Accept( KnowledgesVisitor_ABC& visitor ) const;

    void ProcessAutomateChangeKnowledgeGroup( const sword::UnitMagicAction&      message, unsigned int nCtx );
    void ProcessAutomateChangeSuperior      ( const sword::UnitMagicAction&      message, unsigned int nCtx );
    void ProcessChangeLogisticLinks         ( const sword::UnitMagicAction&      message );
    void ProcessUnitChangeSuperior          ( const sword::UnitMagicAction&      message, unsigned int taskerId );
    void ProcessLogSupplyChangeQuotas       ( const sword::UnitMagicAction&      message );
    void ProcessLogSupplyPushFlow           ( const sword::UnitMagicAction&      message );
    void ProcessLogSupplyPullFlow           ( const sword::UnitMagicAction&      message );
    void ProcessMagicActionMoveTo           ( const sword::UnitMagicAction&      message, unsigned int taskerId );
    void ProcessMagicActionCreateFireOrder  ( const sword::UnitMagicAction&      message );
    void ProcessAutomatCreationRequest      ( const sword::UnitMagicAction&      message, MIL_Entity_ABC& entity, unsigned int nCtx, sword::UnitMagicActionAck& ack );
    void ProcessFormationCreationRequest    ( const sword::UnitMagicAction&      message, unsigned int taskerId, unsigned int nCtx, sword::UnitMagicActionAck& ack );
    void ProcessCrowdCreationRequest        ( const sword::UnitMagicAction&      message, unsigned int parentId, unsigned int context, sword::UnitMagicActionAck& ack );
    void ProcessTransferEquipmentRequest    ( const sword::UnitMagicAction&      message, MIL_AgentPion& pion );
    void ProcessFormationChangeSuperior     ( const sword::UnitMagicAction&      message, unsigned int nCtx );
    void ProcessExecScript( const sword::UnitMagicAction& message, unsigned int brainOwnerId,
            sword::UnitMagicActionAck& ack );
    //@}

    //! @name Init
    //@{
    void ReadOrbat          ( xml::xistream& xis, const MIL_Config& config );
    void ReadUrban          ( xml::xistream& xis, std::vector< const MIL_UrbanObject_ABC* >& cities );
    void ReadCity           ( xml::xistream& xis, std::vector< const MIL_UrbanObject_ABC* >& cities );
    void ReadUrbanObject    ( xml::xistream& xis, MIL_UrbanObject_ABC& parent );
    void InitializeArmies   ( xml::xistream& xis, const MIL_Config& config );
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
    const unsigned int gcPause_;
    const unsigned int gcMult_;
    MIL_EffectManager& effectManager_;
    std::vector< const MIL_UrbanObject_ABC* > cities_;
    bool bSendUnitVisionCones_;

    // Profiling
    std::map< std::string, double > profilers_;
    std::auto_ptr< MIL_ProfilerManager > profilerManager_;
    unsigned int nRandomBreakdownsNextTimeStep_;
    double rKnowledgesTime_;
    double rAutomatesDecisionTime_;
    double rPionsDecisionTime_;
    double rPopulationsDecisionTime_;
    double rActionsTime_;
    double rEffectsTime_;
    double rStatesTime_;

    // Order is important here
    std::auto_ptr< MIL_IDManager >               idManager_;          // has to be declared before agentFactory & automatFactory
    std::auto_ptr< MissionController_ABC >       missionController_;  // has to be declared before populationFactory and agentFactory
    std::auto_ptr< PopulationFactory_ABC >       populationFactory_;      // has to be declared before armyFactory
    std::auto_ptr< InhabitantFactory_ABC >       inhabitantFactory_;      // has to be declared before armyFactory
    std::auto_ptr< AgentFactory_ABC >            agentFactory_;           // has to be declared before Sink
    std::auto_ptr< sword::Sink_ABC >             sink_;
    std::auto_ptr< MIL_ObjectManager >           pObjectManager_;
    std::auto_ptr< propagation::FloodModel_ABC > pFloodModel_;
    std::auto_ptr< AutomateFactory_ABC >         automateFactory_;        // has to be declared before armyFactory & formation factory
    std::auto_ptr< FormationFactory_ABC >        formationFactory_;       // has to be declared before armyFactory
    std::auto_ptr< KnowledgeGroupFactory >       knowledgeGroupFactory_;  // has to be declared before armyFactory
    std::auto_ptr< ArmyFactory_ABC >             armyFactory_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( MIL_EntityManager )

#endif // __MIL_EntityManager_h_
