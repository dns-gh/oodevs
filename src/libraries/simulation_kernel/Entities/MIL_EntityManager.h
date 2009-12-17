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

#include "tools/Resolver.h"

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
class MIL_PopulationType;
class MIL_ObstacleType;
class MIL_AutomateType;
class MIL_NbcAgentType;
class MIL_ControlZone;
class MIL_NuageNBC;
class MIL_ZoneMineeParDispersion;
class TER_Localisation;
class MIL_Config;
class MIL_ProfilerMgr;
class MIL_Time_ABC;
class MIL_Intelligence;
class MIL_IDManager;
class PopulationFactory_ABC;


class HLA_Federate;

struct ASN1T_MsgUnitMagicAction;
struct ASN1T_MsgUnitOrder;
struct ASN1T_MsgAutomatOrder;
struct ASN1T_MsgSetAutomatMode;
struct ASN1T_MsgOrderConduite;
struct ASN1T_MsgObjectMagicAction;
struct ASN1T_MsgPopulationMagicAction;
struct ASN1T_MsgChangeDiplomacy;
struct ASN1T_MsgAutomatChangeKnowledgeGroup;
struct ASN1T_MsgAutomatChangeLogisticLinks;
struct ASN1T_MsgUnitChangeSuperior;
struct ASN1T_MsgLogSupplyChangeQuotas;
struct ASN1T_MsgLogSupplyPushFlow;

class ASN1T_EnumObjectType;

