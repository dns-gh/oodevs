//*****************************************************************************
// 
// $Created: NLD 2002-07-15 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_Agent.h $
// $Author: Nld $
// $Modtime: 27/04/05 17:28 $
// $Revision: 100 $
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
#include "MT_Tools/MT_Quad.h"

#include "GFX/GFX_Color.h"

#include "MOS_AgentManager.h"

class MOS_AgentListView;
class QListViewItem;
class MOS_Value;
class MOS_AttrEditor;
class MOS_RC;
class MOS_Trace;
class MOS_Sector;
class MOS_ObjectKnowledge;
class MOS_AgentModel;
class MOS_Gtia;
class MOS_DynaObject;
class MOS_Surface;
class MOS_Experience;
class MOS_Tiredness;
class MOS_Morale;
class MOS_LogMaintenanceConsign;
class MOS_LogMedicalConsign;
class MOS_LogSupplyConsign;
class MOS_TypePion;
class MOS_TypeAutomate;

//=============================================================================
// Created: NLD 2002-07-15
// Last modified: JVT 04-03-31
//=============================================================================
class MOS_Agent

{
    MT_COPYNOTALLOWED( MOS_Agent );

public:
    //-------------------------------------------------------------------------
    /** @name Types */
    //-------------------------------------------------------------------------
    //@{
    typedef std::set< MOS_LogMaintenanceConsign* >       T_MaintenanceConsignSet;
    typedef T_MaintenanceConsignSet::iterator            IT_MaintenanceConsignSet;
    typedef T_MaintenanceConsignSet::const_iterator      CIT_MaintenanceConsignSet;

    typedef std::set< MOS_LogSupplyConsign* >       T_SupplyConsignSet;
    typedef T_SupplyConsignSet::iterator            IT_SupplyConsignSet;
    typedef T_SupplyConsignSet::const_iterator      CIT_SupplyConsignSet;

    typedef std::set< MOS_LogMedicalConsign* >       T_MedicalConsignSet;
    typedef T_MedicalConsignSet::iterator            IT_MedicalConsignSet;
    typedef T_MedicalConsignSet::const_iterator      CIT_MedicalConsignSet;
    
    typedef std::set< const MOS_Agent* >        T_AgentConstPtrSet;
    typedef T_AgentConstPtrSet::iterator        IT_AgentConstPtrSet;
    typedef T_AgentConstPtrSet::const_iterator  CIT_AgentConstPtrSet;

    typedef std::set< const MOS_DynaObject* >    T_ObjectConstPtrSet;
    typedef T_ObjectConstPtrSet::iterator        IT_ObjectConstPtrSet;
    typedef T_ObjectConstPtrSet::const_iterator  CIT_ObjectConstPtrSet;

    typedef std::map< uint32, MOS_Sector* >                      T_VisionSectorMap;
    typedef T_VisionSectorMap::iterator                          IT_VisionSectorMap;
    typedef T_VisionSectorMap::const_iterator                    CIT_VisionSectorMap;

    typedef MT_Quad< MT_Float, MT_Vector2D, MT_Vector2D, MT_Float > T_VisionConeQuad;
    typedef std::vector< T_VisionConeQuad >                         T_VisionConeVector;
    typedef T_VisionConeVector::iterator                            IT_VisionConeVector;
    typedef T_VisionConeVector::const_iterator                      CIT_VisionConeVector;

    typedef std::vector< T_PointVector >            T_PathFindVector;
    typedef const T_PathFindVector                  CT_PathFindVector;
    typedef T_PathFindVector::iterator              IT_PathFindVector;
    typedef T_PathFindVector::reverse_iterator      RIT_PathFindVector;
    typedef T_PathFindVector::const_iterator        CIT_PathFindVector;

    typedef std::map< MOS_AgentListView*, QListViewItem* >        T_AgentItemMap;
    typedef const T_AgentItemMap                                  CT_AgentItemMap;
    typedef T_AgentItemMap::iterator                              IT_AgentItemMap;
    typedef T_AgentItemMap::const_iterator                        CIT_AgentItemMap;

    typedef std::vector< MOS_RC* >                                T_RCVector;
    typedef const T_RCVector                                      CT_RCVector;
    typedef T_RCVector::iterator                                  IT_RCVector;
    typedef T_RCVector::const_iterator                            CIT_RCVector;

