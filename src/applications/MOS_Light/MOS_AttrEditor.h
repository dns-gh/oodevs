//*****************************************************************************
//
// $Created: FBD 02-12-17 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_AttrEditor.h $
// $Author: Nld $
// $Modtime: 27/04/05 17:32 $
// $Revision: 53 $
// $Workfile: MOS_AttrEditor.h $
//
//*****************************************************************************

#ifndef __MOS_AttrEditor_h_
#define __MOS_AttrEditor_h_

#include "MOS_Types.h"
#include "MOS_Attr_Def.h"
#include "MOS_Agent.h"
#include "MOS_Experience.h"
#include "MOS_Tiredness.h"
#include "MOS_Morale.h"

#include <QFrame.h>
#include <QTabWidget.h>
#include <QButtonGroup.h>
#include <QPushButton.h>

class QListViewItem;

class MOS_ChangeHumanFactorsDialog;
class MOS_ChangeLogisticLinksDialog;
class MOS_LimaListView;
class MOS_Agent;
class MOS_MagicPos;
class MOS_RCEditor;
class MOS_TraceEditor;                       
class MOS_RC;
class MOS_Trace;
class MOS_OrderConduiteMissionDialog;
class MOS_AgentKnowledge;
class MOS_KnowledgeList_View;
class MOS_Value;
class MOS_ObjectKnowledge_ListView;
class MOS_ObjectKnowledge;
class MOS_LogMaintenanceConsign_ListView;
class MOS_LogMedicalConsign_ListView;
class MOS_LogSupplyConsign_ListView;
class MOS_LogisticSupplyChangeQuotasDialog;
class MOS_LogisticSupplyPushFlowDialog;
class MOS_SurrenderDialog;
class QListBox;
class QCheckBox;
class QListView;
class QWidget;

//*****************************************************************************
// Created: FBD 02-12-17
//*****************************************************************************
class MOS_AttrEditor : public QFrame
{
    Q_OBJECT
    MT_COPYNOTALLOWED( MOS_AttrEditor );

public:
    explicit MOS_AttrEditor( QWidget* pParent = 0 );
    virtual ~MOS_AttrEditor();

    //-------------------------------------------------------------------------
    /** @name Init*/
    //-------------------------------------------------------------------------
    //@{
    void Initialize();
    void Terminate();
    //@}

    //-------------------------------------------------------------------------
    /** @name */
    //-------------------------------------------------------------------------
    //@{
    bool IsActive( bool bMove );
    bool IsActiveTab( QWidget* pWidget );
    //@}

    //-------------------------------------------------------------------------
    /** @name Accessors*/
    //-------------------------------------------------------------------------
    //@{
    void           SetAgent( MOS_Agent* pAgent );
    void           SetWidgetStack( QWidgetStack* pWidgetStack );
    void           NotifyAgentHasMoved( MOS_Agent& agent, const MT_Vector2D& vPos );    
    MOS_MagicPos*  GetCoord() const;

    //@}

    //-------------------------------------------------------------------------
    /** @name DIN*/
    //-------------------------------------------------------------------------
    //@{
    void WriteMsg( std::string& sParam );
    //@}

    //-------------------------------------------------------------------------
    /** @name Exit*/
    //-------------------------------------------------------------------------
    //@{
    void Accept();
    void Cancel();
    //@}

    //-------------------------------------------------------------------------
    /** @name */
    //-------------------------------------------------------------------------
    //@{
    void Reset();

    void SetLends( const MOS_Agent::T_LendVector& lends );

    void SetFightRateState        ( E_ForceRatioState        nState );
    void SetRulesOfEngagementState( E_RulesOfEngagementState nState );
    void SetCloseCombatState      ( E_CloseCombatState       nState );

    void SetAutomateMode( bool bEmbraye );
    void SetAgentState  ( E_AgentState nState );
    void SetLoadingState( bool bLoadingState );
    void SetStealthModeEnabled( bool bStealthMode );
    void SetOpState     ( uint nValue );

    void SetSpeed    ( uint nSpeed );
    void SetAltitude( int nAltitude );
    void SetDirection( uint nDirection );
    void SetPosture  ( E_PostureType nOldPosture, E_PostureType nCurrentPosture, uint nPostureCompletionPourcentage );
    void ResetPosture();

    void ResetEquipments();
    void ResetRessources();
    void ResetHumains();
    void AddEquipment( MIL_AgentID nType, MOS_Agent::T_EquipementState nState );
    void AddRessource( MIL_AgentID nType, uint nQty );
    void AddHumains  ( const std::string& strLabel, uint nNbOfficiers, uint nNbSousOfficiers, uint nNbMdr);

