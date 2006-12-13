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

class MIL_EffectManager;
class MIL_ObjectManager;
class MIL_Army;
class MIL_AgentPion;
class MIL_Automate;
class MIL_Formation;
class MIL_VirtualObject_ABC;
class MIL_AgentTypePion;
class MIL_RealObject_ABC;
class MIL_RealObjectType;
class MIL_Population;
class MIL_PopulationType;
class MIL_AutomateType;
class MIL_NbcAgentType;
class MIL_ControlZone;
class MIL_NuageNBC;
class MIL_ZoneMineeParDispersion;
class TER_Localisation;

class HLA_Federate;

struct ASN1T_MsgUnitMagicAction;
struct ASN1T_MsgPionOrder;
struct ASN1T_MsgAutomateOrder;
struct ASN1T_MsgSetAutomateMode;
struct ASN1T_MsgOrderConduite;
struct ASN1T_MsgObjectMagicAction;
struct ASN1T_MsgPopulationMagicAction;
struct ASN1T_MsgChangeDiplomatie;
struct ASN1T_MsgChangeGroupeConnaissance;
struct ASN1T_MsgChangeLiensLogistiques;
struct ASN1T_MsgChangeAutomate;
struct ASN1T_MsgLogRavitaillementChangeQuotas;
struct ASN1T_MsgLogRavitaillementPousserFlux;

class DIA_Parameters;

namespace DIN
{
    class DIN_Input;
};

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
     MIL_EntityManager( MIL_InputArchive& );
     MIL_EntityManager();
    ~MIL_EntityManager();

    static void Initialize( MIL_InputArchive& archive );

    //! @name Factory
    //@{
    MIL_Population& CreatePopulation( const MIL_PopulationType& type, uint nID, MIL_Army& army, MIL_InputArchive& archive );
    MIL_Formation&  CreateFormation ( uint nID, MIL_Army& army, MIL_InputArchive& archive, MIL_Formation* pParent = 0 );
    MIL_Automate&   CreateAutomate  ( const MIL_AutomateType&  type, uint nID, MIL_Formation& formation, MIL_InputArchive& archive );
    MIL_AgentPion&  CreatePion      ( const MIL_AgentTypePion& type, uint nID, MIL_Automate&  automate , MIL_InputArchive& archive );
    MIL_AgentPion&  CreatePion      ( const MIL_AgentTypePion& type, MIL_Automate& automate, const MT_Vector2D& vPosition );

    MIL_RealObject_ABC&         CreateObject                       ( const MIL_RealObjectType& type, uint nID, MIL_Army& army, MIL_InputArchive& archive ); 
    MIL_RealObject_ABC*         CreateObject                       ( MIL_Army& army, DIA_Parameters& diaParameters, uint nCurrentParamIdx );
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
    void ReadODB             ( MIL_InputArchive& archive );
    void SendStateToNewClient() const;
    void Update              ();
    void Clean               ();

    void RegisterLocalAgents( HLA_Federate& federate ) const;
    //@}

    //! @name Network
    //@{
    void OnReceiveMsgUnitMagicAction      ( DIN::DIN_Input&                         msg );
    void OnReceiveMsgUnitMagicAction      ( ASN1T_MsgUnitMagicAction&               msg, uint nCtx );
    void OnReceiveMsgPopulationOrder      ( ASN1T_MsgPopulationOrder&               msg, uint nCtx );     
    void OnReceiveMsgPionOrder            ( ASN1T_MsgPionOrder&                     msg, uint nCtx ); 
    void OnReceiveMsgAutomateOrder        ( ASN1T_MsgAutomateOrder&                 msg, uint nCtx );
    void OnReceiveMsgSetAutomateMode      ( ASN1T_MsgSetAutomateMode&               msg, uint nCtx );
    void OnReceiveMsgFragOrder            ( ASN1T_MsgFragOrder&                     msg, uint nCtx );
    void OnReceiveMsgObjectMagicAction    ( ASN1T_MsgObjectMagicAction&             msg, uint nCtx );
    void OnReceiveMsgPopulationMagicAction( ASN1T_MsgPopulationMagicAction&         msg, uint nCtx );
    void OnReceiveMsgChangeDiplomacy      ( ASN1T_MsgChangeDiplomatie&              msg, uint nCtx );
    void OnReceiveMsgChangeKnowledgeGroup ( ASN1T_MsgChangeGroupeConnaissance&      msg, uint nCtx );
    void OnReceiveMsgChangeLogisticLinks  ( ASN1T_MsgChangeLiensLogistiques&        msg, uint nCtx );
    void OnReceiveMsgChangeAutomate       ( ASN1T_MsgChangeAutomate&                msg, uint nCtx );
    void OnReceiveMsgLogSupplyChangeQuotas( ASN1T_MsgLogRavitaillementChangeQuotas& msg, uint nCtx );
    void OnReceiveMsgLogSupplyPushFlow    ( ASN1T_MsgLogRavitaillementPousserFlux&  msg, uint nCtx );
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

    void WriteODB( MT_XXmlOutputArchive& archive ) const;
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
    
    typedef std::vector< MIL_AgentPion* >  T_PionVector;
    typedef T_PionVector::reverse_iterator RIT_PionVector;
    //@}

private:
    //! @name Init
    //@{
    // Types
    template < typename T > 
    static void InitializeType   ( MIL_InputArchive& archive, const std::string& strSection );
    static void InitializeMedical( MIL_InputArchive& archive );
    static void InitializeSensors( MIL_InputArchive& archive );

    // ODB
    void InitializeArmies     ( MIL_InputArchive& archive );
    void InitializeDiplomacy  ( MIL_InputArchive& archive );
    void InitializePopulations( MIL_InputArchive& archive );
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
    MIL_EffectManager& effectManager_;
    MIL_ObjectManager* pObjectManager_;

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

