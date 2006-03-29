//*****************************************************************************
// 
// $Created: NLD 2002-07-15 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/Agent.h $
// $Author: Nld $
// $Modtime: 8/07/05 15:18 $
// $Revision: 25 $
// $Workfile: Agent.h $
// 
//*****************************************************************************

#ifndef __Agent_h_
#define __Agent_h_

#include "Types.h"
#include "Attr_Def.h"
#include "Order_Def.h"
#include "Perception_Def.h"
#include "RawVisionData.h"
#include "ASN_Types.h"
#include "AgentComposition.h"
#include "IDManager.h"

#include "MT_Tools/MT_Quad.h"

#include "Agent_ABC.h"

class ObjectKnowledge;
class AgentKnowledge;
class AgentModel;
class Gtia;
class Team;
class Object_ABC;
class Surface;
class TypePion;
class TypeAutomate;
class Experience;
class Tiredness;
class Morale;
class LogSupplyConsign;
class LogMedicalConsign;
class LogMaintenanceConsign;
class PopulationConcentration;
class PopulationFlow;
class Population;

// =============================================================================
// Created: APE 2004-07-19
// =============================================================================
class Agent : public Agent_ABC
{
    MT_COPYNOTALLOWED( Agent );
    friend class GLTool;

public:
    //-------------------------------------------------------------------------
    /** @name Types */
    //-------------------------------------------------------------------------
    //@{
    typedef std::vector< Agent* >      T_AgentVector;
    typedef T_AgentVector::iterator        IT_AgentVector;
    typedef T_AgentVector::const_iterator  CIT_AgentVector;

    typedef std::set< const Agent* >        T_AgentConstPtrSet;
    typedef T_AgentConstPtrSet::iterator        IT_AgentConstPtrSet;
    typedef T_AgentConstPtrSet::const_iterator  CIT_AgentConstPtrSet;

    typedef std::set< const Object_ABC* >     T_ObjectConstPtrSet;
    typedef T_ObjectConstPtrSet::iterator         IT_ObjectConstPtrSet;
    typedef T_ObjectConstPtrSet::const_iterator   CIT_ObjectConstPtrSet;

    typedef std::set< const PopulationConcentration* > T_ConcentrationSet;
    typedef T_ConcentrationSet::iterator                   IT_ConcentrationSet;
    typedef T_ConcentrationSet::const_iterator             CIT_ConcentrationSet;

    typedef std::set< const PopulationFlow* > T_FlowSet;
    typedef T_FlowSet::iterator                   IT_FlowSet;
    typedef T_FlowSet::const_iterator             CIT_FlowSet;

    typedef std::map< const PopulationFlow*, T_PointVector > T_FlowVisionMap;
    typedef T_FlowVisionMap::iterator                            IT_FlowVisionMap;
    typedef T_FlowVisionMap::const_iterator                      CIT_FlowVisionMap;

    struct sPopulationCollision
    {
        T_ConcentrationSet concentrations_;
        T_FlowSet          flows_;
    };

    typedef std::map< const Population*, sPopulationCollision > T_PopulationCollisionMap;
    typedef T_PopulationCollisionMap::const_iterator               CIT_PopulationCollisionMap;

    typedef MT_Quad< MT_Float, MT_Vector2D, MT_Vector2D, MT_Float > T_VisionConeQuad;
    typedef std::vector< T_VisionConeQuad >                         T_VisionConeVector;
    typedef T_VisionConeVector::iterator                            IT_VisionConeVector;
    typedef T_VisionConeVector::const_iterator                      CIT_VisionConeVector;

    typedef std::vector< uint >                 T_NbcAgentVector;
    typedef T_NbcAgentVector::const_iterator    CIT_NbcAgentVector;

    typedef std::vector< uint >               T_AgentIdVector;
    typedef T_AgentIdVector::const_iterator   CIT_AgentIdVector;

    typedef std::vector< Surface* >     T_SurfaceVector;
    typedef T_SurfaceVector::const_iterator CIT_SurfaceVector;

