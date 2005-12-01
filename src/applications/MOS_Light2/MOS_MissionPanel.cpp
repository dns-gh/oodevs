// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-03-19 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_MissionPanel.cpp $
// $Author: Age $
// $Modtime: 6/04/05 17:33 $
// $Revision: 16 $
// $Workfile: MOS_MissionPanel.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "MOS_Light2_pch.h"
#include "MOS_MissionPanel.h"
#include "moc_MOS_MissionPanel.cpp"

#include "MOS_Agent.h"
#include "MOS_Population.h"
#include "MOS_PopulationConcentration.h"
#include "MOS_PopulationFlow.h"
#include "MOS_MainWindow.h"
#include "MOS_AgentModel.h"
#include "MOS_ASN_Messages.h"
#include "MOS_ActionContext.h"
#include "MOS_UnitMissionInterface.h"
#include "MOS_AutomateMissionInterface.h"
#include "MOS_PopulationMissionInterface.h"
#include "MOS_FragmentaryOrderInterface.h"
#include "MOS_ShapeEditorMapEventFilter.h"
#include "MOS_RC.h"
#include "MOS_Tools.h"
#include "MOS_Options.h"
#include "MOS_World.h"
#include "MOS_AgentServer.h"
#include "MOS_AgentManager.h"
#include "MOS_ChangeDiplomacyDialog.h"
#include "MOS_ChangeLogisticLinksDialog.h"
#include "MOS_LogisticSupplyPushFlowDialog.h"
#include "MOS_LogisticSupplyChangeQuotasDialog.h"
#include "MOS_LogisticSupplyRecompletionDialog.h"
#include "MOS_ChangeHumanFactorsDialog.h"

// -----------------------------------------------------------------------------
// Name: MOS_MissionPanel constructor
/** @param  pParent 
*/
// Created: APE 2004-03-19
// -----------------------------------------------------------------------------
MOS_MissionPanel::MOS_MissionPanel( QWidget* pParent )
    : QDockWindow                        ( pParent )
    , pMissionInterface_                 ( 0 )
    , pAgentMagicMovePositionEditor_     ( new MOS_ShapeEditorMapEventFilter( this ) )
    , pPopulationMagicMovePositionEditor_( new MOS_ShapeEditorMapEventFilter( this ) )
    , pPopupPopulation_                  ( 0 )
    , pPopupPopulationConcentration_     ( 0 )
    , pPopupPopulationFlow_              ( 0 )
{
    this->setResizeEnabled( true );
    this->setCaption( tr( "Mission" ) );
    this->setCloseMode( QDockWindow::Always );

    connect( pAgentMagicMovePositionEditor_     , SIGNAL( Done() ), this, SLOT( MagicMoveDone() ) );
    connect( pPopulationMagicMovePositionEditor_, SIGNAL( Done() ), this, SLOT( PopulationMagicMoveDone() ) );

    connect( &MOS_MainWindow::GetMainWindow(), SIGNAL( NewPopupMenu( QPopupMenu&, const MOS_ActionContext& ) ), this,   SLOT( FillRemotePopupMenu( QPopupMenu&, const MOS_ActionContext& ) ) );
    connect( &MOS_MainWindow::GetMainWindow(), SIGNAL( TeamChanged() ),                                         this,   SLOT( hide() ) );
}


// -----------------------------------------------------------------------------
// Name: MOS_MissionPanel destructor
// Created: APE 2004-03-19
// -----------------------------------------------------------------------------
MOS_MissionPanel::~MOS_MissionPanel()
{
}


// -----------------------------------------------------------------------------
// Name: MOS_MissionPanel::FillRemotePopupMenu
// Created: APE 2004-03-19
// -----------------------------------------------------------------------------
void MOS_MissionPanel::FillRemotePopupMenu( QPopupMenu& popupMenu, const MOS_ActionContext& context )
{
    if( pMissionInterface_ != 0 )
        pMissionInterface_->FillRemotePopupMenu( popupMenu, context );

    if( context.selectedElement_.pAgent_ != 0 )
        FillStandardPopupMenu( popupMenu, *context.selectedElement_.pAgent_ );
    if( context.selectedElement_.pTeam_ != 0 && MOS_MainWindow::GetMainWindow().GetOptions().bControllerMode_ )
        FillDiplomacyPopupMenu( popupMenu, *context.selectedElement_.pTeam_ );
    if( context.selectedElement_.pPopulation_ )
        FillStandardPopupMenu( popupMenu, *context.selectedElement_.pPopulation_ );
    else if( context.selectedElement_.pPopulationConcentration_ )
        FillStandardPopupMenu( popupMenu, *context.selectedElement_.pPopulationConcentration_ );
    else if( context.selectedElement_.pPopulationFlow_ )
        FillStandardPopupMenu( popupMenu, *context.selectedElement_.pPopulationFlow_ );
    else if( context.selectedElement_.pRC_ != 0 )
        FillFragmentaryOrderPopup( popupMenu, *context.selectedElement_.pRC_ );
}

// -----------------------------------------------------------------------------
// Name: MOS_MissionPanel::FillDiplomacyPopupMenu
// Created: AGE 2005-04-06
// -----------------------------------------------------------------------------
void MOS_MissionPanel::FillDiplomacyPopupMenu( QPopupMenu& popupMenu, MOS_Team& team )
{
    pPopupTeam_ = &team;
    popupMenu.insertItem( tr( "Diplomatie" ), this, SLOT( MagicChangeDiplomatie() ) );
}