    void SetHeliportedHumans( uint );

//    void ResetTransportEquipments();
//    void ResetTransportRessources();
//    void ResetTransportHumains();
//    void ResetTransportCapacities();
//    void AddTransportEquipment( MIL_AgentID nType, MOS_Agent::T_EquipementState nState );
//    void AddTransportRessource( MIL_AgentID nType, uint nQty );
//    void AddTransportRessource( MIL_AgentID nType, uint nQtyAvailable, uint nQtyTransit );
//    void AddTransportRessource( const char*, uint nQty );
//    void AddTransportHumains( uint** );
//    void UpdateTransportRessource( const char*, uint nQty );
//    void UpdateTransportRessource( MIL_AgentID nType, uint nQty );
//    void UpdateTransportRessource( MIL_AgentID nType, uint nQtyAvailable, uint nQtyTransit );
//    void AddTransportCapacity   ( MIL_AgentID nType, uint nQty );
//    void UpdateTransportCapacity( MIL_AgentID nType, uint nQty );
//    void AddTransportEquipements( uint nMOSID, uint nBreakDown, uint nNTI, uint nNbr );
    void SetNbcProtectionSuitWorn( bool bNbcProtectionSuitWorn );
    void SetNbcAgentsContaminating( const MOS_Agent::T_NbcAgentVector& nbcAgentContaminating );
    void SetContaminationState    ( uint nContaminationState );
    void SetRenforts( const MOS_Agent::T_RenfortVector& renforts );
    void SetPionRenforce( uint nID );
    void SetPionTransportes( const MOS_Agent::T_TransportVector& transports );
    void SetTransporteur   ( uint nID );
    void SetJammedState( bool bJammed );
    void SetBlackOutState( bool bBlackOut );
    void SetRadarEnabled ( bool bRadarEnabled );
    void SetPrisonerState   ( bool bPrisoner_   );
    void SetSurrenderedState( bool bSurrendered );
    void SetRefugeesManagedState( bool bRefugeesManaged );

    void SetMorale    ( const MOS_Morale& morale );
    void SetExperience( const MOS_Experience& experience );
    void SetTiredness ( const MOS_Tiredness& tiredness );
    //@}

    //-------------------------------------------------------------------------
    /** @name RC*/
    //-------------------------------------------------------------------------
    //@{
    QListViewItem&  CreateRCItem( MOS_RC& rcItem );
    void            DeleteRCItem( QListViewItem& rcItem );
    //@}

    //-------------------------------------------------------------------------
    /** @name Trace*/
    //-------------------------------------------------------------------------
    //@{
    QListViewItem&   CreateTraceItem( MOS_Trace& TraceItem );
    void             DeleteTraceItem( QListViewItem& TraceItem );
    //@}

    //! @name Log maintenance
    //@{
    void AddMaintenanceConsign          ( MOS_LogMaintenanceConsign& consign );
    void RemoveMaintenanceConsign       ( MOS_LogMaintenanceConsign& consign );
    void AddHandledMaintenanceConsign   ( MOS_LogMaintenanceConsign& consign );
    void RemoveHandledMaintenanceConsign( MOS_LogMaintenanceConsign& consign );

    void SetLogMaintenanceChaineEnabled    ( bool bChainEnabled );
    void SetLogMaintenanceTempsBordee      ( uint nTempsBordee );
    void SetLogMaintenancePriorites        ( const MOS_Agent::T_MaintenancePriorities&  maintenancePriorities_ );
    void SetLogMaintenanceTacticalPriorites( const MOS_Agent::T_TacticalPriorities&  tacticalPriorites );

    void ResetLogMaintenanceDispoHaulers();
    void ResetLogMaintenanceDispoRepairers();

    void AddLogMaintenanceDispoHauler  ( uint nTypeID, uint nPourcentage );
    void AddLogMaintenanceDispoRepairer( uint nTypeID, uint nPourcentage );
    //@}


    //! @name Log Medical
    //@{
    void AddMedicalConsign          ( MOS_LogMedicalConsign& consign );
    void RemoveMedicalConsign       ( MOS_LogMedicalConsign& consign );
    void AddHandledMedicalConsign   ( MOS_LogMedicalConsign& consign );
    void RemoveHandledMedicalConsign( MOS_LogMedicalConsign& consign );