    typedef std::vector< uint >           T_Priorities;
    typedef T_Priorities::const_iterator  CIT_Priorities;
    //@}

public:
    explicit Agent( bool bGenerateId = false );
    explicit Agent( const ASN1T_MsgAutomateCreation& asnMsg );
    explicit Agent( const ASN1T_MsgPionCreation& asnMsg );
    ~Agent();

    void Initialize();

    //! @name Accessors
    //@{
    const AgentModel*       GetModelPion       () const;
    const AgentModel*       GetModelAutomate   () const;
    bool                        IsAutomate         () const;
    bool                        IsEmbraye          () const;
    const std::string           GetCategory        () const;
	const MIL_AgentID           GetID              () const;
    const MT_Vector2D&          GetPos             () const;
    virtual const MT_Vector2D   GetAggregatedPos   () const;
    const std::string&          GetSymbol          () const;
    Agent*                  GetParent          () const;
    Team&                   GetTeam            () const;
    Gtia&                   GetGtia            () const;
    const std::string           GetName            () const;
    int                         GetSpeed           () const;
    int                         GetDirection       () const;
    int                         GetAltitude        () const;
    E_UnitPosture               GetOldStance       () const;
    E_UnitPosture               GetStance          () const;
    int                         GetStanceCompletion() const;
    bool                        IsDead             () const;
    bool                        IsNeutralized      () const;
    uint                        GetRawOpState      () const;
    const T_AgentIdVector&      GetReinforcements  () const; 
    uint                        GetReinforced      () const;
    const T_AgentIdVector&      GetTransportees    () const; 
    uint                        GetTransporter     () const;
    int                         GetContamination   () const;
    bool                        IsOutOfGas         () const;
    const Experience&       GetExperience      () const;
    const Tiredness&        GetTiredness       () const;
    const Morale&           GetMorale          () const;

    bool                        IsLoaded                 () const;
    bool                        AreHumanTransportersReady() const;
    bool                        IsNBSSuitOn              () const;
    const T_NbcAgentVector&     GetContaminatingNBCAgents() const;

    const AgentComposition& GetComposition () const;
    const TypePion*         GetTypePion    () const;
	const TypeAutomate*		GetTypeAutomate() const;
    T_AgentVector&              GetChildren    ();

    AgentKnowledge*         FindAgentKnowledge( uint nId );
    //@}

    //! @name Modifiers
    //@{
    void AddChild     ( Agent&         agent       );
    void SetName      ( const std::string& strName     );
    void SetAgentID   ( MIL_AgentID        nID         );
    void SetGtia      ( Gtia&          gtia        );
    void SetParent    ( Agent*         pParent     );
    void SetCategory  ( const std::string& strCategory );
    void SetSymbol    ( const std::string& strSymbol   );
    void SetPos       ( const MT_Vector2D& vPos        );
    void SetIsAutomate( bool               b           );
    void SetEmbraye   ( bool               b           );
    //@}

    //! @name Network: Attributes update
    //@{
    void OnReceiveMsgObjectInterVisibility                 ( DIN::DIN_Input& input );
    void OnReceiveMsgUnitInterVisibility                   ( DIN::DIN_Input& input );
    void OnReceiveMsgPopulationConcentrationInterVisibility( DIN::DIN_Input& input );
    void OnReceiveMsgPopulationFlowInterVisibility         ( DIN::DIN_Input& input );
    void OnReceiveMsgPopulationCollision                   ( DIN::DIN_Input& input );

    void OnAttributeUpdated    ( const ASN1T_MsgUnitAttributes&            asnMsg );
    void OnAttributeUpdated    ( const ASN1T_MsgUnitDotations&             asnMsg );
    void OnLogisticLinksUpdated( const ASN1T_MsgChangeLiensLogistiquesAck& asnMsg );

    void OnSuperiorChanged( Agent& superior );
    void OnSuperiorChanged( Gtia& superior );
    //@}

    //! @name Vision
    //@{
    void GetVisionCones         ( T_VisionConeVector& res ) const;
    void GetVisionSurfaces      ( T_VisionResultMap& res );
    void OnReceiveMsgVisionCones( DIN::DIN_Input& input );
    MT_Float GetElongationFactor() const;

    MT_Float GetPopulationCollisionDensity() const;
    //@}

