// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "astec_pch.h"
#include "LogisticToolbar.h"
#include "moc_LogisticToolbar.cpp"

#include "AgentManager.h"
#include "ASN_Messages.h"
#include "Agent.h"


#include "MainWindow.h"
#include "Options.h"

// -----------------------------------------------------------------------------
// Name: LogisticToolbar constructor
// Created: HME 2005-11-02
// -----------------------------------------------------------------------------
LogisticToolbar::LogisticToolbar( QMainWindow* pParent )
    : QToolBar( pParent, "logistic toolbar" )
    , options_( App::GetApp().GetOptions() )
{
    this->setLabel( tr( "Options logistique" ) );

    MT_ToolListButton* pLinksButton = new MT_ToolListButton( MAKE_ICON( loglink ), tr( "Afficher les liens logistiques" ), "", this, SLOT( OnLogisticLinks( float ) ), this );
    this->FillButton( *pLinksButton, options_.nDisplayLogLinks_ );

    MT_ToolListButton* pMissingLinksButton = new MT_ToolListButton( MAKE_ICON( missinglog ), tr( "Afficher les liens logistiques manquant" ), "", this, SLOT( OnMissingLogisticLinks( float ) ), this );
    this->FillButton( *pMissingLinksButton, options_.nDisplayMissingLogLinks_ );
    
    //pAutoSetLogisticLinksButton_ = new QToolButton( MAKE_ICON( autolog ), tr( "Calcul automatique des liens logistiques" ), "", this, SLOT( OnAutoSetLogisticLinks() ), this );
    
    pRealTimeLogButton_ = new QToolButton( MAKE_ICON( realtimelog ), tr( "Affichage temps réel des actions logistiques" ), "", this, SLOT( OnRealTimeLog() ), this );  
    pRealTimeLogButton_->setToggleButton( true );
    pRealTimeLogButton_->setOn( options_.bDisplayRealTimeLog_ );

    pAggregateAllButton_ = new QToolButton( MAKE_ICON( aggregate ), tr( "Aggréger tous les automates" ), "", this, SLOT( OnAggregateAll() ), this );
    pAggregateAllButton_->setToggleButton( false );
    pAggregateAllButton_->setOn( false );

    pDesaggregateAllButton_ = new QToolButton( MAKE_ICON( desaggregate ), tr( "Désaggréger tous les automates" ), "", this, SLOT( OnDesaggregateAll() ), this );
    pDesaggregateAllButton_->setToggleButton( false );
    pDesaggregateAllButton_->setOn( false );
}

// -----------------------------------------------------------------------------
// Name: LogisticToolbar destructor
// Created: HME 2005-11-02
// -----------------------------------------------------------------------------
LogisticToolbar::~LogisticToolbar()
{
}

// -----------------------------------------------------------------------------
// Name: LogisticToolbar::FillButton
// Created: HME 2005-11-02
// -----------------------------------------------------------------------------
void LogisticToolbar::FillButton( MT_ToolListButton& button, Options::E_State nState )
{
    button.AddItem( tr( "Sel." ),  (float)Options::eAuto, nState == Options::eAuto );
    button.AddItem( tr( "Tous" ),  (float)Options::eOn,   nState == Options::eOn );
    button.AddItem( tr( "Aucun" ), (float)Options::eOff,  nState == Options::eOff );
    button.setMinimumWidth( 65 );
}

// -----------------------------------------------------------------------------
// Name: LogisticToolbar::OnLogisticLinks
// Created: HME 2005-11-02
// -----------------------------------------------------------------------------
void LogisticToolbar::OnLogisticLinks( float rValue )
{
    options_.nDisplayLogLinks_ = (Options::E_State)(int)(rValue);
}

// -----------------------------------------------------------------------------
// Name: LogisticToolbar::OnMissingLogisticLinks
// Created: HME 2005-11-02
// -----------------------------------------------------------------------------
void LogisticToolbar::OnMissingLogisticLinks( float rValue )
{
    options_.nDisplayMissingLogLinks_ = (Options::E_State)(int)(rValue);
}

