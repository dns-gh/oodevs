//*****************************************************************************
// 
// $Created: FBD 02-12-17 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_AttrEditor.cpp $
// $Author: Nld $
// $Modtime: 27/04/05 17:33 $
// $Revision: 73 $
// $Workfile: MOS_AttrEditor.cpp $
// 
//*****************************************************************************

#include "MOS_Light_pch.h"

#include "MOS_AttrEditor.h"
#include "moc_MOS_AttrEditor.cpp"

#ifndef MOS_USE_INLINE
#   include "MOS_AttrEditor.inl"
#endif


#include <QButtonGroup.h>
#include <QWidget.h>
#include <QCheckBox.h>
#include <QWidgetstack.h>
#include <QTabBar.h>
#include <QHeader.h>

#include "MOS_App.h"
#include "MOS_World.h"
#include "MOS_WorldWidget.h"
#include "MOS_Mainwindow.h"
#include "MOS_Agent.h"
#include "MOS_RCEditor.h"
#include "MOS_RCListView.h"
#include "MOS_TraceEditor.h"
#include "MOS_TraceListView.h"
#include "MOS_MagicPos.h"
#include "MOS_ASN_Messages.h"
#include "MOS_Value.h"
#include "MOS_Gtia.h"
#include "MOS_Team.h"

#include "MOS_LogisticSupplyChangeQuotasDialog.h"
#include "MOS_LogisticSupplyPushFlowDialog.h"
#include "MOS_OrderConduiteMissionDialog.h"
#include "MOS_KnowledgeList_ViewItem.h"
#include "MOS_KnowledgeList_View.h"
#include "MOS_ObjectKnowledge_ListView.h"
#include "MOS_ChangeHumanFactorsDialog.h"
#include "MOS_ChangeLogisticLinksDialog.h"
#include "MOS_LogSupplyConsign_ListView.h"
#include "MOS_LogMaintenanceConsign_ListView.h"
#include "MOS_LogMedicalConsign_ListView.h"
#include "MOS_LogisticSupplyRecompletionDialog.h"

#include "MOS_ASN_Messages.h"

using namespace DIN;