    //! @name PathFind
    //@{
    void UpdatePathFind      ();
    void OnReceiveMsgPathFind( const ASN1T_MsgUnitPathFind& asnMsg );
    void ClearOldPath        ();
    void ClearPath           ();
    //@}

    //! @name Logistique
    //@{
    void AddConsign   ( LogSupplyConsign& consign );
    void RemoveConsign( LogSupplyConsign& consign );

    void AddConsign   ( LogMaintenanceConsign& consign );
    void RemoveConsign( LogMaintenanceConsign& consign );

    void AddConsign   ( LogMedicalConsign& consign );
    void RemoveConsign( LogMedicalConsign& consign );

    void HandleConsign   ( LogSupplyConsign& consign );
    void TerminateConsign( LogSupplyConsign& consign );

    void HandleConsign   ( LogMaintenanceConsign& consign );
    void TerminateConsign( LogMaintenanceConsign& consign );

    void HandleConsign   ( LogMedicalConsign& consign );
    void TerminateConsign( LogMedicalConsign& consign );

    void OnReceiveMsgLogMaintenanceEtat( const ASN1T_MsgLogMaintenanceEtat& asn );
    void OnReceiveMsgLogMedicalEtat( const ASN1T_MsgLogSanteEtat& asn );
    void OnReceiveMsgLogSupplyEtat  ( const ASN1T_MsgLogRavitaillementEtat& asn );
    void OnReceiveMsgLogSupplyQuotas( const ASN1T_MsgLogRavitaillementQuotas& asnMsg );

    void InitializeStocks();

    // Accessor
    bool Agent::IsLogisticTC2           () const;
    bool Agent::IsLogisticBLD           () const;
    bool Agent::IsLogisticBLT           () const;
    bool Agent::IsLogisticMaintenance   () const;
    bool Agent::IsLogisticSante         () const;
    bool Agent::IsLogisticRavitaillement() const;
    //@}

    //! @name ODB
    //@{
    void ReadODB( InputArchive& archive, bool bAutomate );
    void WriteODB( MT_XXmlOutputArchive& archive );
    //@}
	//aggregation
    bool  IsAggregated() const;
    void SetAggregation( bool bAgg );

public:
    //! @name Types
    //@{
    typedef std::set< LogSupplyConsign* > T_SupplyConsigns;
    typedef T_SupplyConsigns::iterator        IT_SupplyConsigns;

    typedef std::set< LogMaintenanceConsign* > T_MaintenanceConsigns;
    typedef T_MaintenanceConsigns::iterator        IT_MaintenanceConsigns;

    typedef std::set< LogMedicalConsign* > T_MedicalConsigns;
    typedef T_MedicalConsigns::iterator        IT_MedicalConsigns;

    struct T_LogisticAvailability
    {
        uint nEquipmentType_;
        uint nNbrTotal_;
        uint nNbrAvailable_;
        uint nNbrWorking_;
        uint nNbrResting_; // std::numeric_limits< uint >::max() quand pas applicable
    };

    typedef std::vector< T_LogisticAvailability >    T_LogisticAvailabilities;
    typedef T_LogisticAvailabilities::const_iterator CIT_LogisticAvailabilities;

    typedef std::vector< std::pair< uint, uint > > T_LogisticStockAvailabilities;
    typedef T_LogisticStockAvailabilities::const_iterator CIT_LogisticStockAvailabilities;
    //@}

//$$$$$ Foutre tout ce binz en public ou utiliser des friend pour eviter d'avoir 34846 fonctions set/get
//$$$$$ qui puent.
public:
    MIL_AgentID					nAgentID_;
    std::string					sName_;
    std::string					symbolName_;
    std::string					levelSymbolName_;
    std::string					strCategory_;
    const TypePion*			pTypePion_;
    const TypeAutomate*		pTypeAutomate_;
    Gtia*					pGtia_;
    Agent*					pParent_;
    T_AgentVector				children_;
    MT_Vector2D					vPos_;
    bool						bAutomate_;
    bool						bEmbraye_;
    bool                        bAggregated_;
    int							nAltitude_;
    int							nDirection_;

    T_PointVector				pathFindVector_;
    T_PointVector				oldPathVector_;