// -----------------------------------------------------------------------------
// Name: MOS_MissionPanel::FillDiplomacyPopupMenu
// Created: AGE 2005-04-06
// -----------------------------------------------------------------------------
void MOS_MissionPanel::FillStandardPopupMenu( QPopupMenu& popupMenu, MOS_Population& population )
{
    pPopupPopulation_ = &population;

   if( popupMenu.count() > 0 )
        popupMenu.insertSeparator();

    // Create and fill the unit menu.
    QPopupMenu* pPopulationMenu = new QPopupMenu( &popupMenu );
    const MOS_AgentModel::T_MissionVector& missions = population.GetModel().GetAvailableMissions();
    for( MOS_AgentModel::CIT_MissionVector it = missions.begin(); it != missions.end(); ++it )
    {
        int nId = pPopulationMenu->insertItem( ENT_Tr::ConvertFromPopulationMission( E_PopulationMission( *it ), ENT_Tr::eToApp ).c_str(), this, SLOT( ActivatePopulationMission( int ) ) );
        pPopulationMenu->setItemParameter( nId, (int)*it );
    }

    // Add the unit mission menu.
    popupMenu.insertItem( tr( "Missions Population" ), pPopulationMenu );

    // Create and fill the fragmentary orders menu.
    QPopupMenu* pFragOrdersMenu = new QPopupMenu( &popupMenu );
    
    // Commun orders.
    //$$$$ Hard coded value!
    for( uint i = eOrdreConduite_Population_ChangerAttitude; i < eNbrFragOrder; ++i )
    {
        int nId = pFragOrdersMenu->insertItem( ENT_Tr::ConvertFromFragOrder( E_FragOrder( i ), ENT_Tr::eToApp ).c_str(), this, SLOT( ActivateFragmentaryOrderPopulation( int ) ) );
        pFragOrdersMenu->setItemParameter( nId, i );
    }

    int nFragId = popupMenu.insertItem( tr( "Ordres de conduite" ), pFragOrdersMenu );

    // Add the magic orders if playing as controller.
    if( MOS_MainWindow::GetMainWindow().GetOptions().bControllerMode_ )
    {
        QPopupMenu* pMagicOrdersMenu = new QPopupMenu( &popupMenu );
        pMagicOrdersMenu->insertItem( tr( "Téléportation" ), this, SLOT( PopulationMagicMove() ) );

        QPopupMenu* pAttitudeMenu = new QPopupMenu();
        pAttitudeMenu->setCheckable( true );
        QPopupMenu* pAttitudeGlobalMenu = new QPopupMenu();
        for( int i = 0; i < eNbrPopulationAttitude; ++i )
        {
            int nIdx = pAttitudeMenu->insertItem( ENT_Tr::ConvertFromPopulationAttitude( ( E_PopulationAttitude )i ).c_str(), this, SLOT( PopulationMagicChangeAttitude( int ) ) );
            pAttitudeMenu->setItemParameter( nIdx, i );
            if(    ( pPopupPopulationConcentration_ && pPopupPopulationConcentration_->GetAttitude() == ( E_PopulationAttitude )i )
                || ( pPopupPopulationFlow_ && pPopupPopulationFlow_->GetAttitude() == ( E_PopulationAttitude )i ) )
                pAttitudeMenu->setItemChecked( nIdx, true );

            nIdx = pAttitudeGlobalMenu->insertItem( ENT_Tr::ConvertFromPopulationAttitude( ( E_PopulationAttitude )i ).c_str(), this, SLOT( PopulationMagicChangeAttitudeGlobal( int ) ) );
            pAttitudeGlobalMenu->setItemParameter( nIdx, i );
        }

        pMagicOrdersMenu->insertItem( tr( "Changement d'attitude" ), pAttitudeMenu );
        pMagicOrdersMenu->insertItem( tr( "Changement d'attitude global" ), pAttitudeGlobalMenu );
        pMagicOrdersMenu->insertItem( tr( "Destruction totale" )          , this, SLOT( PopulationMagicDestroyAll() ) );
        popupMenu.insertItem( tr( "Ordres magiques" ), pMagicOrdersMenu );
    }
}

// -----------------------------------------------------------------------------
// Name: MOS_MissionPanel::FillStandardPopupMenu
// Created: SBO 2005-10-27
// -----------------------------------------------------------------------------
void MOS_MissionPanel::FillStandardPopupMenu( QPopupMenu& popupMenu, MOS_PopulationConcentration& concentration )
{
    pPopupPopulationConcentration_ = &concentration;
    pPopupPopulation_ = 0;
    pPopupPopulationFlow_ = 0;
    FillStandardPopupMenu( popupMenu, const_cast< MOS_Population& >( concentration.GetPopulation() ) );
}
    
// -----------------------------------------------------------------------------
// Name: MOS_MissionPanel::FillStandardPopupMenu
// Created: SBO 2005-10-27
// -----------------------------------------------------------------------------
void MOS_MissionPanel::FillStandardPopupMenu( QPopupMenu& popupMenu, MOS_PopulationFlow& flow )
{
    pPopupPopulationFlow_ = &flow;
    pPopupPopulation_ = 0;
    pPopupPopulationConcentration_ = 0;
    FillStandardPopupMenu( popupMenu, const_cast< MOS_Population& >( flow.GetPopulation() ) );
}