    void SetLogMedicalChaineEnabled    ( bool bChainEnabled );
    void SetLogMedicalTempsBordee      ( uint nTempsBordee );
    void SetLogMedicalPriorites        ( const MOS_Agent::T_MedicalPriorities&  medicalPriorities );
    void SetLogMedicalTacticalPriorites( const MOS_Agent::T_TacticalPriorities&  tacticalPriorites );

    void ResetLogMedicalDispoReleveAmbulances(  );
    void ResetLogMedicalDispoRamassageAmbulances(  );  
    void ResetLogMedicalDispoDoctors(  );

    void AddLogMedicalDispoReleveAmbulances( uint nTypeID, uint nPourcentage );
    void AddLogMedicalDispoRamassageAmbulances( uint nTypeID, uint nPourcentage );  
    void AddLogMedicalDispoDoctors( uint nTypeID, uint nPourcentage );
    //@}

    //! @name Log supply
    //@{
    void AddSupplyConsign          ( MOS_LogSupplyConsign& consign );
    void RemoveSupplyConsign       ( MOS_LogSupplyConsign& consign );
    void AddHandledSupplyConsign   ( MOS_LogSupplyConsign& consign );
    void RemoveHandledSupplyConsign( MOS_LogSupplyConsign& consign );

    void SetLogSupplyChaineEnabled( bool bChainEnabled );

    void ResetLogSupplyDispoTransporters();
    void ResetLogSupplyDispoCommanders  ();

    void AddLogSupplyDispoTransporter( uint nTypeID, uint nPourcentage );
    void AddLogSupplyDispoCommander  ( uint nTypeID, uint nPourcentage );
    void ResetLogSupplyStocks();
    void AddLogSupplyStock( MIL_AgentID nType, uint nQty );

    void ResetLogSupplyQuotas();
    void AddLogSupplyQuota( MIL_AgentID nType, uint nQty );
    //@}
    
protected slots:
    //-------------------------------------------------------------------------
    /** @name Slots */
    //-------------------------------------------------------------------------
    //@{
    void SlotAccept();
    void SlotReject();
    void SlotClearItineraire();
    void SlotOrder();
    void SlotDrawKnowledge( bool bValue );
    void SlotChangeAutomateMode();    
    void SlotMagicActionRecompletementTotal     ();
    void SlotMagicActionRecompletementPersonnel ();
    void SlotMagicActionRecompletementEquipement();
    void SlotMagicActionRecompletementRessources();
    void SlotMagicActionDestroyComposante       ();
    void SlotMagicActionChangeHumanFactors      ();
    void SlotMagicActionChangeLogisticLinks     ();
    void SlotMagicActionDestroyAllComposantes   ();
    void SlotMagicActionLogisticChangeQuotas    ();
    void SlotMagicActionLogisticPushFlow        ();
    void SlotMagicActionSurrender               ();
    //@}  

private:
    //! @name Tools
    //@{
    static std::string ConvertPostureToString( E_PostureType nPosture );
    //@}

private:
    MOS_ChangeHumanFactorsDialog*           pChangeHumanFactorsDialog_;
    MOS_ChangeLogisticLinksDialog*          pChangeLogisticLinksDialog_;
    MOS_LogisticSupplyChangeQuotasDialog*   pLogisticSupplyChangeQuotasDialog_;
    MOS_LogisticSupplyPushFlowDialog*       pLogisticSupplyPushFlowDialog_;
    MOS_SurrenderDialog*                    pSurrenderDialog_;

    QWidgetStack*       pWidgetStack_;
    QTabWidget*         pTabWidget_;
    QButtonGroup*       pButtonGroup_;

    QPushButton*        pButtonClearItineraire_;
    QPushButton*        pButtonOrder_;

    QPushButton*        pButtonChangeAutomateMode_;

    MOS_MagicPos*       pMagicPos_;
    QPushButton*        pButtonMagicRecompletementTotal_;
    QPushButton*        pButtonMagicRecompletementPersonnel_;
    QPushButton*        pButtonMagicRecompletementEquipement_;
    QPushButton*        pButtonMagicRecompletementRessources_;
    QPushButton*        pButtonMagicDestroyComposante_;
    QPushButton*        pButtonMagicDestroyAllComposantes_;
    QPushButton*        pButtonMagicChangeHumanFactors_;
    QPushButton*        pButtonMagicSurrender_;
    QPushButton*        pButtonChangeLogisticLinks_;
    QPushButton*        pButtonLogisticSupplyChangeQuotas_;
    QPushButton*        pButtonLogisticSupplyPushFlow_;
    
    QCheckBox*          pDrawKnowledge_;

