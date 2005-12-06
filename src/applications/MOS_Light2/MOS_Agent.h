//*****************************************************************************
// 
// $Created: NLD 2002-07-15 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_Agent.h $
// $Author: Nld $
// $Modtime: 8/07/05 15:18 $
// $Revision: 25 $
// $Workfile: MOS_Agent.h $
// 
//*****************************************************************************

#ifndef __MOS_Agent_h_
#define __MOS_Agent_h_

#include "MOS_Types.h"
#include "MOS_Attr_Def.h"
#include "MOS_Order_Def.h"
#include "MOS_Perception_Def.h"
#include "MOS_RawVisionData.h"
#include "MOS_ASN_Types.h"
#include "MOS_AgentComposition.h"
#include "MOS_IDManager.h"
#include "MOS_FireResult.h"

#include "MT_Tools/MT_Quad.h"

#include "MOS_Agent_ABC.h"

class MOS_Report_ABC;
class MOS_ObjectKnowledge;
class MOS_AgentKnowledge;
class MOS_AgentModel;
class MOS_Gtia;
class MOS_Team;
class MOS_Object_ABC;
class MOS_Surface;
class MOS_TypePion;
class MOS_TypeAutomate;
class MOS_Experience;
class MOS_Tiredness;
class MOS_Morale;
class MOS_LogSupplyConsign;
class MOS_LogMedicalConsign;
class MOS_LogMaintenanceConsign;
class MOS_PopulationConcentration;
class MOS_PopulationFlow;
class MOS_Population;

// =============================================================================
// Created: APE 2004-07-19
// =============================================================================
class MOS_Agent : public MOS_Agent_ABC
{
    MT_COPYNOTALLOWED( MOS_Agent );
    friend class MOS_GLTool;

public:
    //-------------------------------------------------------------------------
    /** @name Types */
    //-------------------------------------------------------------------------
    //@{
    typedef std::vector< MOS_Agent* >      T_AgentVector;
    typedef T_AgentVector::iterator        IT_AgentVector;
    typedef T_AgentVector::const_iterator  CIT_AgentVector;

    typedef std::set< const MOS_Agent* >        T_AgentConstPtrSet;
    typedef T_AgentConstPtrSet::iterator        IT_AgentConstPtrSet;
    typedef T_AgentConstPtrSet::const_iterator  CIT_AgentConstPtrSet;

    typedef std::set< const MOS_Object_ABC* >     T_ObjectConstPtrSet;
    typedef T_ObjectConstPtrSet::iterator         IT_ObjectConstPtrSet;
    typedef T_ObjectConstPtrSet::const_iterator   CIT_ObjectConstPtrSet;

    typedef std::set< const MOS_PopulationConcentration* > T_ConcentrationSet;
    typedef T_ConcentrationSet::iterator                   IT_ConcentrationSet;
    typedef T_ConcentrationSet::const_iterator             CIT_ConcentrationSet;

    typedef std::set< const MOS_PopulationFlow* > T_FlowSet;
    typedef T_FlowSet::iterator                   IT_FlowSet;
    typedef T_FlowSet::const_iterator             CIT_FlowSet;

    typedef std::map< const MOS_PopulationFlow*, T_PointVector > T_FlowVisionMap;
    typedef T_FlowVisionMap::iterator                            IT_FlowVisionMap;
    typedef T_FlowVisionMap::const_iterator                      CIT_FlowVisionMap;

    struct sPopulationCollision
    {
        T_ConcentrationSet concentrations_;
        T_FlowSet          flows_;
    };

    typedef std::map< const MOS_Population*, sPopulationCollision > T_PopulationCollisionMap;
    typedef T_PopulationCollisionMap::const_iterator               CIT_PopulationCollisionMap;

    typedef MT_Quad< MT_Float, MT_Vector2D, MT_Vector2D, MT_Float > T_VisionConeQuad;
    typedef std::vector< T_VisionConeQuad >                         T_VisionConeVector;
    typedef T_VisionConeVector::iterator                            IT_VisionConeVector;
    typedef T_VisionConeVector::const_iterator                      CIT_VisionConeVector;