// =============================================================================
// @class  MIL_EntityManager
// Created: JVT 2004-08-03
// =============================================================================
class MIL_EntityManager : public MIL_EntityManager_ABC,
                          public tools::Resolver< MIL_AgentPion >,
                          private boost::noncopyable
{

public:
             MIL_EntityManager( const MIL_Time_ABC& time, MIL_EffectManager& effects, MIL_ProfilerMgr& profiler, HLA_Federate* hla, DEC_DataBase& database );
    virtual ~MIL_EntityManager();

    static void Initialize( MIL_Config& config, const MIL_Time_ABC& time, MIL_EffectManager& effects );
    static MIL_EntityManager& GetSingleton();

    //! @name Factory
    //@{
    void CreateAutomat     ( xml::xistream& xis, MIL_Automate&  parent );
    void CreateAutomat     ( xml::xistream& xis, MIL_Formation& formation );
    void CreateIntelligence( xml::xistream& xis, MIL_Formation& formation );
    MIL_AgentPion&  CreatePion( const MIL_AgentTypePion& type, MIL_Automate&  automate , xml::xistream& xis );
    MIL_AgentPion&  CreatePion( const MIL_AgentTypePion& type, MIL_Automate& automate, const MT_Vector2D& vPosition );

    void                        CreateObject( xml::xistream& xis, MIL_Army_ABC& army ); 
    MIL_Object_ABC*             CreateObject( MIL_Army_ABC& army, const std::string& type, const TER_Localisation* pLocalisation, ASN1T_EnumDemolitionTargetType obstacleType );    
    MIL_Object_ABC*             CreateObject( const std::string& type, MIL_Army_ABC& army, const TER_Localisation& localisation );
    MIL_Object_ABC*             CreateObject( MIL_Army_ABC& army, const MIL_ObjectBuilder_ABC& builder );
    MIL_Object_ABC*             CreateObject( const std::string& type, MIL_Army_ABC& army, const TER_Localisation& localisation, const std::string& strOption, const std::string& strExtra, double rCompletion, double rMining, double rBypass );        
    //@}

    //! @name Accessors
    //@{
    virtual       MIL_KnowledgeGroup* FindKnowledgeGroup( uint nID ) const;
    virtual       MIL_Automate*       FindAutomate  ( uint nID ) const;
    virtual       MIL_AgentPion*      FindAgentPion ( uint nID ) const;
                  MIL_Object_ABC*     FindObject    ( uint nID ) const;
    virtual const MIL_ObjectType_ABC& FindObjectType( const std::string& type ) const;
        
    const tools::Resolver< MIL_Army >& MIL_EntityManager::GetArmies() const;
    
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
    void OnReceiveMsgUnitMagicAction             ( const ASN1T_MsgUnitMagicAction&                  msg, uint nCtx );
    void OnReceiveMsgPopulationOrder             ( const ASN1T_MsgPopulationOrder&                  msg, uint nCtx );     
    void OnReceiveMsgUnitOrder                   ( const ASN1T_MsgUnitOrder&                        msg, uint nCtx ); 
    void OnReceiveMsgAutomatOrder                ( const ASN1T_MsgAutomatOrder&                     msg, uint nCtx );
    void OnReceiveMsgSetAutomateMode             ( const ASN1T_MsgSetAutomatMode&                   msg, uint nCtx );
    void OnReceiveMsgUnitCreationRequest         ( const ASN1T_MsgUnitCreationRequest&              msg, uint nCtx );
    void OnReceiveMsgFragOrder                   ( const ASN1T_MsgFragOrder&                        msg, uint nCtx );
    void OnReceiveMsgObjectMagicAction           ( const ASN1T_MsgObjectMagicAction&                msg, uint nCtx );
    void OnReceiveMsgPopulationMagicAction       ( const ASN1T_MsgPopulationMagicAction&            msg, uint nCtx );
    void OnReceiveMsgChangeDiplomacy             ( const ASN1T_MsgChangeDiplomacy&                  msg, uint nCtx );
    void OnReceiveMsgAutomateChangeKnowledgeGroup( const ASN1T_MsgAutomatChangeKnowledgeGroup&      msg, uint nCtx );
    void OnReceiveMsgAutomateChangeLogisticLinks ( const ASN1T_MsgAutomatChangeLogisticLinks&       msg, uint nCtx );
    void OnReceiveMsgAutomateChangeSuperior      ( const ASN1T_MsgAutomatChangeSuperior&            msg, uint nCtx );
    void OnReceiveMsgUnitChangeSuperior          ( const ASN1T_MsgUnitChangeSuperior&               msg, uint nCtx );
    void OnReceiveMsgLogSupplyChangeQuotas       ( const ASN1T_MsgLogSupplyChangeQuotas&            msg, uint nCtx );
    void OnReceiveMsgLogSupplyPushFlow           ( const ASN1T_MsgLogSupplyPushFlow&                msg, uint nCtx );
    void OnReceiveMsgKnowledgeGroupChangeSuperior( const ASN1T_MsgKnowledgeGroupChangeSuperior&     msg, uint nCtx );
    void OnReceiveMsgKnowledgeGroupDelete        ( const ASN1T_MsgKnowledgeGroupDelete&             msg, uint nCtx );
    void OnReceiveMsgKnowledgeGroupSetType       ( const ASN1T_MsgKnowledgeGroupSetType&            msg, uint nCtx );
    //@}

    //! @name Population channeling
    //@{
    void ChannelPopulations( const TER_Localisation& localisation );
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;
    template< typename Archive > friend  void save_construct_data( Archive& archive, const MIL_EntityManager* role, const unsigned int /*version*/ );
    template< typename Archive > friend  void load_construct_data( Archive& archive, MIL_EntityManager* role, const unsigned int /*version*/ );


    void WriteODB( xml::xostream& xos ) const;
    //@}
    
private:

    //! @name types
    //@{
    typedef std::map< uint, MIL_Intelligence* > T_IntelligenceMap;
    typedef T_IntelligenceMap::const_iterator   CIT_IntelligenceMap;
    //@}

private:
    //! @name Init
    //@{
    // Types
    template < typename T > 
    static void InitializeType       ( xml::xistream& xis, MIL_Config& config, const std::string& strSection );
    static void InitializeMedical    ( xml::xistream& xis, MIL_Config& config );
    static void InitializeComposantes( xml::xistream& xis, MIL_Config& config, const MIL_Time_ABC& time );
    static void InitializeWeapons    ( xml::xistream& xis, MIL_Config& config, const MIL_Time_ABC& time, MIL_EffectManager& effects );
    static void InitializeSensors    ( xml::xistream& xis, MIL_Config& config, const MIL_Time_ABC& time );

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
    std::auto_ptr< PopulationFactory_ABC > populationFactory_;
    std::auto_ptr< AgentFactory_ABC >  agentFactory_;
    std::auto_ptr< AutomateFactory_ABC > automateFactory_;
    std::auto_ptr< FormationFactory_ABC > formationFactory_;
    std::auto_ptr< KnowledgeGroupFactory_ABC > knowledgeGroupFactory_;
    std::auto_ptr< ArmyFactory_ABC > armyFactory_;


    // Profiling
    MT_Profiler   profiler_;
    MT_Float      rKnowledgesTime_;
    MT_Float      rAutomatesDecisionTime_;
    MT_Float      rPionsDecisionTime_;    
    MT_Float      rPopulationsDecisionTime_;
    MT_Float      rActionsTime_;
    MT_Float      rEffectsTime_;
    MT_Float      rStatesTime_;

    uint          nRandomBreakdownsNextTimeStep_;

private:
    //! @name Singleton
    //@{
    static MIL_EntityManager* singleton_;
    //@}
};


#endif // __MIL_EntityManager_h_