    typedef std::vector< MOS_Trace* >                                T_TraceVector;
    typedef const T_TraceVector                                      CT_TraceVector;
    typedef T_TraceVector::iterator                                  IT_TraceVector;
    typedef T_TraceVector::const_iterator                            CIT_TraceVector;

    typedef std::map< uint, MOS_ObjectKnowledge* > T_ObjectKnowledgeMap;
    typedef T_ObjectKnowledgeMap::iterator         IT_ObjectKnowledgeMap;
    typedef T_ObjectKnowledgeMap::const_iterator   CIT_ObjectKnowledgeMap;

    typedef std::vector< uint >                     T_MaintenancePriorities;
    typedef T_MaintenancePriorities::const_iterator CIT_MaintenancePriorities;
    
    typedef std::vector< MOS_Agent* >               T_TacticalPriorities;
    typedef T_TacticalPriorities::const_iterator    CIT_TacticalPriorities;

    typedef std::vector< ASN1T_EnumHumanWound > T_MedicalPriorities;
    typedef T_MedicalPriorities::const_iterator CIT_MedicalPriorities;

    struct T_EquipementState
    {
        uint nNbrAvailable_;
        uint nNbrUnavailable_;
        uint nNbrReparable_;
        uint nNbrInMaintenance_;
        uint nNbrPrisoner_;
    };
    
    typedef std::map< MIL_AgentID, T_EquipementState >  T_EquipementQty_Map;
    typedef T_EquipementQty_Map::iterator               IT_EquipementQty_Map;
    typedef T_EquipementQty_Map::const_iterator         CIT_EquipementQty_Map;

    typedef std::vector< std::pair< uint, uint > >        T_DisponibiliteLogMoyenVector;
    typedef T_DisponibiliteLogMoyenVector::const_iterator CIT_DisponibiliteLogMoyenVector;

    struct T_Lend
    {
        const MOS_Agent* pLent_;
              uint       nComposanteTypeID_;
              uint       nNbr_;
    };

    typedef std::vector< T_Lend >        T_LendVector; 
    typedef T_LendVector::const_iterator CIT_LendVector;
        
    typedef std::map< MIL_AgentID, uint >       T_RessourceQty_Map;
    typedef T_RessourceQty_Map::iterator        IT_RessourceQty_Map;
    typedef T_RessourceQty_Map::const_iterator  CIT_RessourceQty_Map;

    typedef std::vector< uint >                 T_NbcAgentVector;
    typedef T_NbcAgentVector::const_iterator    CIT_NbcAgentVector;

    typedef std::vector< uint >                 T_RenfortVector;
    typedef T_RenfortVector::const_iterator     CIT_RenfortVector;

    typedef std::vector< uint >                 T_TransportVector;
    typedef T_TransportVector::const_iterator     CIT_TransportVector;

    typedef std::vector< MOS_Surface* >     T_SurfaceVector;
    typedef T_SurfaceVector::const_iterator CIT_SurfaceVector;

    typedef std::vector< std::pair< uint, uint > > T_QuotasVector;
    typedef T_QuotasVector::const_iterator         CIT_QuotasVector;
    //@}

public:
     MOS_Agent( const ASN1T_MsgAutomateCreation& asnMsg );
     MOS_Agent( const ASN1T_MsgPionCreation& asnMsg );
    ~MOS_Agent();

    //-------------------------------------------------------------------------
    /** @name Accessors */
    //-------------------------------------------------------------------------
    //@{
    bool                    IsAutomate  () const;
    bool                    IsEmbraye   () const;
    MIL_AgentID             GetAgentID  () const;
    const MT_Vector2D&      GetPos      () const;
    MT_Float                GetAltitude () const;
//    E_AgentTypeID           GetType     () const;
    const std::string&      GetSymbol   () const;
    MT_Vector2D             GetPosAround();
    E_TypeArme              GetArme     () const;
    //    void                    SetArme     ();
    MT_Float                GetSpeed    () const;
    MOS_Agent*              GetParent   () const;
          MOS_Team&         GetTeam     () const;
    const MOS_AgentModel*   GetModelPion() const;
    const MOS_AgentModel*   GetModelAutomate() const;
    MT_Float                GetElongationFactor() const;