// -----------------------------------------------------------------------------
// Name: MOS_MissionPanel::FillStandardPopupMenu
// Created: APE 2004-05-12
// -----------------------------------------------------------------------------
void MOS_MissionPanel::FillStandardPopupMenu( QPopupMenu& popupMenu, MOS_Agent& agent )
{
    pPopupAgent_ = &agent;

    if( popupMenu.count() > 0 )
        popupMenu.insertSeparator();

    // Create and fill the unit menu.
    QPopupMenu* pUnitMenu = new QPopupMenu( &popupMenu );
    const MOS_AgentModel::T_MissionVector& missions = agent.GetModelPion()->GetAvailableMissions();
    for( MOS_AgentModel::CIT_MissionVector it = missions.begin(); it != missions.end(); ++it )
    {
        int nId = pUnitMenu->insertItem( ENT_Tr::ConvertFromUnitMission( E_UnitMission( *it ), ENT_Tr::eToApp ).c_str(), this, SLOT( ActivateUnitMission( int ) ) );
        pUnitMenu->setItemParameter( nId, (int)*it );
    }

    // Add the unit mission menu.
    int nUnitMenuId = popupMenu.insertItem( tr( "Missions Pion" ), pUnitMenu  );
    if( agent.IsEmbraye() || ( agent.GetParent() != 0 && agent.GetParent()->IsEmbraye() ) )
        popupMenu.setItemEnabled( nUnitMenuId, false );

    // Create and fill the automata menu if necessary.
    if( agent.IsAutomate() )
    {
        QPopupMenu* pAutomataMenu = new QPopupMenu( &popupMenu );

        const MOS_AgentModel::T_MissionVector& missions = agent.GetModelAutomate()->GetAvailableMissions();
        for( MOS_AgentModel::CIT_MissionVector it = missions.begin(); it != missions.end(); ++it )
        {
            int nId = pAutomataMenu->insertItem( ENT_Tr::ConvertFromAutomataMission( E_AutomataMission( *it ), ENT_Tr::eToApp ).c_str(), this, SLOT( ActivateAutomataMission( int ) ) );
            pAutomataMenu->setItemParameter( nId, (int)*it );
        }

        // Add the automata mission menu.
        int nAutomataMenuId = popupMenu.insertItem( tr( "Missions Automate" ), pAutomataMenu );
        popupMenu.setItemEnabled( nAutomataMenuId, agent.IsEmbraye() );
    }

    // Create and fill the fragmentary orders menu.
    QPopupMenu* pFragOrdersMenu = new QPopupMenu( &popupMenu );
    
    // Commun orders.
    //$$$$ Hard coded value!
    for( uint i = 0; i <= eOrdreConduite_ChangerReglesEngagementPopulation; ++i )
    {
        int nId = pFragOrdersMenu->insertItem( ENT_Tr::ConvertFromFragOrder( E_FragOrder( i ), ENT_Tr::eToApp ).c_str(), this, SLOT( ActivateFragmentaryOrder( int ) ) );
        pFragOrdersMenu->setItemParameter( nId, i );
    }

    // Specific unit frag orders.
    const MOS_AgentModel::T_FragOrderSet& fragOrders = agent.GetModelPion()->GetAvailableFragOrders();
    for( MOS_AgentModel::CIT_FragOrderSet itFrag = fragOrders.begin(); itFrag != fragOrders.end(); ++itFrag )
    {
        int nId = pFragOrdersMenu->insertItem( ENT_Tr::ConvertFromFragOrder( E_FragOrder( *itFrag ), ENT_Tr::eToApp ).c_str(), this, SLOT( ActivateFragmentaryOrder( int ) ) );
        pFragOrdersMenu->setItemParameter( nId, (int)(*itFrag) );
    }

    // Specific automata frag orders.
    if( agent.IsAutomate() )
    {
        const MOS_AgentModel::T_FragOrderSet& fragOrders2 = agent.GetModelAutomate()->GetAvailableFragOrders();
        if( fragOrders2.size() > 0 )
            pFragOrdersMenu->insertSeparator();
        for( MOS_AgentModel::CIT_FragOrderSet itFrag2 = fragOrders2.begin(); itFrag2 != fragOrders2.end(); ++itFrag2 )
        {
            int nId = pFragOrdersMenu->insertItem( ENT_Tr::ConvertFromFragOrder( E_FragOrder( *itFrag2 ), ENT_Tr::eToApp ).c_str(), this, SLOT( ActivateFragmentaryOrder( int ) ) );
            pFragOrdersMenu->setItemParameter( nId, (int)(*itFrag2) );
        }
    }

    int nFragId = popupMenu.insertItem( tr( "Ordres de conduite" ), pFragOrdersMenu );
    if( agent.GetParent() != 0 && agent.GetParent()->IsEmbraye() )
        popupMenu.setItemEnabled( nFragId, false );

    // Add the magic orders if playing as controller.
    if( MOS_MainWindow::GetMainWindow().GetOptions().bControllerMode_ )
    {
        QPopupMenu* pMagicOrdersMenu = new QPopupMenu( &popupMenu );
        int nId;

        nId = pMagicOrdersMenu->insertItem( tr( "Téléportation" ), this, SLOT( MagicMove() ) );
        if( agent.GetParent() != 0 && agent.GetParent()->IsEmbraye() )
            pMagicOrdersMenu->setItemEnabled( nId, false );

        nId = pMagicOrdersMenu->insertItem( tr( "Recompletement personnel" ), this, SLOT( MagicRestore( int ) ) );
        pMagicOrdersMenu->setItemParameter( nId, T_MsgUnitMagicAction_action_recompletement_personnel );
     
        nId = pMagicOrdersMenu->insertItem( tr( "Recompletement équipement" ), this, SLOT( MagicRestore( int ) ) );
        pMagicOrdersMenu->setItemParameter( nId, T_MsgUnitMagicAction_action_recompletement_equipement );
        
        nId = pMagicOrdersMenu->insertItem( tr( "Recompletement ressources" ), this, SLOT( MagicRestore( int ) ) );
        pMagicOrdersMenu->setItemParameter( nId, T_MsgUnitMagicAction_action_recompletement_ressources );
        
        nId = pMagicOrdersMenu->insertItem( tr( "Recompletement total" ), this, SLOT( MagicRestore( int ) ) );
        pMagicOrdersMenu->setItemParameter( nId, T_MsgUnitMagicAction_action_recompletement_total );
        
        pMagicOrdersMenu->insertItem( tr( "Recompletement partiel" ), this, SLOT( MagicRecompletion() ) );

        pMagicOrdersMenu->insertItem( tr( "Détruire composante" ), this, SLOT( MagicDestroyComponent() ) );
        pMagicOrdersMenu->insertItem( tr( "Destruction totale" ), this, SLOT( MagicDestroyAll() ) );

        pMagicOrdersMenu->insertItem( tr( "Facteurs humains" ), this, SLOT( ChangeHumanFactors() ) );

        if( agent.IsAutomate() )
            pMagicOrdersMenu->insertItem( tr( "Se rendre" ), this, SLOT( MagicSurrender() ) );



        popupMenu.insertItem( tr( "Ordres magiques" ), pMagicOrdersMenu );
    }

    // Control toggle.
    if( agent.IsAutomate() )
    {
        popupMenu.insertSeparator();
        popupMenu.insertItem( agent.IsEmbraye() ? tr( "Débrayer automate" ) : tr( "Embrayer automate" ), this, SLOT( ToggleAutomate() ) );
        popupMenu.insertItem( agent.IsAggregated() ? tr( "Désaggréger l'automate" ) : tr( "Aggréger l'automate" ), this, SLOT( ToggleAggregation() ) );
        popupMenu.insertSeparator();
        if( MOS_MainWindow::GetMainWindow().GetOptions().bControllerMode_ )
            popupMenu.insertItem( tr( "Liens logistiques" ), this, SLOT( MagicChangeLogisticLinks() ) );
    }

    if( agent.IsLogisticRavitaillement() && ( agent.IsLogisticBLT() || agent.IsLogisticBLD() ) )
    {
        popupMenu.insertItem( tr( "Pousser flux" )  , this, SLOT( MagicPushFlux    () ) );
        popupMenu.insertItem( tr( "Changer quotas" ), this, SLOT( MagicChangeQuotas() ) );
        popupMenu.insertSeparator();
    }
}