//-----------------------------------------------------------------------------
// Name: MOS_AttrEditor constructor
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
MOS_AttrEditor::MOS_AttrEditor( QWidget* pParent )
    : QFrame                      ( pParent )
    , pWidgetStack_( 0 )
    , pTabWidget_( 0 )
    , pButtonGroup_( 0 )
    , pButtonClearItineraire_( 0 )
    , pButtonOrder_( 0 )
    , pButtonChangeAutomateMode_( 0 )
    , pMagicPos_( 0 )
    , pButtonMagicRecompletementTotal_( 0 )
    , pButtonMagicRecompletementPersonnel_( 0 )
    , pButtonMagicRecompletementEquipement_( 0 )
    , pButtonMagicRecompletementRessources_( 0 )
    , pButtonMagicSurrender_( 0 )
    , pDrawKnowledge_( 0 )
    , pRCEditor_( 0 )
    , pTraceEditor_( 0 )
    , pAgent_( 0 )
    , pOrderConduiteMissionDlg_( 0 )
    , pKnowledgeList_( 0 )
    , pObjectKnowledgeList_( 0 )
    , pRessourcesList_( 0 )
    , pEquipementList_( 0 )
    , pHumainsList_( 0 )
    , pStateListView_( 0 )
    , pSpeedItem_( 0 )
    , pOldPostureItem_( 0 )
    , pCurrentPostureItem_( 0 )
    , pPostureCompletionPourcentageItem_( 0 )
    , pLoadingStateItem_( 0 )
    , pStealthModeItem_( 0 )
    , pDeadItem_( 0 )
    , pNeutralizedItem_( 0 )
    , pOpStateItem_( 0 )
    , pRawOpStateItem_( 0 )
    , pAutomateModeItem_( 0 )
    , pFightRateStateItem_( 0 )
    , pRulesOfEngagementStateItem_( 0 )
    , pCloseCombatStateItem_( 0 )   
    , pNbcProtectionSuitWornItem_( 0 )
    , pNbcAgentsContaminatingItem_( 0 )
    , pRenfortsItem_( 0 )
    , pAltitudeItem_( 0 )
    , pDirectionItem_( 0 )
    , pContaminationStateItem_( 0 )
    , pJammedStateItem_( 0 )
    , pPionRenforceItem_( 0 )
    , pTransporteurItem_( 0 )
    , pTransportsItem_( 0 )
    , pHumanTransportersReadyItem_( 0 )
    , pMoraleItem_( 0 )
    , pExperienceItem_( 0 )
    , pTirednessItem_( 0 )
    , pChangeHumanFactorsDialog_( 0 )
    , pButtonChangeLogisticLinks_( 0 )
    , pChangeLogisticLinksDialog_( 0 )
    , pLogMaintenanceConsignListView_( 0 )
    , pLogMaintenanceConsignHandledListView_( 0 )
    , pLogMaintenanceState_( 0 )
    , pLogMaintenanceStateChainEnabled_( 0 )
    , pLogMaintenanceStateTempsBordee_( 0 )
    , pLogMaintenanceStatePriorites_( 0 )
    , pLogMaintenanceStateTacticalPriorites_( 0 )
    , pLogMaintenanceDispoHaulers_( 0 )
    , pLogMaintenanceDispoRepairers_( 0 )
    , pLogMedicalConsignListView_( 0 )
    , pLogMedicalConsignHandledListView_( 0 )
    , pLogMedicalState_( 0 )
    , pLogMedicalStateChainEnabled_( 0 )
    , pLogMedicalStateTempsBordee_( 0 )
    , pLogMedicalStatePriorites_( 0 )
    , pLogMedicalStateTacticalPriorites_( 0 )
    , pLogMedicalDispoReleveAmbulances_( 0 )
    , pLogMedicalDispoRamassageAmbulances_( 0 ) 
    , pLogMedicalDispoDoctors_( 0 )
    , pLends_( 0 )
    , pLogSupplyConsignListView_( 0 )
    , pLogSupplyConsignHandledListView_( 0 )
    , pLogSupplyState_( 0 )
    , pLogSupplyStateChainEnabled_( 0 )
    , pLogSupplyDispoTransporters_( 0 )
    , pLogSupplyDispoCommanders_( 0 )
    , pLogSupplyStocks_( 0 )
    , pLogSupplyQuotas_( 0 )
    , pButtonLogisticSupplyChangeQuotas_( 0 )
    , pButtonLogisticSupplyPushFlow_( 0 )
    , pLogisticSupplyChangeQuotasDialog_( 0 )
    , pLogisticSupplyPushFlowDialog_( 0 )
    , pTC2Item_ ( 0 )
    , pLogMaintenanceSuperior_( 0 )
    , pLogMedicalSuperior_( 0 )
    , pLogSupplySuperior_( 0 )
    , pLogLinks_( 0 )
{
    setMargin( 50 );
    setLineWidth( 2 );
    setFrameStyle( QFrame::Sunken | QFrame::Box );

    // Main Layout
    QVBoxLayout* pLayout = new QVBoxLayout( this );

    pTabWidget_ = new QTabWidget( this );
    pLayout->addWidget( pTabWidget_ );


    // Tab Etat Physique
    QWidget* pStateWidget = new QWidget( this );
    QVBoxLayout* pLayoutState = new QVBoxLayout( pStateWidget );
    {
        // RC
        pRCEditor_ = new MOS_RCEditor( *this, pStateWidget );
        pLayoutState->addWidget( pRCEditor_ );

        pStateListView_ = new QListView( pStateWidget, "State" );
        pStateListView_->header()->hide();
        pStateListView_->addColumn( "Name" );
        pStateListView_->addColumn( "Value" );
        pStateListView_->setMargin( 5 );
        pStateListView_->setLineWidth( 2 );
        pStateListView_->setFrameStyle( QFrame::Sunken | QFrame::Box );
        pStateListView_->setSorting( -1, FALSE );
        pLayoutState->addWidget( pStateListView_ );

        pRawOpStateItem_                    = new QListViewItem( pStateListView_, "Etat opérationel brut", "" );
        pDeadItem_                          = new QListViewItem( pStateListView_, pRawOpStateItem_, "Mort", "" );
        pNeutralizedItem_                   = new QListViewItem( pStateListView_, pDeadItem_, "Neutralisé", "" );
        pMoraleItem_                        = new QListViewItem( pStateListView_, pNeutralizedItem_, "Moral", "" );
        pExperienceItem_                    = new QListViewItem( pStateListView_, pMoraleItem_, "Experience", "" );
        pTirednessItem_                     = new QListViewItem( pStateListView_, pExperienceItem_, "Fatigue", "" );
        pSpeedItem_                         = new QListViewItem( pStateListView_, pTirednessItem_, "Speed", "" );
        pAltitudeItem_                      = new QListViewItem( pStateListView_, pSpeedItem_, "Altitude", "" );
        pDirectionItem_                     = new QListViewItem( pStateListView_, pAltitudeItem_, "Direction", "" );
        pOldPostureItem_                    = new QListViewItem( pStateListView_, pDirectionItem_, "Ancienne posture", "" );
        pCurrentPostureItem_                = new QListViewItem( pStateListView_, pOldPostureItem_,"Nouvelle posture", "" );
        pPostureCompletionPourcentageItem_  = new QListViewItem( pStateListView_, pCurrentPostureItem_, "Posture completion", "" );
        pLoadingStateItem_                  = new QListViewItem( pStateListView_, pPostureCompletionPourcentageItem_, "Etat embarquement", "" );
        pHumanTransportersReadyItem_        = new QListViewItem( pStateListView_, pLoadingStateItem_, "Transporteurs d'hommes disponibles", "" );
        pStealthModeItem_                   = new QListViewItem( pStateListView_, pHumanTransportersReadyItem_, "Mode furtif", "" );
        pNbcProtectionSuitWornItem_         = new QListViewItem( pStateListView_, pStealthModeItem_, "Protection NBC mise", "" );
        pNbcAgentsContaminatingItem_        = new QListViewItem( pStateListView_, pNbcProtectionSuitWornItem_, "Contaminé par agents NBC", "" );
        pContaminationStateItem_            = new QListViewItem( pStateListView_, pNbcAgentsContaminatingItem_, "Etat contamination", "" );
        pRenfortsItem_                      = new QListViewItem( pStateListView_, pContaminationStateItem_, "Renforcé par", "" );
        pPionRenforceItem_                  = new QListViewItem( pStateListView_, pRenfortsItem_, "En renfort de ", "" );
        pTransporteurItem_                  = new QListViewItem( pStateListView_, pPionRenforceItem_, "Transporteur", "" );
        pTransportsItem_                    = new QListViewItem( pStateListView_, pTransporteurItem_, "Transportant", "" );
        pJammedStateItem_                   = new QListViewItem( pStateListView_, pTransportsItem_   , "Etat brouillage", "" );
        pBlackOutStateItem_                 = new QListViewItem( pStateListView_, pJammedStateItem_, "Etat silence radio", "" );
        pRadarStateItem_                    = new QListViewItem( pStateListView_, pBlackOutStateItem_, "Etat activation radar", "" );
        pSurrenderedStateItem_              = new QListViewItem( pStateListView_, pRadarStateItem_, "Pion rendu", "" );
        pPrisonerStateItem_                 = new QListViewItem( pStateListView_, pSurrenderedStateItem_, "Prisonnier", "" );
        pRefugeesManagedStateItem_          = new QListViewItem( pStateListView_, pPrisonerStateItem_, "Réfugiés pris en compte", "" );

        pAutomateModeItem_                  = 0;
       
        pRulesOfEngagementStateItem_ = new QListViewItem( pStateListView_, pRefugeesManagedStateItem_  , "Régles d'engagement", "None" );
        pFightRateStateItem_         = new QListViewItem( pStateListView_, pRulesOfEngagementStateItem_, "Rapport de force", "None" );
        pCloseCombatStateItem_       = new QListViewItem( pStateListView_, pFightRateStateItem_        , "Etat combat de rencontre", "None" );
        pOpStateItem_                = new QListViewItem( pStateListView_, pCloseCombatStateItem_      , "Etat opérationnel", "None" );

        pLogLinks_ = new QListView( pStateWidget, "State" );
        pLogLinks_->header()->hide();
        pLogLinks_->addColumn( "Name" );
        pLogLinks_->addColumn( "Value" );
        pLogLinks_->setMargin( 5 );
        pLogLinks_->setLineWidth( 2 );
        pLogLinks_->setFrameStyle( QFrame::Sunken | QFrame::Box );
        pLogLinks_->setSorting( -1, FALSE );
        pLayoutState->addWidget( pLogLinks_ );

        pTC2Item_                = new QListViewItem( pLogLinks_,                           "TC2"            , "-" );
        pLogMaintenanceSuperior_ = new QListViewItem( pLogLinks_, pTC2Item_               , "Maintenance"    , "-" );
        pLogMedicalSuperior_     = new QListViewItem( pLogLinks_, pLogMaintenanceSuperior_, "Sante"          , "-" );
        pLogSupplySuperior_      = new QListViewItem( pLogLinks_, pLogMedicalSuperior_    , "Ravitaillement" , "-" );       

//        pStateListView_->setMaximumHeight( 120 );

        // ACTIONS MAGIQUES - $$$$$ POURRI REFAIRE
        // MoveTo
        pMagicPos_ = new MOS_MagicPos( pStateWidget, "Action magique - MoveTo" );
        pMagicPos_->SetWidgetTab( pStateWidget );
        pMagicPos_->SetAttrEditor( this );
        pMagicPos_->Show( false );
        pLayoutState->addWidget( pMagicPos_ );

        // Surrender
        pButtonMagicSurrender_ = new QPushButton( "Action magique - SE RENDRE", pStateWidget );
        pLayoutState->addWidget( pButtonMagicSurrender_ );
        pButtonMagicSurrender_->hide();
        connect( pButtonMagicSurrender_, SIGNAL( clicked() ), this, SLOT( SlotMagicActionSurrender() ) );
                                                                               
        // Recompletement total
        pButtonMagicRecompletementTotal_ = new QPushButton( "Action magique - Recompletement Total", pStateWidget );
        pLayoutState->addWidget( pButtonMagicRecompletementTotal_ );
        pButtonMagicRecompletementTotal_->hide();
        connect( pButtonMagicRecompletementTotal_, SIGNAL( clicked() ), this, SLOT( SlotMagicActionRecompletementTotal() ) );

        // Recompletement Personnel
        pButtonMagicRecompletementPersonnel_ = new QPushButton( "Action magique - Recompletement Personnel", pStateWidget );
        pLayoutState->addWidget( pButtonMagicRecompletementPersonnel_ );
        pButtonMagicRecompletementPersonnel_->hide();
        connect( pButtonMagicRecompletementPersonnel_, SIGNAL( clicked() ), this, SLOT( SlotMagicActionRecompletementPersonnel() ) );

        // Recompletement Equipement
        pButtonMagicRecompletementEquipement_ = new QPushButton( "Action magique - Recompletement Equipement", pStateWidget );
        pLayoutState->addWidget( pButtonMagicRecompletementEquipement_ );
        pButtonMagicRecompletementEquipement_->hide();
        connect( pButtonMagicRecompletementEquipement_, SIGNAL( clicked() ), this, SLOT( SlotMagicActionRecompletementEquipement() ) );

        // Recompletement Ressources
        pButtonMagicRecompletementRessources_ = new QPushButton( "Action magique - Recompletement Ressources", pStateWidget );
        pLayoutState->addWidget( pButtonMagicRecompletementRessources_ );
        pButtonMagicRecompletementRessources_->hide();
        connect( pButtonMagicRecompletementRessources_, SIGNAL( clicked() ), this, SLOT( SlotMagicActionRecompletementRessources() ) );

        // Recompletement Partiel
        pButtonMagicRecompletementPartiel_ = new QPushButton( "Action magique - Recompletement Partiel", pStateWidget );
        pLayoutState->addWidget( pButtonMagicRecompletementPartiel_ );
        pButtonMagicRecompletementPartiel_->hide();
        connect( pButtonMagicRecompletementPartiel_, SIGNAL( clicked() ), this, SLOT( SlotMagicActionRecompletementPartiel() ) );

        // Destruction composante
        pButtonMagicDestroyComposante_ = new QPushButton( "Action magique - Destruction composante", pStateWidget );
        pLayoutState->addWidget( pButtonMagicDestroyComposante_ );
        pButtonMagicDestroyComposante_->hide();
        connect( pButtonMagicDestroyComposante_, SIGNAL( clicked() ), this, SLOT( SlotMagicActionDestroyComposante() ) );

        // Destruction TOUTES LES composante
        pButtonMagicDestroyAllComposantes_ = new QPushButton( "Action magique - Destruction totale", pStateWidget );
        pLayoutState->addWidget( pButtonMagicDestroyAllComposantes_ );
        pButtonMagicDestroyAllComposantes_->hide();
        connect( pButtonMagicDestroyAllComposantes_, SIGNAL( clicked() ), this, SLOT( SlotMagicActionDestroyAllComposantes() ) );

        // Changer facteurs humains
        pButtonMagicChangeHumanFactors_ = new QPushButton( "Changer facteurs humains", pStateWidget );
        pLayoutState->addWidget( pButtonMagicChangeHumanFactors_ );
        pButtonMagicChangeHumanFactors_->hide();
        connect( pButtonMagicChangeHumanFactors_, SIGNAL( clicked() ), this, SLOT( SlotMagicActionChangeHumanFactors() ) );

        // Changer liens logistiques
        pButtonChangeLogisticLinks_ = new QPushButton( "Changer liens logistiques", pStateWidget );
        pLayoutState->addWidget( pButtonChangeLogisticLinks_ );
        pButtonChangeLogisticLinks_->hide();
        connect( pButtonChangeLogisticLinks_, SIGNAL( clicked() ), this, SLOT( SlotMagicActionChangeLogisticLinks() ) );

        // Rav : changement de quota
        pButtonLogisticSupplyChangeQuotas_ = new QPushButton( "Log rav - Affecter quotas", pStateWidget );
        pLayoutState->addWidget( pButtonLogisticSupplyChangeQuotas_ );
        pButtonLogisticSupplyChangeQuotas_->hide();
        connect( pButtonLogisticSupplyChangeQuotas_, SIGNAL( clicked() ), this, SLOT( SlotMagicActionLogisticChangeQuotas() ) );

        // Rav : push flow
        pButtonLogisticSupplyPushFlow_ = new QPushButton( "Log rav - Pousser flux", pStateWidget );
        pLayoutState->addWidget( pButtonLogisticSupplyPushFlow_ );
        pButtonLogisticSupplyPushFlow_->hide();
        connect( pButtonLogisticSupplyPushFlow_, SIGNAL( clicked() ), this, SLOT( SlotMagicActionLogisticPushFlow() ) );

        // CLear Itineraire
        pButtonClearItineraire_ = new QPushButton( "Clear Itineraire", pStateWidget );
        pLayoutState->addWidget( pButtonClearItineraire_ );
        pButtonClearItineraire_ ->hide();
        connect( pButtonClearItineraire_, SIGNAL( clicked() ), this, SLOT( SlotClearItineraire() ) );

        // Ordre de conduite et de mission
        pButtonOrder_ = new QPushButton( "Conduite/Mission", pStateWidget );
        pLayoutState->addWidget( pButtonOrder_ );
        pButtonOrder_ ->hide();
        connect( pButtonOrder_, SIGNAL( clicked() ), this, SLOT( SlotOrder() ) );

        // Embraye/debrayer automate 
        pButtonChangeAutomateMode_= new QPushButton( "Embrayer/Debrayer automate", pStateWidget );
        pLayoutState->addWidget( pButtonChangeAutomateMode_ );
        pButtonChangeAutomateMode_->hide();
        connect( pButtonChangeAutomateMode_, SIGNAL( clicked() ), this, SLOT( SlotChangeAutomateMode() ) );
    }

    pTabWidget_->addTab( pStateWidget, "State" );

    // Tab Trace
    QWidget* pWidgetTrace = new QWidget( this );
    QVBoxLayout* pLayoutTrace = new QVBoxLayout( pWidgetTrace );
    {
        // RC
        pTraceEditor_ = new MOS_TraceEditor( *this, pWidgetTrace );
        pLayoutTrace->addWidget( pTraceEditor_ );
    }
    pTabWidget_->addTab( pWidgetTrace, "Trace" );

    // Tab Connaissances
    QWidget* pWidgetKnowledge = new QWidget( this );
    QVBoxLayout* pLayoutKnowledge = new QVBoxLayout( pWidgetKnowledge );
    {
        // RC
        pKnowledgeList_ = new MOS_KnowledgeList_View( pWidgetKnowledge, "Connaissances" );
        pLayoutKnowledge->addWidget( pKnowledgeList_ );

        pObjectKnowledgeList_ = new MOS_ObjectKnowledge_ListView( pWidgetKnowledge );
        pLayoutKnowledge->addWidget( pObjectKnowledgeList_ );

        // Draw compagnie knowledges
        pDrawKnowledge_ = new QCheckBox( "Afficher connaissances", pWidgetKnowledge );
        pDrawKnowledge_->setChecked( false );
        pLayoutKnowledge->addWidget( pDrawKnowledge_ );
        connect( pDrawKnowledge_, SIGNAL( toggled( bool ) ), this, SLOT( SlotDrawKnowledge( bool ) ) );
    }
    pTabWidget_->addTab( pWidgetKnowledge, "Connaissances" );

    // Tab Ressources
    pWidgetKnowledge = new QWidget( this );
    pLayoutKnowledge = new QVBoxLayout( pWidgetKnowledge );
    {
        pEquipementList_ = new QListView( pWidgetKnowledge, "Equipement" );
        pLayoutKnowledge->addWidget( pEquipementList_ );
        pEquipementList_->addColumn( "Equipement" );
        pEquipementList_->addColumn( "Disponible" );
        pEquipementList_->addColumn( "Indisponible" );
        pEquipementList_->addColumn( "Reparable" );
        pEquipementList_->addColumn( "En Maintenance" );
        pEquipementList_->addColumn( "Prisonnier" );
        pEquipementList_->setMargin( 5 );
        pEquipementList_->setLineWidth( 2 );
        pEquipementList_->setFrameStyle( QFrame::Sunken | QFrame::Box );

        pRessourcesList_ = new QListView( pWidgetKnowledge, "Ressources" );
        pLayoutKnowledge->addWidget( pRessourcesList_ );
        pRessourcesList_->addColumn( "Ressource" );
        pRessourcesList_->addColumn( "Quantité" );
        pRessourcesList_->setMargin( 5 );
        pRessourcesList_->setLineWidth( 2 );
        pRessourcesList_->setFrameStyle( QFrame::Sunken | QFrame::Box );

        pHumainsList_ = new QListView( pWidgetKnowledge, "Humains" );
        pLayoutKnowledge->addWidget( pHumainsList_ );
        pHumainsList_->addColumn( "Catégorie" );
        pHumainsList_->addColumn( "Officiers" );
        pHumainsList_->addColumn( "Ss officiers" );
        pHumainsList_->addColumn( "Mdr" );
        pHumainsList_->setMargin( 5 );
        pHumainsList_->setLineWidth( 2 );
        pHumainsList_->setFrameStyle( QFrame::Sunken | QFrame::Box );

        pLends_ = new QListView( pWidgetKnowledge, "Prets" );
        pLayoutKnowledge->addWidget( pLends_ );
        pLends_->addColumn( "Emprunteur" );
        pLends_->addColumn( "Type equipement" );
        pLends_->addColumn( "Nombre" );
        pLends_->setMargin( 5 );
        pLends_->setLineWidth( 2 );
        pLends_->setFrameStyle( QFrame::Sunken | QFrame::Box );
        pLends_->hide();
    }
    pTabWidget_->addTab( pWidgetKnowledge, "Ressources" );

    // Tab Logistic 
    QWidget*        pLogMaintenanceWidget = new QWidget( this );
    QVBoxLayout*    pLogMaintenanceLayout = new QVBoxLayout( pLogMaintenanceWidget );
    {
        pLogMaintenanceConsignListView_       = new MOS_LogMaintenanceConsign_ListView( pLogMaintenanceWidget, false );
        pLogMaintenanceConsignHandledListView_= new MOS_LogMaintenanceConsign_ListView( pLogMaintenanceWidget, true );
        pLogMaintenanceLayout->addWidget( pLogMaintenanceConsignListView_ );
        pLogMaintenanceLayout->addWidget( pLogMaintenanceConsignHandledListView_ );
        pLogMaintenanceConsignHandledListView_->hide();

        pLogMaintenanceState_ = new QListView( pLogMaintenanceWidget, "Etat chaine maintenance" );
        pLogMaintenanceState_->addColumn( "Etat" );
        pLogMaintenanceState_->addColumn( "" );
        pLogMaintenanceState_->setMargin( 5 );
        pLogMaintenanceState_->setLineWidth( 2 );
        pLogMaintenanceState_->setFrameStyle( QFrame::Sunken | QFrame::Box );
        pLogMaintenanceStateChainEnabled_      = new QListViewItem( pLogMaintenanceState_, "Etat chaine", " - " );
        pLogMaintenanceStateTempsBordee_       = new QListViewItem( pLogMaintenanceState_, pLogMaintenanceStateChainEnabled_, "Temps de bordée", " - " );
        pLogMaintenanceStatePriorites_         = new QListViewItem( pLogMaintenanceState_, pLogMaintenanceStateTempsBordee_ , "Priorités", " - " );  
        pLogMaintenanceStateTacticalPriorites_ = new QListViewItem( pLogMaintenanceState_, pLogMaintenanceStatePriorites_   , "Priorités tactiques", " - " );  
        
        pLogMaintenanceState_->hide();
        pLogMaintenanceLayout->addWidget( pLogMaintenanceState_ );

        pLogMaintenanceDispoHaulers_ = new QListView( pLogMaintenanceWidget, "Disponibilités remorqueurs" );
        pLogMaintenanceDispoHaulers_->addColumn( "Disponibilités remorqueurs" );
        pLogMaintenanceDispoHaulers_->addColumn( "" );
        pLogMaintenanceDispoHaulers_->setMargin( 5 );
        pLogMaintenanceDispoHaulers_->setLineWidth( 2 );
        pLogMaintenanceDispoHaulers_->setFrameStyle( QFrame::Sunken | QFrame::Box );
        pLogMaintenanceDispoHaulers_->setSorting( -1, FALSE );
        pLogMaintenanceDispoHaulers_->hide();
        pLogMaintenanceLayout->addWidget( pLogMaintenanceDispoHaulers_ );
        

        pLogMaintenanceDispoRepairers_ = new QListView( pLogMaintenanceWidget, "Disponibilités réparateurs" );
        pLogMaintenanceDispoRepairers_->addColumn( "Disponibilités réparateurs" );
        pLogMaintenanceDispoRepairers_->addColumn( "" );
        pLogMaintenanceDispoRepairers_->setMargin( 5 );
        pLogMaintenanceDispoRepairers_->setLineWidth( 2 );
        pLogMaintenanceDispoRepairers_->setFrameStyle( QFrame::Sunken | QFrame::Box );
        pLogMaintenanceDispoRepairers_->setSorting( -1, FALSE );
        pLogMaintenanceDispoRepairers_->hide();
        pLogMaintenanceLayout->addWidget( pLogMaintenanceDispoRepairers_ );
    }

    pTabWidget_->addTab( pLogMaintenanceWidget, "LOG Maintenance" );

    // Tab Logistic 
    QWidget*        pLogMedicalWidget = new QWidget( this );
    QVBoxLayout*    pLogMedicalLayout = new QVBoxLayout( pLogMedicalWidget );
    {
        pLogMedicalConsignListView_       = new MOS_LogMedicalConsign_ListView( pLogMedicalWidget, false );
        pLogMedicalConsignHandledListView_= new MOS_LogMedicalConsign_ListView( pLogMedicalWidget, true );
        pLogMedicalLayout->addWidget( pLogMedicalConsignListView_ );
        pLogMedicalLayout->addWidget( pLogMedicalConsignHandledListView_ );
        pLogMedicalConsignHandledListView_->hide();

        pLogMedicalState_ = new QListView( pLogMedicalWidget, "Etat chaine medical" );
        pLogMedicalState_->addColumn( "Etat" );
        pLogMedicalState_->addColumn( "" );
        pLogMedicalState_->setMargin( 5 );
        pLogMedicalState_->setLineWidth( 2 );
        pLogMedicalState_->setFrameStyle( QFrame::Sunken | QFrame::Box );
        pLogMedicalStateChainEnabled_      = new QListViewItem( pLogMedicalState_, "Etat chaine", " - " );
        pLogMedicalStateTempsBordee_       = new QListViewItem( pLogMedicalState_, pLogMedicalStateChainEnabled_, "Temps de bordée", " - " );
        pLogMedicalStatePriorites_         = new QListViewItem( pLogMedicalState_, pLogMedicalStateTempsBordee_, "Priorités", " - " );  
        pLogMedicalStateTacticalPriorites_ = new QListViewItem( pLogMedicalState_, pLogMedicalStatePriorites_, "Priorités tactiques", " - " );  
        pLogMedicalState_->hide();
        pLogMedicalLayout->addWidget( pLogMedicalState_ );
       
        pLogMedicalDispoReleveAmbulances_ = new QListView( pLogMedicalWidget, "Disponibilités ambulances relève" );
        pLogMedicalDispoReleveAmbulances_->addColumn( "Disponibilités ambulances relève" );
        pLogMedicalDispoReleveAmbulances_->addColumn( "" );
        pLogMedicalDispoReleveAmbulances_->setMargin( 5 );
        pLogMedicalDispoReleveAmbulances_->setLineWidth( 2 );
        pLogMedicalDispoReleveAmbulances_->setFrameStyle( QFrame::Sunken | QFrame::Box );
        pLogMedicalDispoReleveAmbulances_->setSorting( -1, FALSE );
        pLogMedicalDispoReleveAmbulances_->hide();
        pLogMedicalLayout->addWidget( pLogMedicalDispoReleveAmbulances_ );

        pLogMedicalDispoRamassageAmbulances_ = new QListView( pLogMedicalWidget, "Disponibilités ambulances ramassage" );
        pLogMedicalDispoRamassageAmbulances_->addColumn( "Disponibilités ambulances ramassage" );
        pLogMedicalDispoRamassageAmbulances_->addColumn( "" );
        pLogMedicalDispoRamassageAmbulances_->setMargin( 5 );
        pLogMedicalDispoRamassageAmbulances_->setLineWidth( 2 );
        pLogMedicalDispoRamassageAmbulances_->setFrameStyle( QFrame::Sunken | QFrame::Box );
        pLogMedicalDispoRamassageAmbulances_->setSorting( -1, FALSE );
        pLogMedicalDispoRamassageAmbulances_->hide();
        pLogMedicalLayout->addWidget( pLogMedicalDispoRamassageAmbulances_ );

        pLogMedicalDispoDoctors_ = new QListView( pLogMedicalWidget, "Disponibilités médecins" );
        pLogMedicalDispoDoctors_->addColumn( "Disponibilités médecins" );
        pLogMedicalDispoDoctors_->addColumn( "" );
        pLogMedicalDispoDoctors_->setMargin( 5 );
        pLogMedicalDispoDoctors_->setLineWidth( 2 );
        pLogMedicalDispoDoctors_->setFrameStyle( QFrame::Sunken | QFrame::Box );
        pLogMedicalDispoDoctors_->setSorting( -1, FALSE );
        pLogMedicalDispoDoctors_->hide();
        pLogMedicalLayout->addWidget( pLogMedicalDispoDoctors_ );
    }

    pTabWidget_->addTab( pLogMedicalWidget, "LOG Santé" );

    // Tab Logistic 
    QWidget*        pLogSupplyWidget = new QWidget( this );
    QVBoxLayout*    pLogSupplyLayout = new QVBoxLayout( pLogSupplyWidget );
    {
        pLogSupplyConsignListView_       = new MOS_LogSupplyConsign_ListView( pLogSupplyWidget, false );
        pLogSupplyConsignHandledListView_= new MOS_LogSupplyConsign_ListView( pLogSupplyWidget, true );
        pLogSupplyLayout->addWidget( pLogSupplyConsignListView_ );
        pLogSupplyLayout->addWidget( pLogSupplyConsignHandledListView_ );
        pLogSupplyConsignHandledListView_->hide();

        pLogSupplyState_ = new QListView( pLogSupplyWidget, "Etat chaine rav" );
        pLogSupplyState_->addColumn( "Etat" );
        pLogSupplyState_->addColumn( "" );
        pLogSupplyState_->setMargin( 5 );
        pLogSupplyState_->setLineWidth( 2 );
        pLogSupplyState_->setFrameStyle( QFrame::Sunken | QFrame::Box );
        pLogSupplyStateChainEnabled_ = new QListViewItem( pLogSupplyState_, "Etat chaine", " - " );
        pLogSupplyState_->hide();
        pLogSupplyLayout->addWidget( pLogSupplyState_ );

        pLogSupplyStocks_ = new QListView( pLogSupplyWidget, "Stocks" );
        pLogSupplyStocks_->addColumn( "Stock" );
        pLogSupplyStocks_->addColumn( "Quantité" );
        pLogSupplyStocks_->setMargin( 5 );
        pLogSupplyStocks_->setLineWidth( 2 );
        pLogSupplyStocks_->setFrameStyle( QFrame::Sunken | QFrame::Box );
        pLogSupplyLayout->addWidget( pLogSupplyStocks_ );
        pLogSupplyStocks_->hide();

        pLogSupplyQuotas_ = new QListView( pLogSupplyWidget, "Quotas" );
        pLogSupplyQuotas_->addColumn( "Type" );
        pLogSupplyQuotas_->addColumn( "Quota" );
        pLogSupplyQuotas_->setMargin( 5 );
        pLogSupplyQuotas_->setLineWidth( 2 );
        pLogSupplyQuotas_->setFrameStyle( QFrame::Sunken | QFrame::Box );
        pLogSupplyLayout->addWidget( pLogSupplyQuotas_ );
        pLogSupplyQuotas_->hide();
       
        pLogSupplyDispoTransporters_ = new QListView( pLogSupplyWidget, "Disponibilités transporteurs pour convois" );
        pLogSupplyDispoTransporters_->addColumn( "Disponibilités transporteurs pour convois" );
        pLogSupplyDispoTransporters_->addColumn( "" );
        pLogSupplyDispoTransporters_->setMargin( 5 );
        pLogSupplyDispoTransporters_->setLineWidth( 2 );
        pLogSupplyDispoTransporters_->setFrameStyle( QFrame::Sunken | QFrame::Box );
        pLogSupplyDispoTransporters_->setSorting( -1, FALSE );
        pLogSupplyDispoTransporters_->hide();
        pLogSupplyLayout->addWidget( pLogSupplyDispoTransporters_ );

        pLogSupplyDispoCommanders_ = new QListView( pLogSupplyWidget, "Disponibilités chefs de convois" );
        pLogSupplyDispoCommanders_->addColumn( "Disponibilités chefs de convois" );
        pLogSupplyDispoCommanders_->addColumn( "" );
        pLogSupplyDispoCommanders_->setMargin( 5 );
        pLogSupplyDispoCommanders_->setLineWidth( 2 );
        pLogSupplyDispoCommanders_->setFrameStyle( QFrame::Sunken | QFrame::Box );
        pLogSupplyDispoCommanders_->setSorting( -1, FALSE );
        pLogSupplyDispoCommanders_->hide();
        pLogSupplyLayout->addWidget( pLogSupplyDispoCommanders_ );

    }

    pTabWidget_->addTab( pLogSupplyWidget, "LOG Rav" );


    /*
        , pLogMedicalConsignListView( 0 )
    , pLogMedicalConsignHandledListView( 0 )
    , pLogMedicalState( 0 )
    , pLogMedicalStateChainEnabled( 0 )
    , pLogMedicalStateTempsBordee( 0 )
    , pLogMedicalStatePriorites( 0 )
    
    */

/*    // Tab Transport 
    pWidgetKnowledge = new QWidget( this );
    pLayoutKnowledge = new QVBoxLayout( pWidgetKnowledge );
    {
        pTransportCapacityList_ = new QListView( pWidgetKnowledge, "Capacite" );
        pLayoutKnowledge->addWidget( pTransportCapacityList_ );
        pTransportCapacityList_->addColumn( "Type dotation" );
        pTransportCapacityList_->addColumn( "Pourcentage transporteur" );
        pTransportCapacityList_->setMargin( 5 );
        pTransportCapacityList_->setLineWidth( 2 );
        pTransportCapacityList_->setFrameStyle( QFrame::Sunken | QFrame::Box );

        pTransportEquipementList_ = new QListView( pWidgetKnowledge, "Equipement" );
        pLayoutKnowledge->addWidget( pTransportEquipementList_ );
        pTransportEquipementList_->addColumn( "Equipement" );
        pTransportEquipementList_->addColumn( "Panne" );
        pTransportEquipementList_->addColumn( "NTI" );
        pTransportEquipementList_->addColumn( "Nombre" );
        pTransportEquipementList_->setMargin( 5 );
        pTransportEquipementList_->setLineWidth( 2 );
        pTransportEquipementList_->setFrameStyle( QFrame::Sunken | QFrame::Box );

        pTransportRessourcesList_ = new QListView( pWidgetKnowledge, "Ressources" );
        pLayoutKnowledge->addWidget( pTransportRessourcesList_ );
        pTransportRessourcesList_->addColumn( "Ressource" );
        pTransportRessourcesList_->addColumn( "Disponible" );
        pTransportRessourcesList_->addColumn( "En transit" );
        pTransportRessourcesList_->setMargin( 5 );
        pTransportRessourcesList_->setLineWidth( 2 );
        pTransportRessourcesList_->setFrameStyle( QFrame::Sunken | QFrame::Box );


        pTransportHumainsList_ = new QListView( pWidgetKnowledge, "Humains" );
        pLayoutKnowledge->addWidget( pTransportHumainsList_ );
        pTransportHumainsList_->addColumn( "Catégorie" );
        pTransportHumainsList_->addColumn( "O" );
        pTransportHumainsList_->addColumn( "SO" );
        pTransportHumainsList_->addColumn( "MDR" );
        pTransportHumainsList_->setMargin( 5 );
        pTransportHumainsList_->setLineWidth( 2 );
        pTransportHumainsList_->setFrameStyle( QFrame::Sunken | QFrame::Box );
    }

    pTabWidget_->addTab( pWidgetKnowledge, "Emports" );*/

    // Button Group OK/CANCEL
    pButtonGroup_ = new QButtonGroup( 1, Vertical, this );
    QPushButton* pCancelPushButton = new QPushButton( pButtonGroup_ );
    pCancelPushButton->setText( "Cancel" );
    pCancelPushButton->setAccel( Key_Escape );
    QPushButton* pOkPushButton = new QPushButton( pButtonGroup_ );
    pOkPushButton->setText( "Send To Sim" );
    QHBoxLayout* pButtonLayout = new QHBoxLayout( pLayout );
    pButtonLayout->addWidget( pButtonGroup_ );
    pButtonGroup_->hide();

    connect( pOkPushButton, SIGNAL( clicked() ), this, SLOT( SlotAccept() ) );
    connect( pCancelPushButton, SIGNAL( clicked() ), this, SLOT( SlotReject() ) );

    setFocusPolicy( QWidget::ClickFocus );
}