    E_PostureType           GetOldPosture                  () const;
    E_PostureType           GetCurrentPosture              () const;
    uint                    GetPostureCompletionPourcentage() const;

    const MOS_Experience& GetExperience() const;
    const MOS_Tiredness&  GetTiredness() const;
    const MOS_Morale&     GetMorale() const;

    MOS_Gtia&   GetGtia() const;
    //@}

    //-------------------------------------------------------------------------
    /** @name List Agent*/
    //-------------------------------------------------------------------------
    //@{
    QListViewItem*          GetItem                 ( MOS_AgentListView& listview ) const;
    void                    RegisterListView        ( MOS_AgentListView& listview );
    void                    UnregisterListView      ( MOS_AgentListView& listview );
    void                    RegisterListViewItem    ( MOS_AgentListView* pListView, QListViewItem* pItem );
    void                    UnregisterListViewItem  ( MOS_AgentListView* pListView, QListViewItem* pItem );
    void                    DeleteListView          ( MOS_AgentListView& listview );
    void                    DeleteListView          ();
    //@}

    //-------------------------------------------------------------------------
    /** @name Attributes*/
    //-------------------------------------------------------------------------
    //@{
    void OnReceiveMsgObjectInterVisibility( DIN::DIN_Input& input );
    void OnReceiveMsgUnitInterVisibility  ( DIN::DIN_Input& input );

    void OnAttributeUpdated         ( const ASN1T_MsgUnitAttributes& asnMsg );
    void OnAttributeUpdated         ( const ASN1T_MsgUnitDotations& asnMsg );
    void OnLogisticLinksChanged( const ASN1T_MsgChangeLiensLogistiquesAck& asnMsg );

    void OnAutomateChanged      ( MOS_Agent& newAutomate );
    void OnKnowledgeGroupChanged( MOS_Gtia& newGtia );

    MOS_AttrEditor*          GetAttributeEditor() const;
    void                     SetAttributeEditor( MOS_AttrEditor* pAttrEditor );
    //@}

    //-------------------------------------------------------------------------
    /** @name RC */
    //-------------------------------------------------------------------------
    //@{
    void OnReceiveMsgWaitForOrderConduite( const ASN1T_MsgAttenteOrdreConduite& asnMsg );
    void OnReceiveMsgCR                  ( const ASN1T_MsgCR& msg );
    void RegisterRC                      ( MOS_RC& rc );
    void UnregisterRC                    ( MOS_RC& rc );
    //@}

    T_TraceVector& GetTraceVector();
    T_RCVector& GetRCVector();

    //-------------------------------------------------------------------------
    /** @name Trace */
    //-------------------------------------------------------------------------
    //@{
    void OnReceiveTraceMsg( DIN::DIN_Input& input );
    void RegisterTrace    ( MOS_Trace& Trace );
    void UnregisterTrace  ( MOS_Trace& Trace );
    //@}

    void OnReceiveDebugDrawPointsMsg( DIN::DIN_Input& input );

    //-------------------------------------------------------------------------
    /** @name Vision*/
    //-------------------------------------------------------------------------
    //@{
    void DrawVision             ();
    void DrawVisionCone         ();
    void DrawVisionSurface      ();
    void GetVisionSurfaces      ( T_VisionResultMap& res );
    void OnReceiveMsgVisionCones( DIN::DIN_Input& input );
    //@}

    //-------------------------------------------------------------------------
    /** @name Draw */
    //-------------------------------------------------------------------------
    //@{
    void                  Draw          ();
    bool                  IsInside      ( const MT_Vector2D& vPos ) const;
    const std::string     GetName       () const;
    void                  SetName       ( const std::string sText );
    void                  SetDefaultName();
    //@}

    //-------------------------------------------------------------------------
    /** @name PathFind */
    //-------------------------------------------------------------------------
    //@{
    void  DrawPathFind        ();
    void  DrawOldPathFind     ();
    void  UpdatePathFind      ();
    void  OnReceiveMsgPathFind( const ASN1T_MsgUnitPathFind& asnMsg );
    void  ClearPathFind       ();
    void  NotifyMagicalMove   ();
    void  SaveOldPathFind     ();
   //@}