// -----------------------------------------------------------------------------
// Name: MOS_MissionPanel::FillFragmentaryOrderPopup
// Created: APE 2004-05-12
// -----------------------------------------------------------------------------
void MOS_MissionPanel::FillFragmentaryOrderPopup( QPopupMenu& popupMenu, MOS_RC& rc )
{
    pPopupAgent_ = dynamic_cast< MOS_Agent* >( &rc.GetAgent() );
    
    const MOS_RC::T_FragOrderVector& orderList = rc.GetFragOrders();
    for( MOS_RC::CIT_FragOrderVector it = orderList.begin(); it != orderList.end(); ++it )
    {
        int nId = popupMenu.insertItem( ENT_Tr::ConvertFromFragOrder( E_FragOrder( *it ), ENT_Tr::eToApp ).c_str(), this, SLOT( ActivateFragmentaryOrder( int ) ) );
        popupMenu.setItemParameter( nId, (int)(*it) );
    }
}


// -----------------------------------------------------------------------------
// Name: MOS_MissionPanel::hideEvent
// Created: APE 2004-04-27
// -----------------------------------------------------------------------------
void MOS_MissionPanel::hideEvent( QHideEvent* pEvent )
{
    if( ! pEvent->spontaneous() )
    {
        delete pMissionInterface_;
        pMissionInterface_ = 0;
    }
}


// -----------------------------------------------------------------------------
// Name: MOS_MissionPanel::ToggleAutomate
// Created: APE 2004-03-22
// -----------------------------------------------------------------------------
void MOS_MissionPanel::ToggleAutomate()
{
    MOS_ASN_MsgSetAutomateMode asnMsg;
    asnMsg.GetAsnMsg().unit_id = pPopupAgent_->GetID();
    asnMsg.GetAsnMsg().mode    = pPopupAgent_->IsEmbraye() ? EnumAutomateState::debraye : EnumAutomateState::embraye;
    asnMsg.Send( 0 );

    std::stringstream strMsg;
    strMsg << "Demande passage mode " << (pPopupAgent_->IsEmbraye() ? "débrayé" : "embrayé") << " pour agent " << pPopupAgent_->GetID();
    MT_LOG_INFO( strMsg.str().c_str(), eSent, 0 );
}