    MOS_RCEditor*       pRCEditor_;
    MOS_TraceEditor*    pTraceEditor_;

    MOS_Agent*          pAgent_;
    MOS_OrderConduiteMissionDialog*     pOrderConduiteMissionDlg_;      //$$$$$$ a VIRER

    MOS_KnowledgeList_View*          pKnowledgeList_;
    MOS_ObjectKnowledge_ListView*    pObjectKnowledgeList_;

    QListView*          pRessourcesList_;
    QListView*          pEquipementList_;
    QListView*          pHumainsList_;

//    QListView*          pTransportCapacityList_;
//    QListView*          pTransportRessourcesList_;
//    QListView*          pTransportEquipementList_;
//    QListView*          pTransportHumainsList_;

    // Maintenance
    MOS_LogMaintenanceConsign_ListView* pLogMaintenanceConsignListView_;
    MOS_LogMaintenanceConsign_ListView* pLogMaintenanceConsignHandledListView_;
    QListView*                          pLogMaintenanceState_;
    QListViewItem*                      pLogMaintenanceStateChainEnabled_;
    QListViewItem*                      pLogMaintenanceStateTempsBordee_;
    QListViewItem*                      pLogMaintenanceStatePriorites_;
    QListViewItem*                      pLogMaintenanceStateTacticalPriorites_;
    QListView*                          pLogMaintenanceDispoHaulers_;
    QListView*                          pLogMaintenanceDispoRepairers_;

    // Medical
    MOS_LogMedicalConsign_ListView* pLogMedicalConsignListView_;
    MOS_LogMedicalConsign_ListView* pLogMedicalConsignHandledListView_;
    QListView*                      pLogMedicalState_;
    QListViewItem*                  pLogMedicalStateChainEnabled_;
    QListViewItem*                  pLogMedicalStateTempsBordee_;
    QListViewItem*                  pLogMedicalStatePriorites_;
    QListViewItem*                  pLogMedicalStateTacticalPriorites_;
    QListView*                      pLogMedicalDispoReleveAmbulances_;
    QListView*                      pLogMedicalDispoRamassageAmbulances_; 
    QListView*                      pLogMedicalDispoDoctors_;   

    // Supply
    MOS_LogSupplyConsign_ListView* pLogSupplyConsignListView_;
    MOS_LogSupplyConsign_ListView* pLogSupplyConsignHandledListView_;
    QListView*                     pLogSupplyState_;
    QListViewItem*                 pLogSupplyStateChainEnabled_;
    QListView*                     pLogSupplyDispoTransporters_;
    QListView*                     pLogSupplyDispoCommanders_;
    QListView*                     pLogSupplyStocks_;
    QListView*                     pLogSupplyQuotas_;


    QListView*          pStateListView_;
    QListViewItem*      pSpeedItem_;
    QListViewItem*      pAltitudeItem_;
    QListViewItem*      pDirectionItem_;
    QListViewItem*      pOldPostureItem_;
    QListViewItem*      pCurrentPostureItem_;
    QListViewItem*      pPostureCompletionPourcentageItem_;
    
    QListViewItem*      pLoadingStateItem_;
    QListViewItem*      pStealthModeItem_;
    QListViewItem*      pAgentStateItem_;
    QListViewItem*      pOpStateItem_;
    QListViewItem*      pAutomateModeItem_;
    QListViewItem*      pFightRateStateItem_;
    QListViewItem*      pRulesOfEngagementStateItem_;
    QListViewItem*      pCloseCombatStateItem_;    
    QListViewItem*      pNbcProtectionSuitWornItem_;
    QListViewItem*      pNbcAgentsContaminatingItem_;
    QListViewItem*      pContaminationStateItem_;
    QListViewItem*      pRenfortsItem_;
    QListViewItem*      pPionRenforceItem_;
    QListViewItem*      pTransporteurItem_;
    QListViewItem*      pTransportsItem_;
    QListViewItem*      pJammedStateItem_;
    QListViewItem*      pBlackOutStateItem_;
    QListViewItem*      pRadarStateItem_;
    QListViewItem*      pPrisonerStateItem_;
    QListViewItem*      pSurrenderedStateItem_;
    QListViewItem*      pRefugeesManagedStateItem_;

    QListViewItem*      pMoraleItem_;
    QListViewItem*      pExperienceItem_;    
    QListViewItem*      pTirednessItem_;

    QListView*      pLends_;
};

#ifdef MOS_USE_INLINE
#	include "MOS_AttrEditor.inl"
#endif


#endif // __MOS_AttrEditor_h_