    //! @name Logistic - maintenance
    //@{
    void AddMaintenanceConsign          ( MOS_LogMaintenanceConsign& consign );
    void RemoveMaintenanceConsign       ( MOS_LogMaintenanceConsign& consign );
    void AddHandledMaintenanceConsign   ( MOS_LogMaintenanceConsign& consign );
    void RemoveHandledMaintenanceConsign( MOS_LogMaintenanceConsign& consign );
    void OnReceiveMsgLogMaintenanceEtat( const ASN1T_MsgLogMaintenanceEtat& asn );
    bool IsMaintenancePionLog() const;
    //@}

    //! @name Logistic - santé
    //@{
    void AddMedicalConsign          ( MOS_LogMedicalConsign& consign );
    void RemoveMedicalConsign       ( MOS_LogMedicalConsign& consign );
    void AddHandledMedicalConsign   ( MOS_LogMedicalConsign& consign );
    void RemoveHandledMedicalConsign( MOS_LogMedicalConsign& consign );
    void OnReceiveMsgLogMedicalEtat( const ASN1T_MsgLogSanteEtat& asn );
    bool IsMedicalPionLog() const;
    //@}

    //! @name Logistic - Rav
    //@{
    void AddSupplyConsign          ( MOS_LogSupplyConsign& consign );
    void RemoveSupplyConsign       ( MOS_LogSupplyConsign& consign );
    void AddHandledSupplyConsign   ( MOS_LogSupplyConsign& consign );
    void RemoveHandledSupplyConsign( MOS_LogSupplyConsign& consign );
    void OnReceiveMsgLogSupplyEtat  ( const ASN1T_MsgLogRavitaillementEtat& asn );
    void OnReceiveMsgLogSupplyQuotas( const ASN1T_MsgLogRavitaillementQuotas& asn );
    bool IsSupplyPionLog() const;
    //@}

   bool bNeedRedrawVision_;

protected:
    void OnAttributeUpdated_Personnel          ( const ASN1T_MsgUnitDotations& asnMsg );
    void OnAttributeUpdated_Equipement         ( const ASN1T_MsgUnitDotations& asnMsg );
    void OnAttributeUpdated_Ressources         ( const ASN1T_MsgUnitDotations& asnMsg );   

public:
    MIL_AgentID             nAgentID_;
    bool                    bEmbraye_;
    std::string             sName_;
    std::string             symbolName_;
    const MOS_TypePion*     pTypePion_;
    const MOS_TypeAutomate* pTypeAutomate_;
    MOS_Agent*              pParent_;
    MT_Vector2D*            pAgentPoint_;
    MT_Vector2D             vPos_;
//    E_AgentTypeID           nType_;
    E_TypeArme              nArme_;
    bool                    bAutomate_;
    MT_Float                rSpeed_;
    int                     nAltitude_;
    int                     nDirection_;
    MOS_AttrEditor*         pAttrEditor_;
    MT_Float                rScrollPathFind_;
    T_AgentItemMap          agentItemMap_;

    T_RCVector              RCVector_;
    T_TraceVector           TraceVector_;
    T_PointVector           debugPointsToDraw_;

    T_PointVector           pathFindVector_;
    T_PathFindVector        oldPathFindVector_;
    bool                    bMagicMove_;

    T_AgentConstPtrSet      identifiedAgents_;
    T_AgentConstPtrSet      recognizedAgents_;
    T_AgentConstPtrSet      detectedAgents_;
    T_AgentConstPtrSet      recordedAgents_;

    T_ObjectConstPtrSet     objectsPerceived_;

    T_VisionSectorMap       sensorMap_;
    T_VisionResultMap       surfaceVision_;
    T_VisionConeVector      visionConeVector_;

//    T_KnowledgeMap          knowledgeMap_;

    uint                    nSpeed_;

    E_PostureType           nOldPosture_;
    E_PostureType           nCurrentPosture_;
    uint                    nPostureCompletionPourcentage_;

    bool                    bLoadingState_;
    bool                    bStealthModeEnabled_;

//    MOS_AgentKnowledge*  pWaitingKnowledge_;

    // Ressources humaines
    uint nNbOfficiers_;
    uint nNbOfficiersKilled_;
    uint nNbOfficiersInjured_;
    uint nNbOfficiersMentalInjured_;
    uint nNbOfficiersInMedicalChain_;
    uint nNbOfficiersInMaintenanceChain_;
    uint nNbOfficiersNBC_;