//-----------------------------------------------------------------------------
// Name: MOS_AttrEditor destructor
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
MOS_AttrEditor::~MOS_AttrEditor()
{

}

//-----------------------------------------------------------------------------
// Name: MOS_AttrEditor::Initialize
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_AttrEditor::Initialize()
{
    if( pMagicPos_ )
        pMagicPos_->Initialize();

    if( pRCEditor_ )
        pRCEditor_->Initialize();

    if( pTraceEditor_ )
        pTraceEditor_->Initialize();

    // Order de conduite/Mission
    pOrderConduiteMissionDlg_ = new MOS_OrderConduiteMissionDialog( &MOS_App::GetApp().GetMainWindow() );
    pOrderConduiteMissionDlg_->hide();

    pChangeHumanFactorsDialog_ = new MOS_ChangeHumanFactorsDialog( &MOS_App::GetApp().GetMainWindow() ); 
    pChangeHumanFactorsDialog_->hide();

    pChangeLogisticLinksDialog_ = new MOS_ChangeLogisticLinksDialog( &MOS_App::GetApp().GetMainWindow() ); 
    pChangeLogisticLinksDialog_->hide();

    pLogisticSupplyChangeQuotasDialog_ = new MOS_LogisticSupplyChangeQuotasDialog( &MOS_App::GetApp().GetMainWindow() );
    pLogisticSupplyChangeQuotasDialog_->hide();

    pLogisticSupplyPushFlowDialog_ = new MOS_LogisticSupplyPushFlowDialog( &MOS_App::GetApp().GetMainWindow() );
    pLogisticSupplyPushFlowDialog_ ->hide();

    pLogisticSupplyRecompletionDialog_ = new MOS_LogisticSupplyRecompletionDialog( &MOS_App::GetApp().GetMainWindow() );
    pLogisticSupplyRecompletionDialog_->hide();

    if( pKnowledgeList_ != 0 )
        pKnowledgeList_->Initialize();
}