// -----------------------------------------------------------------------------
// Name: LogisticToolbar::OnRealTimeLog
// Created: HME 2005-11-02
// -----------------------------------------------------------------------------
void LogisticToolbar::OnRealTimeLog()
{
    options_.bDisplayRealTimeLog_ = !options_.bDisplayRealTimeLog_;
    pRealTimeLogButton_->setOn( options_.bDisplayRealTimeLog_ );
}

//// -----------------------------------------------------------------------------
//// Name: LogisticToolbar::OnAutoSetLogisticLinks
//// Created: HME 2005-11-02
//// -----------------------------------------------------------------------------
//void LogisticToolbar::OnAutoSetLogisticLinks()
//{
//    if ( QMessageBox::question(
//            this,
//            tr("Attention"),
//            tr("Ecraser les liens existants ?"),
//            tr("&Oui"), tr("&Non"),
//            QString::null, 0, 1 ) )
//        return;
//    AgentManager::CT_AgentMap& agentMap = App::GetApp().GetAgentManager().GetAgentList();
//    //link the BLD to the BLT and the TC2 to the BLD
//    for ( AgentManager::CIT_AgentMap itAgent = agentMap.begin(); itAgent != agentMap.end(); ++itAgent )
//    {
//        //found a BLD
//        if ( itAgent->second->IsLogisticBLD() )
//        {
//            Agent* pAgent = itAgent->second;
//            //find the nearest BLT
//            int distMin = -1;
//            Agent* pSelected = 0;
//            for ( AgentManager::CIT_AgentMap itAgent2 = agentMap.begin(); itAgent2 != agentMap.end(); ++itAgent2 )
//            {
//                Agent* pAgent2 = itAgent2->second;
//                if ( pAgent2->IsLogisticBLT()
//                    && (   ( pAgent->IsLogisticMaintenance() && pAgent2->IsLogisticMaintenance() )
//                         ||( pAgent->IsLogisticSante() && pAgent2->IsLogisticSante() )
//                         ||( pAgent->IsLogisticRavitaillement() && pAgent2->IsLogisticRavitaillement() ) )
//                    && ( ( pAgent->GetPos().Distance( pAgent2->GetPos() ) < distMin ) ) ||( distMin == -1 ) )
//                {
//                    distMin = pAgent->GetPos().Distance( pAgent2->GetPos() );
//                    pSelected = pAgent2;
//                }
//            }
//            if ( pSelected != 0 )
//            {
//                //send the message
//                ASN_MsgChangeLiensLogistiques asnMsg;
//                asnMsg.GetAsnMsg().oid_automate = pAgent->GetID();
//                asnMsg.GetAsnMsg().m.oid_ravitaillementPresent = 0;
//                asnMsg.GetAsnMsg().m.oid_maintenancePresent = 0;
//                asnMsg.GetAsnMsg().m.oid_santePresent = 0;
//                if( pSelected->IsLogisticMaintenance() )
//                {
//                        asnMsg.GetAsnMsg().m.oid_maintenancePresent = 1;
//		                asnMsg.GetAsnMsg().oid_maintenance = pSelected->GetID();
//                }
//                else
//		                asnMsg.GetAsnMsg().oid_maintenance = (uint)0;
//                if( pSelected->IsLogisticRavitaillement() )
//                {		 
//                    asnMsg.GetAsnMsg().m.oid_ravitaillementPresent = 1;
//                    asnMsg.GetAsnMsg().oid_ravitaillement = pSelected->GetID();
//                }
//                else
//		                asnMsg.GetAsnMsg().oid_ravitaillement = (uint)0;
//                if( pSelected->IsLogisticSante() )
//                {		        
//                    asnMsg.GetAsnMsg().m.oid_santePresent = 1;
//                    asnMsg.GetAsnMsg().oid_sante = pSelected->GetID();
//                }
//                else
//		                asnMsg.GetAsnMsg().oid_sante = (uint)0;
//                asnMsg.Send( 57 );
//            }
//        }
//        
//        //found a TC2
//        if ( itAgent->second->IsLogisticTC2() )
//        {
//            Agent* pAgent = itAgent->second;
//            //find the nearest BLDs
//            int distMinRav = -1; int distMinSan = -1; int distMinMai = -1;
//            Agent* pSelectedRav = 0; Agent* pSelectedSan = 0; Agent* pSelectedMai = 0;
//            for ( AgentManager::CIT_AgentMap itAgent2 = agentMap.begin(); itAgent2 != agentMap.end(); ++itAgent2 )
//            {
//                Agent* pAgent2 = itAgent2->second;
//                if ( pAgent2->IsLogisticBLD()
//                    && pAgent2->IsLogisticMaintenance()
//                    && ( ( pAgent->GetPos().Distance( pAgent2->GetPos() ) < distMinMai ) ||( distMinMai == -1 ) ) )
//                {
//                    distMinMai = pAgent->GetPos().Distance( pAgent2->GetPos() );
//                    pSelectedMai = pAgent2;
//                }
//                else if ( pAgent2->IsLogisticBLD()
//                    && pAgent2->IsLogisticSante()
//                    && ( ( pAgent->GetPos().Distance( pAgent2->GetPos() ) < distMinSan ) ||( distMinSan == -1 ) ) )
//                {
//                    distMinSan = pAgent->GetPos().Distance( pAgent2->GetPos() );
//                    pSelectedSan = pAgent2;
//                }
//                else if ( pAgent2->IsLogisticBLD()
//                    && pAgent2->IsLogisticRavitaillement()
//                    && ( ( pAgent->GetPos().Distance( pAgent2->GetPos() ) < distMinRav ) ||( distMinRav == -1 ) ) )
//                {
//                    distMinRav = pAgent->GetPos().Distance( pAgent2->GetPos() );
//                    pSelectedRav = pAgent2;
//                }
//            }
//            //send the message
//            ASN_MsgChangeLiensLogistiques asnMsg;
//            asnMsg.GetAsnMsg().oid_automate = pAgent->GetID();
//            asnMsg.GetAsnMsg().m.oid_ravitaillementPresent = 0;
//            asnMsg.GetAsnMsg().m.oid_maintenancePresent = 0;
//            asnMsg.GetAsnMsg().m.oid_santePresent = 0;
//            if( pSelectedMai != 0 )
//            {
//                asnMsg.GetAsnMsg().oid_maintenance = pSelectedMai->GetID();
//                asnMsg.GetAsnMsg().m.oid_maintenancePresent = 1;
//            }
//            else
//                asnMsg.GetAsnMsg().oid_maintenance = (uint)0;
//            if( pSelectedRav != 0 )
//            {
//                asnMsg.GetAsnMsg().m.oid_ravitaillementPresent = 1;
//                asnMsg.GetAsnMsg().oid_ravitaillement = pSelectedRav->GetID();
//            }
//            else
//                asnMsg.GetAsnMsg().oid_ravitaillement = (uint)0;
//            if( pSelectedSan != 0 )
//            {
//                asnMsg.GetAsnMsg().oid_sante = pSelectedSan->GetID();
//                asnMsg.GetAsnMsg().m.oid_santePresent = 1;
//            }
//            else
//                asnMsg.GetAsnMsg().oid_sante = (uint)0;
//            asnMsg.Send( 57 );
//        }
//    }
//}

// -----------------------------------------------------------------------------
// Name: LogisticToolbar::OnAggregateAll
// Created: HME 2005-12-02
// -----------------------------------------------------------------------------
void LogisticToolbar::OnAggregateAll()
{
  AgentManager::CT_AgentMap& agentMap = App::GetApp().GetAgentManager().GetAgentList();
    //link the BLD to the BLT and the TC2 to the BLD
    for ( AgentManager::CIT_AgentMap itAgent = agentMap.begin(); itAgent != agentMap.end(); ++itAgent )
        if( itAgent->second->IsAutomate() )
            itAgent->second->SetAggregation( true );
}

// -----------------------------------------------------------------------------
// Name: LogisticToolbar::OnDesaggregateAll
// Created: HME 2005-12-02
// -----------------------------------------------------------------------------
void LogisticToolbar::OnDesaggregateAll()
{
  AgentManager::CT_AgentMap& agentMap = App::GetApp().GetAgentManager().GetAgentList();
    //link the BLD to the BLT and the TC2 to the BLD
    for ( AgentManager::CIT_AgentMap itAgent = agentMap.begin(); itAgent != agentMap.end(); ++itAgent )
        if( itAgent->second->IsAutomate() )
            itAgent->second->SetAggregation( false );
}