    typedef std::vector< MOS_Report_ABC* >          T_ReportVector;
    typedef T_ReportVector::iterator                IT_ReportVector;
    typedef T_ReportVector::const_iterator          CIT_ReportVector;
    typedef T_ReportVector::const_reverse_iterator  RCIT_ReportVector;

    typedef std::vector< uint >                 T_NbcAgentVector;
    typedef T_NbcAgentVector::const_iterator    CIT_NbcAgentVector;

    typedef std::vector< uint >               T_AgentIdVector;
    typedef T_AgentIdVector::const_iterator   CIT_AgentIdVector;

    typedef std::vector< MOS_Surface* >     T_SurfaceVector;
    typedef T_SurfaceVector::const_iterator CIT_SurfaceVector;

    typedef std::vector< uint >           T_Priorities;
    typedef T_Priorities::const_iterator  CIT_Priorities;
    //@}

public:
    explicit MOS_Agent( bool bGenerateId = false );
    explicit MOS_Agent( const ASN1T_MsgAutomateCreation& asnMsg );
    explicit MOS_Agent( const ASN1T_MsgPionCreation& asnMsg );
    ~MOS_Agent();

    void Initialize();

    //! @name Accessors
    //@{
    const MOS_AgentModel*       GetModelPion       () const;
    const MOS_AgentModel*       GetModelAutomate   () const;
    bool                        IsAutomate         () const;
    bool                        IsEmbraye          () const;
    const std::string           GetCategory        () const;
	const MIL_AgentID           GetID              () const;
    const MT_Vector2D&          GetPos             () const;
    const std::string&          GetSymbol          () const;
    MOS_Agent*                  GetParent          () const;
    MOS_Team&                   GetTeam            () const;
    MOS_Gtia&                   GetGtia            () const;
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
    const T_FireResults&        GetFireResult      () const;
    const MOS_Experience&       GetExperience      () const;
    const MOS_Tiredness&        GetTiredness       () const;
    const MOS_Morale&           GetMorale          () const;

    bool                        IsLoaded                 () const;
    bool                        AreHumanTransportersReady() const;
    bool                        IsNBSSuitOn              () const;
    const T_NbcAgentVector&     GetContaminatingNBCAgents() const;

    const MOS_AgentComposition& GetComposition () const;
    const MOS_TypePion*         GetTypePion    () const;
	const MOS_TypeAutomate*		GetTypeAutomate() const;
    T_AgentVector&              GetChildren    ();

    MOS_AgentKnowledge*         FindAgentKnowledge( uint nId );
    //@}

    //! @name Modifiers
    //@{
    void AddChild     ( MOS_Agent&         agent       );
    void SetName      ( const std::string& strName     );
    void SetAgentID   ( MIL_AgentID        nID         );
    void SetGtia      ( MOS_Gtia&          gtia        );
    void SetParent    ( MOS_Agent*         pParent     );
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

    void OnSuperiorChanged( MOS_Agent& superior );
    void OnSuperiorChanged( MOS_Gtia& superior );
    //@}

    //! @name Reports
    //@{
    void OnReceiveMsgWaitForOrderConduite( const ASN1T_MsgAttenteOrdreConduite& asnMsg );
	int  MOS_Agent::GetCurrentMission() const;
	void OnReceiveMission                ( const ASN1T_MsgPionOrder& asnMsg );


    // fire results
    void OnReceiveMsgStopFire( const ASN1T_FireDamagesPion&       asnMsg );
    void OnReceiveMsgStopFire( const ASN1T_FireDamagesPopulation& asnMsg );
    void DeleteAllFireResults();
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
    void AddConsign   ( MOS_LogSupplyConsign& consign );
    void RemoveConsign( MOS_LogSupplyConsign& consign );

    void AddConsign   ( MOS_LogMaintenanceConsign& consign );
    void RemoveConsign( MOS_LogMaintenanceConsign& consign );

    void AddConsign   ( MOS_LogMedicalConsign& consign );
    void RemoveConsign( MOS_LogMedicalConsign& consign );