// -----------------------------------------------------------------------------
// Name: MOS_MissionPanel::ToggleAggregation
// Created: HME 2005-11-30
// -----------------------------------------------------------------------------
void MOS_MissionPanel::ToggleAggregation()
{
    if( pPopupAgent_->IsAggregated() )
        pPopupAgent_->SetAggregation( false );
    else
        pPopupAgent_->SetAggregation( true );
}

// -----------------------------------------------------------------------------
// Name: MOS_MissionPanel::ActivateUnitMission
// Created: APE 2004-10-25
// -----------------------------------------------------------------------------
void MOS_MissionPanel::ActivateUnitMission( int nMissionId )
{
    this->hide();
    delete pMissionInterface_;
    pMissionInterface_ = 0;

    pMissionInterface_ = new MOS_UnitMissionInterface( *pPopupAgent_, (uint)nMissionId, *this );

    this->setWidget( pMissionInterface_ );

    // For some magic reason, the following line resizes the widget
    // to a nice size (but not the minimal one).
    this->resize( 10, 10 );
    this->show();
}


// -----------------------------------------------------------------------------
// Name: MOS_MissionPanel::ActivateAutomataMission
// Created: APE 2004-10-25
// -----------------------------------------------------------------------------
void MOS_MissionPanel::ActivateAutomataMission( int nMissionId )
{
    this->hide();
    delete pMissionInterface_;
    pMissionInterface_ = 0;

    pMissionInterface_ = new MOS_AutomateMissionInterface( *pPopupAgent_, (uint)nMissionId, *this );

    this->setWidget( pMissionInterface_ );

    // For some magic reason, the following line resizes the widget
    // to a nice size (but not the minimal one).
    this->resize( 10, 10 );
    this->show();
}

// -----------------------------------------------------------------------------
// Name: MOS_MissionPanel::ActivatePopulationMission
// Created: HME 2005-10-06
// -----------------------------------------------------------------------------
void MOS_MissionPanel::ActivatePopulationMission( int nMissionId )
{
    this->hide();
    delete pMissionInterface_;
    pMissionInterface_ = 0;

    pMissionInterface_ = new MOS_PopulationMissionInterface( *pPopupPopulation_, (uint)nMissionId, *this );

    this->setWidget( pMissionInterface_ );

    // For some magic reason, the following line resizes the widget
    // to a nice size (but not the minimal one).
    this->resize( 10, 10 );
    this->show();
}