//-----------------------------------------------------------------------------
// Name: MOS_AttrEditor::terminate
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_AttrEditor::Terminate()
{
    if( pMagicPos_ )
        pMagicPos_->Terminate();

    if( pRCEditor_ )
        pRCEditor_->Terminate();

    if( pTraceEditor_ )
        pTraceEditor_->Terminate();
}

//-----------------------------------------------------------------------------
// Name: MOS_AttrEditor::IsActive
// Created: FBD 03-01-10
//-----------------------------------------------------------------------------
bool MOS_AttrEditor::IsActive( bool /*bMove*/ )
{
    if( pWidgetStack_ )
    {
        if( pWidgetStack_->visibleWidget() == (QWidget*)this )
            return true;
    }
    return false;
}
//-----------------------------------------------------------------------------
// Name: MOS_AttrEditor::IsActiveTab
// Created: FBD 03-01-20
//-----------------------------------------------------------------------------
bool MOS_AttrEditor::IsActiveTab( QWidget* pWidget )
{
    assert( pTabWidget_ );
    if( pTabWidget_->currentPage() == pWidget )
        return true;
    return false;
}


//-----------------------------------------------------------------------------
// Name: MOS_AttrEditor::WriteMsg
// Created: FBD 03-01-10
//-----------------------------------------------------------------------------
void MOS_AttrEditor::WriteMsg( std::string& sParam )
{
    if( !pAgent_ )
        return;

    if( pMagicPos_ && pMagicPos_->IsModifiedAgent() )
        pMagicPos_->WriteMsg( sParam );
}

//-----------------------------------------------------------------------------
// Name: MOS_AttrEditor::SetAgent
// Created: FBD 03-01-15
//-----------------------------------------------------------------------------
void MOS_AttrEditor::SetAgent( MOS_Agent* pAgent )
{
    if( pAgent_ != 0 )
        pAgent_->SetAttributeEditor( 0 );

    pAgent_ = pAgent;

    pMagicPos_->SetAgent( pAgent );
    pRCEditor_->SetAgent( pAgent );
    pTraceEditor_->SetAgent( pAgent );

    pButtonChangeAutomateMode_->hide();
    pDrawKnowledge_->hide();

    if( pAgent_ )
    {
        pButtonGroup_->show();
        pButtonClearItineraire_->show();
        pButtonOrder_->show();
        pButtonMagicChangeHumanFactors_->show();
        pButtonMagicDestroyComposante_->show();
        pButtonMagicRecompletementTotal_->show();
        pButtonMagicRecompletementPersonnel_->show();
        pButtonMagicRecompletementEquipement_->show();
        pButtonMagicRecompletementRessources_->show();
        pButtonMagicRecompletementPartiel_->show();
        pButtonMagicDestroyAllComposantes_->show();
        pAgent_->SetAttributeEditor( this );
        pDrawKnowledge_->show();
        if( pAgent_->IsAutomate() )
        {
            pButtonChangeLogisticLinks_->show();
            pButtonChangeAutomateMode_->show();
            pButtonMagicSurrender_->show();
            pDrawKnowledge_->setChecked( pAgent->GetGtia().ShouldDrawKnowledges() );
            if( pAgent_->IsSupplyPionLog() )
            {
                pButtonLogisticSupplyChangeQuotas_->show();
                pButtonLogisticSupplyPushFlow_    ->show();
            }
            pLogLinks_->show();
        }
        else 
            pLogLinks_->hide();
    }
    else
    {
        pButtonMagicSurrender_->hide();
        pButtonGroup_->hide();
        pButtonClearItineraire_->hide();
        pButtonOrder_->hide();
        pButtonMagicDestroyComposante_->hide();
        pButtonMagicRecompletementTotal_->hide();
        pButtonMagicRecompletementPersonnel_->hide();
        pButtonMagicRecompletementEquipement_->hide();
        pButtonMagicRecompletementRessources_->hide();    
        pButtonMagicChangeHumanFactors_->hide();
        pButtonChangeLogisticLinks_->hide();
        pButtonMagicDestroyAllComposantes_->hide();
        pButtonLogisticSupplyChangeQuotas_->hide();
        pButtonLogisticSupplyPushFlow_    ->hide();
    }

    if( pAgent )
        pKnowledgeList_->SetGtia( & pAgent->GetGtia() );
    else
        pKnowledgeList_->SetGtia( 0 );

    if( pAgent )
        pObjectKnowledgeList_->SetTeam( & pAgent->GetTeam() );
    else
        pObjectKnowledgeList_->SetTeam( 0 );
}