    void HandleConsign   ( MOS_LogSupplyConsign& consign );
    void TerminateConsign( MOS_LogSupplyConsign& consign );

    void HandleConsign   ( MOS_LogMaintenanceConsign& consign );
    void TerminateConsign( MOS_LogMaintenanceConsign& consign );

    void HandleConsign   ( MOS_LogMedicalConsign& consign );
    void TerminateConsign( MOS_LogMedicalConsign& consign );

    void OnReceiveMsgLogMaintenanceEtat( const ASN1T_MsgLogMaintenanceEtat& asn );
    void OnReceiveMsgLogMedicalEtat( const ASN1T_MsgLogSanteEtat& asn );
    void OnReceiveMsgLogSupplyEtat  ( const ASN1T_MsgLogRavitaillementEtat& asn );
    void OnReceiveMsgLogSupplyQuotas( const ASN1T_MsgLogRavitaillementQuotas& asnMsg );

    void InitializeStocks();

    // Accessor
    bool MOS_Agent::IsLogisticTC2           () const;
    bool MOS_Agent::IsLogisticBLD           () const;
    bool MOS_Agent::IsLogisticBLT           () const;
    bool MOS_Agent::IsLogisticMaintenance   () const;
    bool MOS_Agent::IsLogisticSante         () const;
    bool MOS_Agent::IsLogisticRavitaillement() const;
    //@}

    //! @name ODB
    //@{
    void ReadODB( MOS_InputArchive& archive, bool bAutomate );
    void WriteODB( MT_XXmlOutputArchive& archive );
    //@}
	//aggregation
    bool  IsAggregated() const;
    void SetAggregation( bool bAgg );

public:
    //! @name Types
    //@{
    typedef std::set< MOS_LogSupplyConsign* > T_SupplyConsigns;
    typedef T_SupplyConsigns::iterator        IT_SupplyConsigns;

    typedef std::set< MOS_LogMaintenanceConsign* > T_MaintenanceConsigns;
    typedef T_MaintenanceConsigns::iterator        IT_MaintenanceConsigns;

    typedef std::set< MOS_LogMedicalConsign* > T_MedicalConsigns;
    typedef T_MedicalConsigns::iterator        IT_MedicalConsigns;

    typedef std::vector< std::pair< uint, uint > >    T_LogisticAvailabilities;
    typedef T_LogisticAvailabilities::const_iterator  CIT_LogisticAvailabilities;
    //@}


//$$$$$ Foutre tout ce binz en public ou utiliser des friend pour eviter d'avoir 34846 fonctions set/get
//$$$$$ qui puent.
public:
    MIL_AgentID					nAgentID_;
    std::string					sName_;
    std::string					symbolName_;
    std::string					levelSymbolName_;
    std::string					strCategory_;
    const MOS_TypePion*			pTypePion_;
    const MOS_TypeAutomate*		pTypeAutomate_;
    MOS_Gtia*					pGtia_;
    MOS_Agent*					pParent_;
    T_AgentVector				children_;
    MT_Vector2D					vPos_;
    bool						bAutomate_;
    bool						bEmbraye_;
    bool                    bAggregated_;
    int							nAltitude_;
    int							nDirection_;

    T_ReportVector				reportVector_;

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
    
    std::vector< MT_Vector2D >	reportPoints_;
    T_FireResults				fireResults_;

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

    MOS_AgentComposition		composition_;

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
    const MOS_Experience* pExperience_;
    const MOS_Tiredness*  pTiredness_;
    const MOS_Morale*     pMorale_;

    // Logistic Stuff
    T_SupplyConsigns        requestedSupplies_;
    T_SupplyConsigns        handledSupplies_;
    typedef struct
    {
        bool                     bChainEnabled_;
        T_LogisticAvailabilities dispoTransporters_;
        T_LogisticAvailabilities dispoCommanders_;  
        T_ResourceQty_Map        stocks_;
        T_LogisticAvailabilities quotas_;
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

    int nCurrentMission_;

private:
    static MIL_AgentID nMaxId_;
};

#include "MOS_Agent.inl"

#endif // __MOS_Agent_h_