// -----------------------------------------------------------------------------
// Name: MOS_MissionPanel::ActivateFragmentaryOrder
// Created: APE 2004-05-12
// -----------------------------------------------------------------------------
void MOS_MissionPanel::ActivateFragmentaryOrder( int nOrderId )
{
    this->hide();
    delete pMissionInterface_;
    pMissionInterface_ = 0;

    switch( (E_FragOrder)nOrderId )
    {
        case eOrdreConduite_ChangerReglesEngagement:
        case eOrdreConduite_ChangerReglesEngagementPopulation:
        case eOrdreConduite_ChangerPositionDebarquement:
        case eOrdreConduite_AcquerirObjectif:
        case eOrdreConduite_Pion_ChangerDePosition:
        case eOrdreConduite_Pion_AppliquerFeux:
        case eOrdreConduite_Pion_RenforcerEnVSRAM:
        case eOrdreConduite_Pion_TransfererVSRAM:
        case eOrdreConduite_Pion_ReprendreAuxOrdresVSRAM:
        case eOrdreConduite_ModifierTempsBordeeMaintenance:
        case eOrdreConduite_ModifierTempsBordeeSante:
        case eOrdreConduite_ModifierPrioritesReparations:
        case eOrdreConduite_ModifierPrioritesBlesses:
        case eOrdreConduite_ModifierPrioritesTactiquesBlesses:
        case eOrdreConduite_ModifierPrioritesTactiquesReparations:
        case eOrdreConduite_Automate_RealiserVariantement:
        case eOrdreConduite_Automate_DesequiperSiteFranchissement:
        case eOrdreConduite_Automate_ReagirFaceAEni:
        case eOrdreConduite_Automate_AffecterPionAObstacle:
        case eOrdreConduite_Automate_ReconnaitreZoneImplantation:
        case eOrdreConduite_Automate_TC2_GererMaterielAvantDeplacement:
        case eOrdreConduite_Automate_ActiverObstacle:
        case eOrdreConduite_Automate_TransfererRenforts:
        {
            pMissionInterface_ = new MOS_FragmentaryOrderInterface( *pPopupAgent_, (uint)nOrderId, *this );

            this->setWidget( pMissionInterface_ );
            this->show();
            return;
        }
        default:
        {
            MOS_ASN_MsgOrderConduite asnMsg;
            asnMsg.GetAsnMsg().unit_id  = pPopupAgent_->GetID();
            asnMsg.GetAsnMsg().order_id = 43;
            asnMsg.GetAsnMsg().order_conduite.t =  (uint)nOrderId +1;  // +1 because of ASN/enum stupidity
            asnMsg.Send( 65 );

            QString strMsg( tr( "Ordre de conduite %1 pour agent %2" ) );
            strMsg = strMsg.arg( ENT_Tr::ConvertFromFragOrder( E_FragOrder( (uint)nOrderId ), ENT_Tr::eToApp ).c_str(), pPopupAgent_->GetID() );

            MT_LOG_INFO( strMsg.ascii(), eSent, 0 );
            return;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: MOS_MissionPanel::ActivateFragmentaryOrderPopulation
// Created: SBO 2005-11-23
// -----------------------------------------------------------------------------
void MOS_MissionPanel::ActivateFragmentaryOrderPopulation( int nOrderId )
{
    hide();
    delete pMissionInterface_;
    pMissionInterface_ = 0;

    switch( (E_FragOrder)nOrderId )
    {
        case eOrdreConduite_Population_ChangerAttitude:
        {
            pMissionInterface_ = new MOS_FragmentaryOrderInterface( *pPopupPopulation_, (uint)nOrderId, *this );

            this->setWidget( pMissionInterface_ );
            this->show();
            return;
        }
        default:
        {
            MOS_ASN_MsgOrderConduite asnMsg;
            asnMsg.GetAsnMsg().unit_id  = pPopupPopulation_->GetID();
            asnMsg.GetAsnMsg().order_id = 43;
            asnMsg.GetAsnMsg().order_conduite.t =  (uint)nOrderId +1;  // +1 because of ASN/enum stupidity
            asnMsg.Send( 65 );

            QString strMsg( tr( "Ordre de conduite %1 pour population %2" ) );
            strMsg = strMsg.arg( ENT_Tr::ConvertFromFragOrder( E_FragOrder( (uint)nOrderId ), ENT_Tr::eToApp ).c_str(), pPopupPopulation_->GetID() );

            MT_LOG_INFO( strMsg.ascii(), eSent, 0 );
            return;
        }
    }
}

// =============================================================================
// MAGIC ACTIONS: Agents
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MOS_MissionPanel::MagicMove
// Created: APE 2004-05-26
// -----------------------------------------------------------------------------
void MOS_MissionPanel::MagicMove()
{
    pAgentMagicMovePositionEditor_->PrepareNewLine( MOS_ShapeEditorMapEventFilter::ePoint );
    pAgentMagicMovePositionEditor_->Push();
}


// -----------------------------------------------------------------------------
// Name: MOS_MissionPanel::MagicMoveDone
// Created: APE 2004-05-26
// -----------------------------------------------------------------------------
void MOS_MissionPanel::MagicMoveDone()
{
    T_PointVector& pointList = pAgentMagicMovePositionEditor_->GetPointList();
    if( pointList.empty() )
        return;

    assert( pointList.size() == 1 );
    std::string strPos;
    MOS_App::GetApp().GetWorld().SimToMosMgrsCoord( pointList.front(), strPos );

    ASN1T_CoordUTM  coordUTM;
    coordUTM = strPos.c_str();

    MOS_ASN_MsgUnitMagicAction asnMsg;
    asnMsg.GetAsnMsg().oid              = pPopupAgent_->GetID();
    asnMsg.GetAsnMsg().action.t         = T_MsgUnitMagicAction_action_move_to;
    asnMsg.GetAsnMsg().action.u.move_to = &coordUTM;
    asnMsg.Send( 56 );

    std::stringstream strMsg;
    strMsg << "Demande déplacement magique pour agent #" << pPopupAgent_->GetID();
    MT_LOG_INFO( strMsg.str().c_str(), eSent, 0 );

    //$$$ Temporary solution. Clearing the paths should be done when we receive confirmation
    // that the magic move has gone through to the sim, but since we lack that confirmation
    // message, we have to do this here.
    pPopupAgent_->ClearPath();
    
    if( pPopupAgent_->IsAutomate() && pPopupAgent_->IsEmbraye() )
    {
        const MOS_AgentManager::T_AgentMap& agents = MOS_App::GetApp().GetAgentManager().GetAgentList();
        for( MOS_AgentManager::CIT_AgentMap it = agents.begin(); it != agents.end(); ++it )
            if( (it->second)->GetParent() == pPopupAgent_ )
            (it->second)->ClearPath();
    }
}


// -----------------------------------------------------------------------------
// Name: MOS_MissionPanel::MagicRestore
// Created: APE 2004-05-26
// -----------------------------------------------------------------------------
void MOS_MissionPanel::MagicRestore( int nId )
{
    assert( pPopupAgent_ );
    MOS_ASN_MsgUnitMagicAction asnMsg;
    asnMsg.GetAsnMsg().oid              = pPopupAgent_->GetID();
    asnMsg.GetAsnMsg().action.t         = nId;
    asnMsg.Send( 56 );

    std::stringstream strMsg;
    strMsg << "Demande recompletement magique pour agent #" << pPopupAgent_->GetID();
    MT_LOG_INFO( strMsg.str().c_str(), eSent, 0 );
}

// -----------------------------------------------------------------------------
// Name: MOS_MissionPanel::MagicDestroyAll
// Created: NLD 2005-07-27
// -----------------------------------------------------------------------------
void MOS_MissionPanel::MagicDestroyAll()
{
    assert( pPopupAgent_ != 0 );

    MOS_ASN_MsgUnitMagicAction asnMsg;
    asnMsg.GetAsnMsg().oid      = pPopupAgent_->GetID();
    asnMsg.GetAsnMsg().action.t = T_MsgUnitMagicAction_action_destruction_totale;
    asnMsg.Send( 561 );

    std::stringstream strMsg;
    strMsg << "Demande destruction totale pour agent #" << pPopupAgent_->GetID();
    MT_LOG_INFO( strMsg.str().c_str(), eSent, 0 );
}

// -----------------------------------------------------------------------------
// Name: MOS_MissionPanel::MagicDestroyComponent
// Created: APE 2004-07-29
// -----------------------------------------------------------------------------
void MOS_MissionPanel::MagicDestroyComponent()
{
    assert( pPopupAgent_ != 0 );
    MOS_App::GetApp().GetMOSServer().GetController().GetMessageMgr().SendMsgUnitMagicActionDestroyComposante( *pPopupAgent_ );

    std::stringstream strMsg;
    strMsg << "Demande destruction composante pour agent #" << pPopupAgent_->GetID();
    MT_LOG_INFO( strMsg.str().c_str(), eSent, 0 );
}

// -----------------------------------------------------------------------------
// Name: MOS_MissionPanel::MagicRecompletion
// Created: SBO 2005-07-28
// -----------------------------------------------------------------------------
void MOS_MissionPanel::MagicRecompletion()
{
    static MOS_LogisticSupplyRecompletionDialog* pDialog = new MOS_LogisticSupplyRecompletionDialog( this );
    pDialog->SetAgent( *pPopupAgent_ );
    pDialog->show();
}

// -----------------------------------------------------------------------------
// Name: MOS_MissionPanel::MagicSurrender
// Created: AGE 2005-04-06
// -----------------------------------------------------------------------------
void MOS_MissionPanel::MagicSurrender()
{
    assert( pPopupAgent_  );

    MOS_ASN_MsgUnitMagicAction asnMsg;
    asnMsg.GetAsnMsg().oid                = pPopupAgent_ ->GetID();
    asnMsg.GetAsnMsg().action.t           = T_MsgUnitMagicAction_action_se_rendre;
    asnMsg.Send( 547 );

    std::stringstream strMsg;
    strMsg << "Demande reddition pour agent #" << pPopupAgent_->GetID();
    MT_LOG_INFO( strMsg.str().c_str(), eSent, 0 );
}

// -----------------------------------------------------------------------------
// Name: MOS_MissionPanel::MagicChangeDiplomatie
// Created: AGE 2005-04-06
// -----------------------------------------------------------------------------
void MOS_MissionPanel::MagicChangeDiplomatie()
{
    static MOS_ChangeDiplomacyDialog* pDialog = new MOS_ChangeDiplomacyDialog( this );
    pDialog->Initialize( *pPopupTeam_ );
    pDialog->show();
}

// -----------------------------------------------------------------------------
// Name: MOS_MissionPanel::MagicChangeLogisticLinks
// Created: AGE 2005-04-06
// -----------------------------------------------------------------------------
void MOS_MissionPanel::MagicChangeLogisticLinks()
{
    static MOS_ChangeLogisticLinksDialog* pDialog = new MOS_ChangeLogisticLinksDialog( this );
    pDialog->SetAgent( *pPopupAgent_ );
    pDialog->show();
}

// -----------------------------------------------------------------------------
// Name: MOS_MissionPanel::MagicPushFlux
// Created: AGE 2005-04-06
// -----------------------------------------------------------------------------
void MOS_MissionPanel::MagicPushFlux()
{
    static MOS_LogisticSupplyPushFlowDialog* pDialog = new MOS_LogisticSupplyPushFlowDialog( this );
    pDialog->SetAgent( *pPopupAgent_ );
    pDialog->show();
}

// -----------------------------------------------------------------------------
// Name: MOS_MissionPanel::MagicChangeQuotas
// Created: AGE 2005-04-06
// -----------------------------------------------------------------------------
void MOS_MissionPanel::MagicChangeQuotas()
{
    static MOS_LogisticSupplyChangeQuotasDialog* pDialog = new MOS_LogisticSupplyChangeQuotasDialog( this );
    pDialog->SetAgent( *pPopupAgent_ );
    pDialog->show();
}

// -----------------------------------------------------------------------------
// Name: MOS_MissionPanel::ChangeHumanFactors
// Created: AGE 2005-09-22
// -----------------------------------------------------------------------------
void MOS_MissionPanel::ChangeHumanFactors()
{
    static MOS_ChangeHumanFactorsDialog* pDialog = new MOS_ChangeHumanFactorsDialog( this );
    pDialog->SetAgent( *pPopupAgent_ );
    pDialog->show();
}

// =============================================================================
// MAGIC ACTIONS: Populations
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MOS_MissionPanel::PopulationMagicMove
// Created: SBO 2005-10-27
// -----------------------------------------------------------------------------
void MOS_MissionPanel::PopulationMagicMove()
{
    pPopulationMagicMovePositionEditor_->PrepareNewLine( MOS_ShapeEditorMapEventFilter::ePoint );
    pPopulationMagicMovePositionEditor_->Push();
}
 
// -----------------------------------------------------------------------------
// Name: MOS_MissionPanel::PopulationMagicMoveDone
// Created: SBO 2005-10-27
// -----------------------------------------------------------------------------
void MOS_MissionPanel::PopulationMagicMoveDone()
{
    if( pPopupPopulationConcentration_ )
        pPopupPopulation_ = const_cast< MOS_Population* >( &pPopupPopulationConcentration_->GetPopulation() );
    else if( pPopupPopulationFlow_ )
        pPopupPopulation_ = const_cast< MOS_Population* >( &pPopupPopulationFlow_->GetPopulation() );
    assert( pPopupPopulation_ != 0 );

    T_PointVector& pointList = pPopulationMagicMovePositionEditor_->GetPointList();
    if( pointList.empty() )
        return;

    assert( pointList.size() == 1 );
    std::string strPos;
    MOS_App::GetApp().GetWorld().SimToMosMgrsCoord( pointList.front(), strPos );

    ASN1T_CoordUTM  coordUTM;
    coordUTM = strPos.c_str();

    MOS_ASN_MsgPopulationMagicAction asnMsg;
    asnMsg.GetAsnMsg().oid_population   = pPopupPopulation_->GetID();
    asnMsg.GetAsnMsg().action.t         = T_MsgPopulationMagicAction_action_move_to;
    asnMsg.GetAsnMsg().action.u.move_to = &coordUTM;
    asnMsg.Send( 56 );

    std::stringstream strMsg;
    strMsg << "Demande déplacement magique pour population #" << pPopupPopulation_->GetID();
    MT_LOG_INFO( strMsg.str().c_str(), eSent, 0 );

    // $$$$ SBO 2005-10-28: cancel path as for agents?
}

// -----------------------------------------------------------------------------
// Name: MOS_MissionPanel::PopulationMagicChangeAttitudeGlobal
// Created: SBO 2005-11-02
// -----------------------------------------------------------------------------
void MOS_MissionPanel::PopulationMagicChangeAttitudeGlobal( int nAttitude )
{
    PopulationMagicChangeAttitude( nAttitude, true );
}

// -----------------------------------------------------------------------------
// Name: MOS_MissionPanel::PopulationMagicChangeAttitude
// Created: SBO 2005-10-27
// -----------------------------------------------------------------------------
void MOS_MissionPanel::PopulationMagicChangeAttitude( int nAttitude, bool bGlobal /* = false */ )
{
    if( pPopupPopulationConcentration_ )
        pPopupPopulation_ = const_cast< MOS_Population* >( &pPopupPopulationConcentration_->GetPopulation() );
    else if( pPopupPopulationFlow_ )
        pPopupPopulation_ = const_cast< MOS_Population* >( &pPopupPopulationFlow_->GetPopulation() );
    assert( pPopupPopulation_ != 0 );

    MOS_ASN_MsgPopulationMagicAction asnMsg;
    asnMsg.GetAsnMsg().oid_population = pPopupPopulation_->GetID();
    asnMsg.GetAsnMsg().action.t       = T_MsgPopulationMagicAction_action_change_attitude;

    ASN1T_MagicActionPopulationChangeAttitude* pAction = new ASN1T_MagicActionPopulationChangeAttitude();

    pAction->oid_population = pPopupPopulation_->GetID();
    pAction->attitude       = ( ASN1T_EnumPopulationAttitude )nAttitude;

    if( ! bGlobal && pPopupPopulationConcentration_ )
    {
        pAction->beneficiaire.t = T_MagicActionPopulationChangeAttitude_beneficiaire_concentration;
        pAction->beneficiaire.u.concentration = pPopupPopulationConcentration_->GetID();
    }
    else if( ! bGlobal && pPopupPopulationFlow_ )
    {
        pAction->beneficiaire.t = T_MagicActionPopulationChangeAttitude_beneficiaire_flux;
        pAction->beneficiaire.u.flux = pPopupPopulationFlow_->GetID();
    }
    else
        pAction->beneficiaire.t = T_MagicActionPopulationChangeAttitude_beneficiaire_global;

    asnMsg.GetAsnMsg().action.u.change_attitude = pAction;

    asnMsg.Send( 561 );

    std::stringstream strMsg;
    strMsg << "Demande changement d'attitude pour population #" << pPopupPopulation_->GetID();
    MT_LOG_INFO( strMsg.str().c_str(), eSent, 0 );

    delete pAction;
}
    
// -----------------------------------------------------------------------------
// Name: MOS_MissionPanel::PopulationMagicDestroyAll
// Created: SBO 2005-10-27
// -----------------------------------------------------------------------------
void MOS_MissionPanel::PopulationMagicDestroyAll()
{
    if( pPopupPopulationConcentration_ )
        pPopupPopulation_ = const_cast< MOS_Population* >( &pPopupPopulationConcentration_->GetPopulation() );
    else if( pPopupPopulationFlow_ )
        pPopupPopulation_ = const_cast< MOS_Population* >( &pPopupPopulationFlow_->GetPopulation() );
    assert( pPopupPopulation_ != 0 );

    MOS_ASN_MsgPopulationMagicAction asnMsg;
    asnMsg.GetAsnMsg().oid_population = pPopupPopulation_->GetID();
    asnMsg.GetAsnMsg().action.t       = T_MsgPopulationMagicAction_action_destruction_totale;
    asnMsg.Send( 561 );

    std::stringstream strMsg;
    strMsg << "Demande destruction totale pour population #" << pPopupPopulation_->GetID();
    MT_LOG_INFO( strMsg.str().c_str(), eSent, 0 );
}
