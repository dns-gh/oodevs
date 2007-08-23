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

namespace xml
{
    class xostream;
    class xistream;
}

namespace DIN
{
    class DIN_Input;
}

class MIL_EffectManager;
class MIL_ObjectManager;
class MIL_Army;
class MIL_AgentPion;
class MIL_Automate;
class MIL_Formation;
class MIL_AgentTypePion;
class MIL_RealObject_ABC;
class MIL_RealObjectType;
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

class DIA_Parameters;

// =============================================================================
// @class  MIL_EntityManager
// Created: JVT 2004-08-03
// =============================================================================
class MIL_EntityManager
{
    MT_COPYNOTALLOWED( MIL_EntityManager )

public:
    //! @name Types
    //@{
    typedef std::map< uint, MIL_Army*> T_ArmyMap;
    typedef T_ArmyMap::const_iterator  CIT_ArmyMap;
    //@}

public:
             MIL_EntityManager( const MIL_Time_ABC& time, MIL_EffectManager& effects, MIL_ProfilerMgr& profiler, HLA_Federate* hla );
             MIL_EntityManager();
    virtual ~MIL_EntityManager();

    static void Initialize( MIL_Config& config, const MIL_Time_ABC& time, MIL_EffectManager& effects );

    //! @name Factory
    //@{
    void CreateFormation ( xml::xistream& xis, MIL_Army& army, MIL_Formation* parent = 0 );
    void CreateAutomat   ( xml::xistream& xis, MIL_Formation& formation );
    void CreatePopulation( xml::xistream& xis, MIL_Army& army );
    MIL_AgentPion&  CreatePion      ( const MIL_AgentTypePion& type, uint nID, MIL_Automate&  automate , xml::xistream& xis );
    MIL_AgentPion&  CreatePion      ( const MIL_AgentTypePion& type, MIL_Automate& automate, const MT_Vector2D& vPosition );

    void                        CreateObject                       ( xml::xistream& xis, MIL_Army& army ); 
    MIL_RealObject_ABC*         CreateObject                       ( MIL_Army& army, const MIL_ObstacleType& obstacleType, DIA_Parameters& diaParameters, uint nCurrentParamIdx );
    MIL_RealObject_ABC*         CreateObject                       ( const MIL_RealObjectType& type, MIL_Army& army, const TER_Localisation& localisation, const std::string& strOption, const std::string& strExtra, double rCompletion, double rMining, double rBypass );
    MIL_NuageNBC&               CreateObjectNuageNBC               ( MIL_Army& army, const TER_Localisation& localisation, const MIL_NbcAgentType& nbcAgentType );
    MIL_ZoneMineeParDispersion& CreateObjectZoneeMineeParDispersion( MIL_Army& army, const TER_Localisation& localisation, uint nNbrMines );
    MIL_ControlZone&            CreateObjectControlZone            ( MIL_Army& army, const TER_Localisation& localisation, MT_Float rRadius );
    //@}

    //! @name Accessors
    //@{
          MIL_Army*           FindArmy      ( const std::string& strName ) const;
          MIL_Army*           FindArmy      ( uint nID ) const;
          MIL_Formation*      FindFormation ( uint nID ) const;
          MIL_Automate*       FindAutomate  ( uint nID ) const;
          MIL_Population*     FindPopulation( uint nID ) const;
          MIL_AgentPion*      FindAgentPion ( uint nID ) const;
          MIL_RealObject_ABC* FindObject    ( uint nID ) const;
    const T_ArmyMap&          GetArmies     () const;
    
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

    void RegisterLocalAgents( HLA_Federate& federate ) const;
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
    void OnReceiveMsgUnitChangeSuperior          ( const ASN1T_MsgUnitChangeSuperior&               msg, uint nCtx );
    void OnReceiveMsgLogSupplyChangeQuotas       ( const ASN1T_MsgLogSupplyChangeQuotas&            msg, uint nCtx );
    void OnReceiveMsgLogSupplyPushFlow           ( const ASN1T_MsgLogSupplyPushFlow&                msg, uint nCtx );
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

    void WriteODB( xml::xostream& xos ) const;
    //@}
    
private:
    //! @name Types
    //@{
    typedef std::map< uint, MIL_AgentPion* > T_PionMap;
    typedef T_PionMap::const_iterator        CIT_PionMap;

    typedef std::map< uint, MIL_Automate* > T_AutomateMap;
    typedef T_AutomateMap::const_iterator   CIT_AutomateMap;

    typedef std::map< uint, MIL_Formation* > T_FormationMap;
    typedef T_FormationMap::const_iterator   CIT_FormationMap;

    typedef std::map< uint, MIL_Population* > T_PopulationMap;
    typedef T_PopulationMap::const_iterator   CIT_PopulationMap;
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
    void ReadArmy             ( xml::xistream& xis );
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

    T_ArmyMap       armies_;
    T_FormationMap  formations_;
    T_PionMap       pions_;
    T_AutomateMap   automates_;



    T_PopulationMap populations_;

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
};

#include "MIL_EntityManager.inl"

#endif // __MIL_EntityManager_h_