//-----------------------------------------------------------------------------
// Name: MOS_AttrEditor::SlotClearItineraire
// Created: FBD 03-01-29
//-----------------------------------------------------------------------------
void MOS_AttrEditor::SlotClearItineraire()
{
    if( pAgent_ )
        pAgent_->ClearPathFind();
}

//-----------------------------------------------------------------------------
// Name: MOS_AttrEditor::SlotClearItineraire
// Created: FBD 03-01-29
//-----------------------------------------------------------------------------
void MOS_AttrEditor::SlotOrder()
{
    if( pOrderConduiteMissionDlg_ )
    {
        pOrderConduiteMissionDlg_->Initialize( *pAgent_ );
        pOrderConduiteMissionDlg_->show();
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_AttrEditor::SlotChangeAutomateMode
// Created: NLD 2003-04-11
//-----------------------------------------------------------------------------
void MOS_AttrEditor::SlotChangeAutomateMode()
{
    if( pAgent_ )
    {
        MOS_ASN_MsgSetAutomateMode asnMsg;
        asnMsg.GetAsnMsg().unit_id = pAgent_->GetAgentID();
        asnMsg.GetAsnMsg().mode    = pAgent_->IsEmbraye() ? EnumAutomateState::debraye : EnumAutomateState::embraye;
        asnMsg.Send( 54 );
    }
}

// -----------------------------------------------------------------------------
// Name: MOS_AttrEditor::SlotMagicActionDestroyComposante
// Created: NLD 2004-03-01
// -----------------------------------------------------------------------------
void MOS_AttrEditor::SlotMagicActionDestroyComposante()
{
    MOS_App::GetApp().GetMOSServer().GetController().GetMessageMgr().SendMsgUnitMagicActionDestroyComposante( *pAgent_ );
}

// -----------------------------------------------------------------------------
// Name: MOS_AttrEditor::SlotMagicActionDestroyAllComposantes
// Created: NLD 2004-03-01
// -----------------------------------------------------------------------------
void MOS_AttrEditor::SlotMagicActionDestroyAllComposantes()
{
    assert( pAgent_ );
    MOS_ASN_MsgUnitMagicAction asnMsg;
    asnMsg.GetAsnMsg().oid      = pAgent_->GetAgentID();
    asnMsg.GetAsnMsg().action.t = T_MsgUnitMagicAction_action_destruction_totale;
    asnMsg.Send( 561 );
}

//-----------------------------------------------------------------------------
// Name: MOS_AttrEditor::SlotMagicActionRecompletementTotal
// Created: NLD 2003-07-11
//-----------------------------------------------------------------------------
void MOS_AttrEditor::SlotMagicActionRecompletementTotal()
{
    assert( pAgent_ );
    MOS_ASN_MsgUnitMagicAction asnMsg;
    asnMsg.GetAsnMsg().oid              = pAgent_->GetAgentID();
    asnMsg.GetAsnMsg().action.t         = T_MsgUnitMagicAction_action_recompletement_total;
    asnMsg.Send( 56 );
}



//-----------------------------------------------------------------------------
// Name: MOS_AttrEditor::SlotMagicActionRecompletementPersonnel
// Created: NLD 2003-07-11
//-----------------------------------------------------------------------------
void MOS_AttrEditor::SlotMagicActionRecompletementPersonnel()
{
    assert( pAgent_ );
    MOS_ASN_MsgUnitMagicAction asnMsg;
    asnMsg.GetAsnMsg().oid              = pAgent_->GetAgentID();
    asnMsg.GetAsnMsg().action.t         = T_MsgUnitMagicAction_action_recompletement_personnel;
    asnMsg.Send( 56 );
}


//-----------------------------------------------------------------------------
// Name: MOS_AttrEditor::SlotMagicActionRecompletementEquipement
// Created: NLD 2003-07-11
//-----------------------------------------------------------------------------
void MOS_AttrEditor::SlotMagicActionRecompletementEquipement()
{
    assert( pAgent_ );
    MOS_ASN_MsgUnitMagicAction asnMsg;
    asnMsg.GetAsnMsg().oid              = pAgent_->GetAgentID();
    asnMsg.GetAsnMsg().action.t         = T_MsgUnitMagicAction_action_recompletement_equipement;
    asnMsg.Send( 56 );
}


//-----------------------------------------------------------------------------
// Name: MOS_AttrEditor::SlotMagicActionRecompletementRessources
// Created: NLD 2003-07-11
//-----------------------------------------------------------------------------
void MOS_AttrEditor::SlotMagicActionRecompletementRessources()
{
    assert( pAgent_ );
    MOS_ASN_MsgUnitMagicAction asnMsg;
    asnMsg.GetAsnMsg().oid              = pAgent_->GetAgentID();
    asnMsg.GetAsnMsg().action.t         = T_MsgUnitMagicAction_action_recompletement_ressources;
    asnMsg.Send( 56 );
}

// -----------------------------------------------------------------------------
// Name: MOS_AttrEditor::SlotMagicActionRecompletementPartiel
// Created: NLD 2003-07-11
//-----------------------------------------------------------------------------
void MOS_AttrEditor::SlotMagicActionRecompletementPartiel()
{
    assert( pAgent_ );
    assert( pLogisticSupplyRecompletionDialog_ );
    pLogisticSupplyRecompletionDialog_->SetAgent( *pAgent_ );
    pLogisticSupplyRecompletionDialog_->show();
}

// -----------------------------------------------------------------------------
// Name: MOS_AttrEditor::SlotMagicActionSurrender
// Created: NLD 2005-03-04
// -----------------------------------------------------------------------------
void MOS_AttrEditor::SlotMagicActionSurrender()
{
    assert( pAgent_ );

    MOS_ASN_MsgUnitMagicAction asnMsg;
    asnMsg.GetAsnMsg().oid                = pAgent_->GetAgentID();
    asnMsg.GetAsnMsg().action.t           = T_MsgUnitMagicAction_action_se_rendre;
    asnMsg.Send( 547 );
}

//-----------------------------------------------------------------------------
// Name: MOS_AttrEditor::SlotDrawKnowledge
// Created: AGN 03-04-03
//-----------------------------------------------------------------------------
void MOS_AttrEditor::SlotDrawKnowledge( bool bValue )
{
    if( pAgent_ )
    {
        pAgent_->GetGtia().NotifyShouldDrawKnowledges( bValue );
        pAgent_->GetTeam().NotifyShouldDrawKnowledges( bValue );
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_AttrEditor::SlotAccept
// Created: FBD 02-11-29
//-----------------------------------------------------------------------------
void MOS_AttrEditor::SlotAccept()
{
    Accept();
}


//-----------------------------------------------------------------------------
// Name: MOS_AttrEditor::Accept
// Created: FBD 03-01-15
//-----------------------------------------------------------------------------
void MOS_AttrEditor::Accept()
{
    std::string sParam;
    WriteMsg( sParam );
}
//-----------------------------------------------------------------------------
// Name: MOS_AttrEditor::SlotReject
// Created: FBD 02-12-03
//-----------------------------------------------------------------------------
void MOS_AttrEditor::SlotReject()
{
    Cancel();
}

//-----------------------------------------------------------------------------
// Name: MOS_AttrEditor::NotifyAgentHasMoved
// Created: FBD 03-01-20
//-----------------------------------------------------------------------------
void MOS_AttrEditor::NotifyAgentHasMoved( MOS_Agent& agent, const MT_Vector2D& vPos )
{
    if( pMagicPos_ )
        pMagicPos_->NotifyAgentHasMoved( agent, vPos );
}


//-----------------------------------------------------------------------------
// Name: MOS_AttrEditor::DeleteRCItem
// Created: FBD 03-01-27
//-----------------------------------------------------------------------------
QListViewItem& MOS_AttrEditor::CreateRCItem( MOS_RC& rc )
{
    assert( pRCEditor_ );
    return *pRCEditor_->GetRCListView().CreateRC( rc );
}
//-----------------------------------------------------------------------------
// Name: MOS_AttrEditor::DeleteRCItem
// Created: FBD 03-01-27
//-----------------------------------------------------------------------------
void MOS_AttrEditor::DeleteRCItem( QListViewItem& rcItem )
{
    assert( pRCEditor_ );
    pRCEditor_->GetRCListView().DeleteRC( rcItem );
}


//-----------------------------------------------------------------------------
// Name: MOS_AttrEditor::DeleteTraceItem
// Created: FBD 03-01-27
//-----------------------------------------------------------------------------
QListViewItem& MOS_AttrEditor::CreateTraceItem( MOS_Trace& Trace )
{
    assert( pTraceEditor_ );
    return *pTraceEditor_->GetTraceListView().CreateTrace( Trace );
}
//-----------------------------------------------------------------------------
// Name: MOS_AttrEditor::DeleteTraceItem
// Created: FBD 03-01-27
//-----------------------------------------------------------------------------
void MOS_AttrEditor::DeleteTraceItem( QListViewItem& TraceItem )
{
    assert( pTraceEditor_ );
    pTraceEditor_->GetTraceListView().DeleteTrace( TraceItem );
}

//-----------------------------------------------------------------------------
// Name: MOS_AttrEditor::ResetEquipments
// Created: AGN 03-06-04
//-----------------------------------------------------------------------------
void MOS_AttrEditor::ResetEquipments()
{
    assert( pEquipementList_ != 0 );
    pEquipementList_->clear();
}


//-----------------------------------------------------------------------------
// Name: MOS_AttrEditor::ResetTransportEquipments
// Created: AGN 03-06-04
//-----------------------------------------------------------------------------
//void MOS_AttrEditor::ResetTransportEquipments()
//{
//    if ( pTransportEquipementList_ )
//        pTransportEquipementList_->clear();
//}



//-----------------------------------------------------------------------------
// Name: MOS_AttrEditor::ResetRessources
// Created: AGN 03-06-04
//-----------------------------------------------------------------------------
void MOS_AttrEditor::ResetRessources()
{
    if ( pRessourcesList_ )
       pRessourcesList_->clear();
}


//-----------------------------------------------------------------------------
// Name: MOS_AttrEditor::ResetTransportRessources
// Created: AGN 03-06-04
//-----------------------------------------------------------------------------
//void MOS_AttrEditor::ResetTransportRessources()
//{
//    if ( pTransportRessourcesList_ )
//        pTransportRessourcesList_->clear();
//}



//-----------------------------------------------------------------------------
// Name: MOS_AttrEditor::ResetHumains
// Created: AGN 03-06-04
//-----------------------------------------------------------------------------
void MOS_AttrEditor::ResetHumains()
{
    if ( pHumainsList_ )
        pHumainsList_->clear();
}

//-----------------------------------------------------------------------------
// Name: MOS_AttrEditor::ResetTransportHumains
// Created: AGN 03-06-04
//-----------------------------------------------------------------------------
//void MOS_AttrEditor::ResetTransportHumains()
//{
//    if ( pTransportHumainsList_ )
//        pTransportHumainsList_->clear();
//}



//-----------------------------------------------------------------------------
// Name: MOS_AttrEditor::ResetTransportCapacities
// Created: JVT 04-04-01
//-----------------------------------------------------------------------------
//void MOS_AttrEditor::ResetTransportCapacities()
//{
//    if ( pTransportCapacityList_ )
//        pTransportCapacityList_->clear();
//}



//-----------------------------------------------------------------------------
// Name: MOS_AttrEditor::AddEquipment
// Created: AGN 03-06-04
//-----------------------------------------------------------------------------
void MOS_AttrEditor::AddEquipment( MIL_AgentID nType, MOS_Agent::T_EquipementState nState )
{
    assert( pEquipementList_ != 0 );
    QListViewItem* pItem = new QListViewItem( pEquipementList_ );
    pItem->setText( 0, MOS_App::GetApp().GetEquipementName( nType ).c_str() );
    pItem->setText( 1, MT_FormatString( "%d", nState.nNbrAvailable_).c_str() );
    pItem->setText( 2, MT_FormatString( "%d", nState.nNbrUnavailable_ ).c_str() );
    pItem->setText( 3, MT_FormatString( "%d", nState.nNbrReparable_ ).c_str() );
    pItem->setText( 4, MT_FormatString( "%d", nState.nNbrInMaintenance_ ).c_str() );
    pItem->setText( 5, MT_FormatString( "%d", nState.nNbrPrisoner_ ).c_str() );
}

//-----------------------------------------------------------------------------
// Name: MOS_AttrEditor::AddTransportEquipment
// Created: AGN 03-06-04
//-----------------------------------------------------------------------------
//void MOS_AttrEditor::AddTransportEquipment( MIL_AgentID nType, MOS_Agent::T_EquipementState nState )
//{
//    assert( pTransportEquipementList_ != 0 );
//    QListViewItem* pItem = new QListViewItem( pTransportEquipementList_ );
//    pItem->setText( 0, MOS_App::GetApp().GetEquipementName( nType ).c_str() );
//    pItem->setText( 1, MT_FormatString( "%d", nState.nNbrAvailable_).c_str() );
//    pItem->setText( 2, MT_FormatString( "%d", nState.nNbrUnavailable_ ).c_str() );
//    pItem->setText( 3, MT_FormatString( "%d", nState.nNbrReparable_ ).c_str() );
//}
//


//-----------------------------------------------------------------------------
// Name: MOS_AttrEditor::AddRessource
// Created: AGN 03-06-04
//-----------------------------------------------------------------------------
void MOS_AttrEditor::AddRessource( MIL_AgentID nType, uint nQty )
{
    assert( pRessourcesList_ != 0 );
    QListViewItem* pItem = new QListViewItem( pRessourcesList_ );
    pItem->setText( 0, MOS_App::GetApp().GetRessourceName( nType ).c_str() );
    pItem->setText( 1, MT_FormatString( "%d", nQty ).c_str() );
}



//-----------------------------------------------------------------------------
// Name: MOS_AttrEditor::AddTransportRessource
// Created: AGN 03-06-04
//-----------------------------------------------------------------------------
//void MOS_AttrEditor::AddTransportRessource( MIL_AgentID nType, uint nQty )
//{
//    assert( pTransportRessourcesList_ != 0 );
//    QListViewItem* pItem = new QListViewItem( pTransportRessourcesList_ );
//    pItem->setText( 0, MOS_App::GetApp().GetRessourceName( nType ).c_str() );
//    pItem->setText( 2, MT_FormatString( "%d", nQty ).c_str() );
//}


//-----------------------------------------------------------------------------
// Name: MOS_AttrEditor::AddTransportRessource
// Created: JVT 04-03-31
//-----------------------------------------------------------------------------
//void MOS_AttrEditor::AddTransportRessource( const char* name, uint nQty )
//{
//    assert( pTransportRessourcesList_ );
//    QListViewItem* pItem = new QListViewItem( pTransportRessourcesList_ );
//    pItem->setText( 0, name );
//    pItem->setText( 2, MT_FormatString( "%d", nQty ).c_str() );
//}


//-----------------------------------------------------------------------------
// Name: MOS_AttrEditor::AddTransportRessource
// Created: JVT 04-04-08
//-----------------------------------------------------------------------------
//void MOS_AttrEditor::AddTransportRessource( MIL_AgentID nType, uint nQtyAvailable, uint nQtyTransit )
//{
//    assert( pTransportRessourcesList_ );
//    QListViewItem* pItem = new QListViewItem( pTransportRessourcesList_ );
//    pItem->setText( 0, MOS_App::GetApp().GetRessourceName( nType ).c_str() );
//    pItem->setText( 1, MT_FormatString( "%d", nQtyAvailable ).c_str() );
//    pItem->setText( 2, MT_FormatString( "%d", nQtyTransit ).c_str() );
//}

//-----------------------------------------------------------------------------
// Name: MOS_AttrEditor::UpdateTransportRessource
// Created: JVT 04-03-31
//-----------------------------------------------------------------------------
//void MOS_AttrEditor::UpdateTransportRessource( MIL_AgentID nType, uint nQty )
//{
//    assert( pTransportRessourcesList_ );
//    QListViewItem* pItem = pTransportRessourcesList_->findItem( MOS_App::GetApp().GetRessourceName( nType ).c_str(), 0 );
//    if ( pItem )
//        pItem->setText( 2, MT_FormatString( "%d", nQty ).c_str() );
//    else
//        AddTransportRessource( nType, nQty );
//}



//-----------------------------------------------------------------------------
// Name: MOS_AttrEditor::UpdateTransportRessource
// Created: JVT 04-03-31
//-----------------------------------------------------------------------------
//void MOS_AttrEditor::UpdateTransportRessource( const char* sName, uint nQty )
//{
//    assert( pTransportRessourcesList_ );
//    QListViewItem* pItem = pTransportRessourcesList_->findItem( sName, 0 );
//    if ( pItem )
//        pItem->setText( 2, MT_FormatString( "%d", nQty ).c_str() );
//    else
//        AddTransportRessource( sName, nQty );
//}


//-----------------------------------------------------------------------------
// Name: MOS_AttrEditor::UpdateTransportRessource
// Created: JVT 04-04-08
//-----------------------------------------------------------------------------
//void MOS_AttrEditor::UpdateTransportRessource( MIL_AgentID nType, uint nQtyAvailable, uint nQtyTransit )
//{
//    assert( pTransportRessourcesList_ );
//    QListViewItem* pItem = pTransportRessourcesList_->findItem( MOS_App::GetApp().GetRessourceName( nType ).c_str(), 0 );
//    if ( pItem )
//    {
//        pItem->setText( 1, MT_FormatString( "%d", nQtyAvailable ).c_str() );
//        pItem->setText( 2, MT_FormatString( "%d", nQtyTransit ).c_str() );
//    }
//    else
//        AddTransportRessource( nType, nQtyAvailable, nQtyTransit );
//}
//

//-----------------------------------------------------------------------------
// Name: MOS_AttrEditor::AddTransportEquipements
// Created: JVT 04-04-09
//-----------------------------------------------------------------------------
//void MOS_AttrEditor::AddTransportEquipements( uint nMOSID, uint nBreakDown, uint nNTI, uint nNbr )
//{
//    assert( pTransportEquipementList_ );
//    QListViewItem* pItem = new QListViewItem( pTransportEquipementList_ );
//    pItem->setText( 0, MOS_App::GetApp().GetEquipementName( nMOSID ).c_str() );
//    pItem->setText( 1, nBreakDown == (uint)-1 ? "N/A" : MOS_App::GetApp().GetBreakDownName( nBreakDown ).c_str() );
//    pItem->setText( 2, nNTI       == (uint)-1 ? "N/A" : MT_FormatString( "%d", nNTI ).c_str() );
//    pItem->setText( 3, MT_FormatString( "%d", nNbr ).c_str() );
//}


//-----------------------------------------------------------------------------
// Name: MOS_AttrEditor::AddHumains
// Created: AGN 03-06-04
//-----------------------------------------------------------------------------
void MOS_AttrEditor::AddHumains( const std::string& strLabel, uint nNbOfficiers, uint nNbSousOfficiers, uint nNbMdr )
{
    assert( pHumainsList_ != 0 );
    QListViewItem* pItem = new QListViewItem( pHumainsList_ );
    pItem->setText( 0, strLabel.c_str() );
    pItem->setText( 1, MT_FormatString( "%d", nNbOfficiers     ).c_str() );
    pItem->setText( 2, MT_FormatString( "%d", nNbSousOfficiers ).c_str() );
    pItem->setText( 3, MT_FormatString( "%d", nNbMdr           ).c_str() );
}


//-----------------------------------------------------------------------------
// Name: MOS_AttrEditor::AddTransportHumains
// Created: JVT 04-04-13
//-----------------------------------------------------------------------------
//void MOS_AttrEditor::AddTransportHumains( uint** nNbrs )
//{
//    assert( pTransportHumainsList_ );
//
//    static const char* labels[] = {
//                                    "Disponibles",
//                                    "Morts",
//                                    "Blessés U3",
//                                    "Blessés U2",
//                                    "Blessés U1",
//                                    "Blessés UE",
//                                    "Non catégorisés"
//                                };
//
//    for ( uint i = 0; i < 7; ++i )
//    {
//        QListViewItem* pItem = new QListViewItem( pTransportHumainsList_ );
//        pItem->setText( 0, labels[ i ] );
//        
//        for ( uint j = 0; j < 3; ++j )
//            pItem->setText( 1 + j, MT_FormatString( "%d", nNbrs[i][j] ).c_str() );
//    }
//}

//-----------------------------------------------------------------------------
// Name: MOS_AttrEditor::AddTransportCapacity
// Created: JVT 04-04-01
//-----------------------------------------------------------------------------
//void MOS_AttrEditor::AddTransportCapacity( MIL_AgentID nType, uint nQty )
//{
//    assert( pTransportCapacityList_ );
//    QListViewItem* pItem = new QListViewItem( pTransportCapacityList_ );
//    pItem->setText( 0, MOS_App::GetApp().GetTransportDotationName( nType ).c_str() );
//    pItem->setText( 1, MT_FormatString( "%d %%", nQty ).c_str() );
//}


//-----------------------------------------------------------------------------
// Name: MOS_AttrEditor::UpdateTransportCapacity
// Created: JVT 04-04-01
//-----------------------------------------------------------------------------
//void MOS_AttrEditor::UpdateTransportCapacity( MIL_AgentID nType, uint nQty )
//{
//    assert( pTransportCapacityList_ );
//    QListViewItem* pItem = pTransportCapacityList_->findItem( MOS_App::GetApp().GetTransportDotationName( nType ).c_str(), 0 );
//    if ( pItem )
//        pItem->setText( 1, MT_FormatString( "%d %%", nQty ).c_str() );
//    else
//        AddTransportCapacity( nType, nQty );
//}

// -----------------------------------------------------------------------------
// Name: MOS_AttrEditor::SlotMagicActionChangeHumanFactors
// Created: NLD 2004-11-30
// -----------------------------------------------------------------------------
void MOS_AttrEditor::SlotMagicActionChangeHumanFactors()
{
    assert( pAgent_ );
    assert( pChangeHumanFactorsDialog_ );
    pChangeHumanFactorsDialog_->SetAgent( *pAgent_ );
    pChangeHumanFactorsDialog_->show();
}

// -----------------------------------------------------------------------------
// Name: MOS_AttrEditor::SlotMagicActionChangeLogisticLinks
// Created: NLD 2004-11-30
// -----------------------------------------------------------------------------
void MOS_AttrEditor::SlotMagicActionChangeLogisticLinks()
{
    assert( pAgent_ );
    assert( pChangeLogisticLinksDialog_ );
    pChangeLogisticLinksDialog_->SetAgent( *pAgent_ );
    pChangeLogisticLinksDialog_->show();
}

// -----------------------------------------------------------------------------
// Name: MOS_AttrEditor::SlotMagicActionLogisticChangeQuotas
// Created: NLD 2005-02-03
// -----------------------------------------------------------------------------
void MOS_AttrEditor::SlotMagicActionLogisticChangeQuotas()
{
    assert( pAgent_ );
    assert( pButtonLogisticSupplyChangeQuotas_ );
    pLogisticSupplyChangeQuotasDialog_->SetAgent( *pAgent_ );
    pLogisticSupplyChangeQuotasDialog_->show();
}
        
// -----------------------------------------------------------------------------
// Name: MOS_AttrEditor::SlotMagicActionLogisticPushFlow
// Created: NLD 2005-02-03
// -----------------------------------------------------------------------------
void MOS_AttrEditor::SlotMagicActionLogisticPushFlow()
{
    assert( pAgent_ );
    assert( pButtonLogisticSupplyPushFlow_ );
    pLogisticSupplyPushFlowDialog_->SetAgent( *pAgent_ );
    pLogisticSupplyPushFlowDialog_->show();
}

// -----------------------------------------------------------------------------
// Name: MOS_AttrEditor::AddMaintenanceConsign
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void MOS_AttrEditor::AddMaintenanceConsign( MOS_LogMaintenanceConsign& consign )
{
    assert( pLogMaintenanceConsignListView_ );
    pLogMaintenanceConsignListView_->AddConsign( consign );
}

// -----------------------------------------------------------------------------
// Name: MOS_AttrEditor::RemoveMaintenanceConsign
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void MOS_AttrEditor::RemoveMaintenanceConsign( MOS_LogMaintenanceConsign& consign )
{
    assert( pLogMaintenanceConsignListView_ );
    pLogMaintenanceConsignListView_->RemoveConsign( consign );
}

// -----------------------------------------------------------------------------
// Name: MOS_AttrEditor::AddHandledMaintenanceConsign
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void MOS_AttrEditor::AddHandledMaintenanceConsign( MOS_LogMaintenanceConsign& consign )
{
    assert( pLogMaintenanceConsignHandledListView_ );
    pLogMaintenanceConsignHandledListView_->show();
    pLogMaintenanceConsignHandledListView_->AddConsign( consign );
}

// -----------------------------------------------------------------------------
// Name: MOS_AttrEditor::RemoveHandledMaintenanceConsign
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void MOS_AttrEditor::RemoveHandledMaintenanceConsign( MOS_LogMaintenanceConsign& consign )
{
    assert( pLogMaintenanceConsignHandledListView_ );
    pLogMaintenanceConsignHandledListView_->RemoveConsign( consign );
}

// -----------------------------------------------------------------------------
// Name: MOS_AttrEditor::SetLogMaintenanceChaineEnabled
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void MOS_AttrEditor::SetLogMaintenanceChaineEnabled( bool bChainEnabled )
{
    pLogMaintenanceState_->show();
    pLogMaintenanceStateChainEnabled_->setText( 1, bChainEnabled ? "Activée" : "Désactivée" );
}
    
// -----------------------------------------------------------------------------
// Name: MOS_AttrEditor::SetLogMaintenanceTempsBordee
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void MOS_AttrEditor::SetLogMaintenanceTempsBordee( uint nTempsBordee )
{
    pLogMaintenanceState_->show();
    pLogMaintenanceStateTempsBordee_->setText( 1, MT_FormatString( "%d heures", nTempsBordee ).c_str() );
}

// -----------------------------------------------------------------------------
// Name: MOS_AttrEditor::SetLogMaintenancePriorites
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void MOS_AttrEditor::SetLogMaintenancePriorites( const MOS_Agent::T_MaintenancePriorities&  maintenancePriorities )
{
    pLogMaintenanceState_->show();
    std::stringstream strPriorities;
    if( maintenancePriorities.empty() )
        strPriorities << "None";
    else
    {
        for( MOS_Agent::CIT_MaintenancePriorities it = maintenancePriorities.begin(); it != maintenancePriorities.end(); ++it )
            strPriorities << "'" << MOS_App::GetApp().GetEquipementName( *it ) << "' > ";
    }
    pLogMaintenanceStatePriorites_->setText( 1, strPriorities.str().c_str() );
}

// -----------------------------------------------------------------------------
// Name: MOS_AttrEditor::SetLogMaintenanceTacticalPriorites
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void MOS_AttrEditor::SetLogMaintenanceTacticalPriorites( const MOS_Agent::T_TacticalPriorities&  priorities )
{
    pLogMaintenanceState_->show();
    std::stringstream strPriorities;
    if( priorities.empty() )
        strPriorities << "None";
    else
    {
        for( MOS_Agent::CIT_TacticalPriorities it = priorities.begin(); it != priorities.end(); ++it )
            strPriorities << "'" << (**it).GetAgentID() << "' > ";
    }
    pLogMaintenanceStateTacticalPriorites_->setText( 1, strPriorities.str().c_str() );
}

// -----------------------------------------------------------------------------
// Name: MOS_AttrEditor::ResetDispoHaulers
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void MOS_AttrEditor::ResetLogMaintenanceDispoHaulers()
{
    pLogMaintenanceDispoHaulers_->clear();
    pLogMaintenanceDispoHaulers_->hide();
}

// -----------------------------------------------------------------------------
// Name: MOS_AttrEditor::ResetDispoRepairers
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void MOS_AttrEditor::ResetLogMaintenanceDispoRepairers()
{
    pLogMaintenanceDispoRepairers_->clear();
    pLogMaintenanceDispoRepairers_->hide();
}

// -----------------------------------------------------------------------------
// Name: MOS_AttrEditor::AddDispoHauler
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void MOS_AttrEditor::AddLogMaintenanceDispoHauler( uint nTypeID, uint nPourcentage )
{
    new QListViewItem( pLogMaintenanceDispoHaulers_, MOS_App::GetApp().GetEquipementName( nTypeID ).c_str(), MT_FormatString( "%d%%", nPourcentage ).c_str() );
    pLogMaintenanceDispoHaulers_->show();
}

// -----------------------------------------------------------------------------
// Name: MOS_AttrEditor::AddDispoRepairer
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void MOS_AttrEditor::AddLogMaintenanceDispoRepairer( uint nTypeID, uint nPourcentage )
{
    new QListViewItem( pLogMaintenanceDispoRepairers_, MOS_App::GetApp().GetEquipementName( nTypeID ).c_str(), MT_FormatString( "%d%%", nPourcentage ).c_str() );
    pLogMaintenanceDispoRepairers_->show();
}


// -----------------------------------------------------------------------------
// Name: MOS_AttrEditor::AddSupplyConsign
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void MOS_AttrEditor::AddSupplyConsign( MOS_LogSupplyConsign& consign )
{
    assert( pLogSupplyConsignListView_ );
    pLogSupplyConsignListView_->AddConsign( consign );
}

// -----------------------------------------------------------------------------
// Name: MOS_AttrEditor::RemoveSupplyConsign
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void MOS_AttrEditor::RemoveSupplyConsign( MOS_LogSupplyConsign& consign )
{
    assert( pLogSupplyConsignListView_ );
    pLogSupplyConsignListView_->RemoveConsign( consign );
}

// -----------------------------------------------------------------------------
// Name: MOS_AttrEditor::AddHandledSupplyConsign
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void MOS_AttrEditor::AddHandledSupplyConsign( MOS_LogSupplyConsign& consign )
{
    assert( pLogSupplyConsignHandledListView_ );
    pLogSupplyConsignHandledListView_->show();
    pLogSupplyConsignHandledListView_->AddConsign( consign );
}

// -----------------------------------------------------------------------------
// Name: MOS_AttrEditor::RemoveHandledSupplyConsign
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void MOS_AttrEditor::RemoveHandledSupplyConsign( MOS_LogSupplyConsign& consign )
{
    assert( pLogSupplyConsignHandledListView_ );
    pLogSupplyConsignHandledListView_->RemoveConsign( consign );
}

// -----------------------------------------------------------------------------
// Name: MOS_AttrEditor::SetLogSupplyChaineEnabled
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void MOS_AttrEditor::SetLogSupplyChaineEnabled( bool bChainEnabled )
{
    pLogSupplyState_->show();
    pLogSupplyStateChainEnabled_->setText( 1, bChainEnabled ? "Activée" : "Désactivée" );
}
    
// -----------------------------------------------------------------------------
// Name: MOS_AttrEditor::ResetDispoHaulers
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void MOS_AttrEditor::ResetLogSupplyDispoTransporters()
{
    pLogSupplyDispoTransporters_->clear();
    pLogSupplyDispoTransporters_->hide();
}

// -----------------------------------------------------------------------------
// Name: MOS_AttrEditor::ResetLogSupplyDispoCommanders
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void MOS_AttrEditor::ResetLogSupplyDispoCommanders()
{
    pLogSupplyDispoCommanders_->clear();
    pLogSupplyDispoCommanders_->hide();
}

// -----------------------------------------------------------------------------
// Name: MOS_AttrEditor::AddLogSupplyDispoTransporter
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void MOS_AttrEditor::AddLogSupplyDispoTransporter( uint nTypeID, uint nPourcentage )
{
    new QListViewItem( pLogSupplyDispoTransporters_, MOS_App::GetApp().GetEquipementName( nTypeID ).c_str(), MT_FormatString( "%d%%", nPourcentage ).c_str() );
    pLogSupplyDispoTransporters_->show();
}

// -----------------------------------------------------------------------------
// Name: MOS_AttrEditor::AddLogSupplyDispoCommander
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void MOS_AttrEditor::AddLogSupplyDispoCommander( uint nTypeID, uint nPourcentage )
{
    new QListViewItem( pLogSupplyDispoCommanders_, MOS_App::GetApp().GetEquipementName( nTypeID ).c_str(), MT_FormatString( "%d%%", nPourcentage ).c_str() );
    pLogSupplyDispoCommanders_->show();
}

// -----------------------------------------------------------------------------
// Name: MOS_AttrEditor::ResetLogSupplyStocks
// Created: NLD 2005-01-28
// -----------------------------------------------------------------------------
void MOS_AttrEditor::ResetLogSupplyStocks()
{
    if( pLogSupplyStocks_ )
    {
        pLogSupplyStocks_->hide();
        pLogSupplyStocks_->clear();
    }
}

// -----------------------------------------------------------------------------
// Name: MOS_AttrEditor::AddLogSupplyStock
// Created: NLD 2005-01-28
// -----------------------------------------------------------------------------
void MOS_AttrEditor::AddLogSupplyStock( MIL_AgentID nType, uint nQty )
{
    pLogSupplyStocks_->show();
    assert( pLogSupplyStocks_ );
    QListViewItem* pItem = new QListViewItem( pLogSupplyStocks_ );
    pItem->setText( 0, MOS_App::GetApp().GetRessourceName( nType ).c_str() );
    pItem->setText( 1, MT_FormatString( "%d", nQty ).c_str() );
}

// -----------------------------------------------------------------------------
// Name: MOS_AttrEditor::ResetLogSupplyQuotas
// Created: NLD 2005-01-28
// -----------------------------------------------------------------------------
void MOS_AttrEditor::ResetLogSupplyQuotas()
{
    if( pLogSupplyQuotas_ )
    {
        pLogSupplyQuotas_->hide();
        pLogSupplyQuotas_->clear();
    }
}

// -----------------------------------------------------------------------------
// Name: MOS_AttrEditor::AddLogSupplyQuota
// Created: NLD 2005-01-28
// -----------------------------------------------------------------------------
void MOS_AttrEditor::AddLogSupplyQuota( MIL_AgentID nType, uint nQty )
{
    pLogSupplyQuotas_->show();
    assert( pLogSupplyQuotas_ );
    QListViewItem* pItem = new QListViewItem( pLogSupplyQuotas_ );
    pItem->setText( 0, MOS_App::GetApp().GetRessourceName( nType ).c_str() );
    pItem->setText( 1, MT_FormatString( "%d", nQty ).c_str() );
}

// -----------------------------------------------------------------------------
// Name: MOS_AttrEditor::AddMedicalConsign
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void MOS_AttrEditor::AddMedicalConsign( MOS_LogMedicalConsign& consign )
{
    assert( pLogMedicalConsignListView_ );
    pLogMedicalConsignListView_->AddConsign( consign );
}

// -----------------------------------------------------------------------------
// Name: MOS_AttrEditor::RemoveMedicalConsign
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void MOS_AttrEditor::RemoveMedicalConsign( MOS_LogMedicalConsign& consign )
{
    assert( pLogMedicalConsignListView_ );
    pLogMedicalConsignListView_->RemoveConsign( consign );
}

// -----------------------------------------------------------------------------
// Name: MOS_AttrEditor::AddHandledMedicalConsign
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void MOS_AttrEditor::AddHandledMedicalConsign( MOS_LogMedicalConsign& consign )
{
    assert( pLogMedicalConsignHandledListView_ );
    pLogMedicalConsignHandledListView_->show();
    pLogMedicalConsignHandledListView_->AddConsign( consign );
}

// -----------------------------------------------------------------------------
// Name: MOS_AttrEditor::RemoveHandledMedicalConsign
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void MOS_AttrEditor::RemoveHandledMedicalConsign( MOS_LogMedicalConsign& consign )
{
    assert( pLogMedicalConsignHandledListView_ );
    pLogMedicalConsignHandledListView_->RemoveConsign( consign );
}

// -----------------------------------------------------------------------------
// Name: MOS_AttrEditor::SetLogMedicalChaineEnabled
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void MOS_AttrEditor::SetLogMedicalChaineEnabled( bool bChainEnabled )
{
    pLogMedicalState_->show();
    pLogMedicalStateChainEnabled_->setText( 1, bChainEnabled ? "Activée" : "Désactivée" );
}
    
// -----------------------------------------------------------------------------
// Name: MOS_AttrEditor::SetLogMedicalTempsBordee
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void MOS_AttrEditor::SetLogMedicalTempsBordee( uint nTempsBordee )
{
    pLogMedicalState_->show();
    pLogMedicalStateTempsBordee_->setText( 1, MT_FormatString( "%d heures", nTempsBordee ).c_str() );
}

// -----------------------------------------------------------------------------
// Name: MOS_AttrEditor::SetLogMedicalPriorites
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void MOS_AttrEditor::SetLogMedicalPriorites( const MOS_Agent::T_MedicalPriorities&  medicalPriorities )
{
    pLogMedicalState_->show();
    std::stringstream strPriorities;
    if( medicalPriorities.empty() )
        strPriorities << "None";
    else
    {
        for( MOS_Agent::CIT_MedicalPriorities it = medicalPriorities.begin(); it != medicalPriorities.end(); ++it )
        {
            switch( *it )
            {
                case EnumHumanWound::blesse_urgence_1      : strPriorities << "U1 > "; break;
                case EnumHumanWound::blesse_urgence_2      : strPriorities << "U2 > "; break;
                case EnumHumanWound::blesse_urgence_3      : strPriorities << "U3 > "; break;
                case EnumHumanWound::blesse_urgence_extreme: strPriorities << "UE > "; break;
                case EnumHumanWound::mort                  : strPriorities << "Morts > "; break;
                case EnumHumanWound::non_blesse            : strPriorities << "Non blessés > "; break;
            }
        }
    }
    pLogMedicalStatePriorites_->setText( 1, strPriorities.str().c_str() );
}

// -----------------------------------------------------------------------------
// Name: MOS_AttrEditor::SetLogMedicalTacticalPriorites
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void MOS_AttrEditor::SetLogMedicalTacticalPriorites( const MOS_Agent::T_TacticalPriorities&  priorities )
{
    pLogMedicalState_->show();
    std::stringstream strPriorities;
    if( priorities.empty() )
        strPriorities << "None";
    else
    {
        for( MOS_Agent::CIT_TacticalPriorities it = priorities.begin(); it != priorities.end(); ++it )
            strPriorities << "'" << (**it).GetAgentID() << "' > ";
    }
    pLogMedicalStateTacticalPriorites_->setText( 1, strPriorities.str().c_str() );
}

// -----------------------------------------------------------------------------
// Name: MOS_AttrEditor::ResetLogMedicalDispoReleveAmbulances
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
void MOS_AttrEditor::ResetLogMedicalDispoReleveAmbulances(  )
{
    pLogMedicalDispoReleveAmbulances_->clear();
    pLogMedicalDispoReleveAmbulances_->hide();
}

// -----------------------------------------------------------------------------
// Name: MOS_AttrEditor::ResetLogMedicalDispoRamassageAmbulances
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
void MOS_AttrEditor::ResetLogMedicalDispoRamassageAmbulances(  )
{
    pLogMedicalDispoRamassageAmbulances_->clear();
    pLogMedicalDispoRamassageAmbulances_->hide();
}

// -----------------------------------------------------------------------------
// Name: MOS_AttrEditor::ResetLogMedicalDispoDoctors
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
void MOS_AttrEditor::ResetLogMedicalDispoDoctors(  )
{
    pLogMedicalDispoDoctors_->clear();
    pLogMedicalDispoDoctors_->hide();
}


// -----------------------------------------------------------------------------
// Name: MOS_AttrEditor::AddLogMedicalDispoReleveAmbulances
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
void MOS_AttrEditor::AddLogMedicalDispoReleveAmbulances( uint nTypeID, uint nPourcentage )
{
    new QListViewItem( pLogMedicalDispoReleveAmbulances_, MOS_App::GetApp().GetEquipementName( nTypeID ).c_str(), MT_FormatString( "%d%%", nPourcentage ).c_str() );
    pLogMedicalDispoReleveAmbulances_->show();
}
// -----------------------------------------------------------------------------
// Name: MOS_AttrEditor::AddLogMedicalDispoRamassageAmbulances
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
void MOS_AttrEditor::AddLogMedicalDispoRamassageAmbulances( uint nTypeID, uint nPourcentage )
{
    new QListViewItem( pLogMedicalDispoRamassageAmbulances_, MOS_App::GetApp().GetEquipementName( nTypeID ).c_str(), MT_FormatString( "%d%%", nPourcentage ).c_str() );
    pLogMedicalDispoRamassageAmbulances_->show();
}
// -----------------------------------------------------------------------------
// Name: MOS_AttrEditor::AddLogMedicalDispoDoctors
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
void MOS_AttrEditor::AddLogMedicalDispoDoctors( uint nTypeID, uint nPourcentage )
{
    new QListViewItem( pLogMedicalDispoDoctors_, MOS_App::GetApp().GetEquipementName( nTypeID ).c_str(), MT_FormatString( "%d%%", nPourcentage ).c_str() );
    pLogMedicalDispoDoctors_->show();
}

// -----------------------------------------------------------------------------
// Name: MOS_AttrEditor::SetLends
// Created: NLD 2005-01-18
// -----------------------------------------------------------------------------
void MOS_AttrEditor::SetLends( const MOS_Agent::T_LendVector& lends )
{
    pLends_->clear();
    if( lends.empty() )
        return;
    for( MOS_Agent::CIT_LendVector it = lends.begin(); it != lends.end(); ++it )
    {
        const MOS_Agent::T_Lend& lend = *it;
        new QListViewItem( pLends_, MT_FormatString( "%d", lend.pLent_->GetAgentID() ).c_str(), MOS_App::GetApp().GetEquipementName( lend.nComposanteTypeID_ ).c_str(), MT_FormatString( "%d", lend.nNbr_ ).c_str() );
    }
    pLends_->show();
}

    
// -----------------------------------------------------------------------------
// Name: MOS_AttrEditor::Reset
// Created: NLD 2003-12-01
// -----------------------------------------------------------------------------
void MOS_AttrEditor::Reset()
{
    ResetRessources();
    ResetEquipments();
    ResetHumains();
    
    ResetLogMaintenanceDispoHaulers();
    ResetLogMaintenanceDispoRepairers();

    ResetLogMedicalDispoReleveAmbulances(  );
    ResetLogMedicalDispoRamassageAmbulances(  );  
    ResetLogMedicalDispoDoctors(  );

    ResetLogSupplyDispoCommanders();
    ResetLogSupplyDispoTransporters();
    ResetLogSupplyStocks();
    ResetLogSupplyQuotas();

    pLogMaintenanceState_->hide();
    pLogMaintenanceConsignHandledListView_->hide();

    pLogMedicalState_->hide();
    pLogMedicalConsignHandledListView_->hide();

    pLogSupplyState_->hide();
    pLogSupplyConsignHandledListView_->hide();

    pLends_->clear();
    pLends_->hide();

    ResetPosture();
}