    T_AgentConstPtrSet			identifiedAgents_;
    T_AgentConstPtrSet			recognizedAgents_;
    T_AgentConstPtrSet			detectedAgents_;
    T_AgentConstPtrSet          recordedAgents_;

    T_ObjectConstPtrSet			objectsPerceived_;
    T_ObjectConstPtrSet			objectsPerceptionInProgress_;

    T_ConcentrationSet          concentrationsPerceived_;
    T_FlowVisionMap             flowsPerceived_;

    T_PopulationCollisionMap    populationCollisions_;
    
    T_SurfaceVector				surfaces_;
    T_VisionResultMap			surfaceVision_;
    MT_Float					rElongationFactor_;
    bool						bVisionSurfacesNeedUpdating_;

    unsigned int				nSpeed_;

    E_UnitPosture				nOldPosture_;
    E_UnitPosture				nCurrentPosture_;
    uint						nPostureCompletionPourcentage_;

    bool						bLoadingState_;
    bool						bStealthModeEnabled_;

    AgentComposition		composition_;

    // State
    uint                    nRawOpState_;
    bool                    bDead_;
    bool                    bNeutralized_;

    // Etat decisionnel
    E_EtatRapFor            nFightRateState_;
    E_Roe                   nRulesOfEngagementState_;
    E_RoePopulation         nRulesOfEngagementPopulationState_;
    E_EtatCombatRencontre   nCloseCombatState_;
    E_EtatOperationnel      nOpState_;
    E_DisponibiliteAuTir    nIndirectFireAvailability_;
    
    // NBC
    bool                bNbcProtectionSuitWorn_;
    T_NbcAgentVector    contaminatingNbcAgents_;
    int                 nContamination_;

    // Renforts
    T_AgentIdVector renforts_;
    uint            nPionRenforce_;

    // Transport
    T_AgentIdVector pionTransportes_;
    uint            nTransporteur_;
    bool            bHumanTransportersReady_;

    // Human Stuff
    const Experience* pExperience_;
    const Tiredness*  pTiredness_;
    const Morale*     pMorale_;

    // Logistic Stuff
    T_SupplyConsigns        requestedSupplies_;
    T_SupplyConsigns        handledSupplies_;
    typedef struct
    {
        bool                     bChainEnabled_;
        T_LogisticAvailabilities dispoTransporters_;
        T_LogisticAvailabilities dispoCommanders_;  
        T_ResourceQty_Map             stocks_;
        T_LogisticStockAvailabilities quotas_;
    } T_SupplyData;
    T_SupplyData* pSupplyData_;

    T_MaintenanceConsigns   requestedMaintenances_;
    T_MaintenanceConsigns   handledMaintenances_;
    typedef struct
    {
        bool                        bChainEnabled_;
        uint                        nTempsBordee_;
        T_Priorities                priorities_;
        T_Priorities                tacticalPriorities_;
        T_LogisticAvailabilities    dispoHaulers_;
        T_LogisticAvailabilities    dispoRepairers_;
    } T_MaintenanceData;
    T_MaintenanceData* pMaintenanceData_;

    T_MedicalConsigns       requestedMedical_;
    T_MedicalConsigns       handledMedical_;
    typedef struct
    {
        bool                     bChainEnabled_;
        uint                     nTempsBordee_;
        T_LogisticAvailabilities dispoReleveAmbulances_;
        T_LogisticAvailabilities dispoRamassageAmbulances_;  
        T_LogisticAvailabilities dispoDoctors_;
        T_Priorities             priorities_;
        T_Priorities             tacticalPriorities_;
    } T_MedicalData;
    T_MedicalData* pMedicalData_;


    bool bRadioSilence_;
    bool bCommJammed_;
    bool bRadarEnabled_;

    bool bPrisoner_;
    bool bSurrendered_;
    bool bRefugeesManaged_;

    uint nTC2_;
    uint nLogMaintenanceSuperior_;
    uint nLogMedicalSuperior_;
    uint nLogSupplySuperior_;


private:
    static MIL_AgentID nMaxId_;
};

#include "Agent.inl"

#endif // __Agent_h_