    uint nNbSousOfficiers_;
    uint nNbSousOfficiersKilled_;
    uint nNbSousOfficiersInjured_;
    uint nNbSousOfficiersMentalInjured_;
    uint nNbSousOfficiersInMedicalChain_;
    uint nNbSousOfficiersInMaintenanceChain_;
    uint nNbSousOfficiersNBC_;

    uint nNbMdrs_;
    uint nNbMdrsKilled_;
    uint nNbMdrsInjured_;
    uint nNbMdrsMentalInjured_;
    uint nNbMdrsInMedicalChain_;
    uint nNbMdrsInMaintenanceChain_;
    uint nNbMdrsNBC_;


    // Equipement
    T_EquipementQty_Map  equipement_;

    // Ressources
    T_RessourceQty_Map    ressources_;

    // State
    uint            nOpState_;
    E_AgentState    nState_;

    // Etat decisionnel
    E_ForceRatioState        nFightRateState_;
    E_RulesOfEngagementState nRulesOfEngagementState_;
    E_CloseCombatState       nCloseCombatState_;
    
    // NBC
    bool                bNbcProtectionSuitWorn_;
    T_NbcAgentVector    nbcAgentContaminating_;
    uint                nContaminationState_;

    // Brouillage
    bool bJammed_;
    bool bBlackOut_;

    // Radar
    bool bRadarEnabled_;

    // Renforts
    T_RenfortVector renforts_;
    uint            nPionRenforce_;

    // Transport
    T_TransportVector pionTransportes_;
    uint              nTransporteur_;

    // Facteurs humains
    const MOS_Experience* pExperience_;
    const MOS_Tiredness*  pTiredness_;
    const MOS_Morale*     pMorale_;

    // Log maintenance
    T_MaintenanceConsignSet       maintenanceConsigns_;        // TTA
    T_MaintenanceConsignSet       maintenanceConsignsHandled_; // LOG
    bool                          bMaintenancePionLog_;
    bool                          bMaintenanceChainEnabled_;
    uint                          nMaintenanceTempsBordee_;
    T_MaintenancePriorities       maintenancePriorities_;
    T_TacticalPriorities          maintenanceTacticalPriorities_;
    T_DisponibiliteLogMoyenVector maintenanceDispoHaulers_;
    T_DisponibiliteLogMoyenVector maintenanceDispoRepairers_;  

    // Log santé
    T_MedicalConsignSet           medicalConsigns_;        // TTA
    T_MedicalConsignSet           medicalConsignsHandled_; // LOG
    bool                          bMedicalPionLog_;
    bool                          bMedicalChainEnabled_;
    uint                          nMedicalTempsBordee_;
    T_DisponibiliteLogMoyenVector medicalDispoReleveAmbulances_;
    T_DisponibiliteLogMoyenVector medicalDispoRamassageAmbulances_;  
    T_DisponibiliteLogMoyenVector medicalDispoDoctors_;
    T_MedicalPriorities           medicalPriorities_;
    T_TacticalPriorities          medicalTacticalPriorities_;

    // Log rav
    T_SupplyConsignSet            supplyConsigns_;        // TTA
    T_SupplyConsignSet            supplyConsignsHandled_; // LOG
    bool                          bSupplyPionLog_;
    bool                          bSupplyChainEnabled_;
    T_DisponibiliteLogMoyenVector supplyDispoTransporters_;
    T_DisponibiliteLogMoyenVector supplyDispoCommanders_;  
    T_RessourceQty_Map            stocks_;
    T_QuotasVector                quotas_;

    // Prets
    T_LendVector        lends_;

    // Surrendered / prisoner
    bool bSurrendered_;
    bool bPrisoner_;

    // Refugees
    bool bRefugeesManaged_;

    MOS_Gtia* pGtia_;

    MT_Float rElongationFactor_;
    T_SurfaceVector surfaces_;

    // Logistic links
    uint nTC2ID_;
    uint nLogMaintenanceSuperior_;
    uint nLogMedicalSuperior_;
    uint nLogSupplySuperior_;
};

#ifdef MOS_USE_INLINE
#   include "MOS_Agent.inl"
#endif

#endif // __MOS_Agent_h_
