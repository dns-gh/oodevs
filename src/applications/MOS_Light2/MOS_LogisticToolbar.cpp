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

#include "MOS_Light2_pch.h"
#include "MOS_LogisticToolbar.h"
#include "moc_MOS_LogisticToolbar.cpp"

#include "MOS_AgentManager.h"
#include "MOS_ASN_Messages.h"
#include "MOS_Agent.h"


#include "MOS_MainWindow.h"
#include "MOS_Options.h"

// -----------------------------------------------------------------------------
// Name: MOS_LogisticToolbar constructor
// Created: HME 2005-11-02
// -----------------------------------------------------------------------------
MOS_LogisticToolbar::MOS_LogisticToolbar( QMainWindow* pParent )
    : QToolBar( pParent, "logistic toolbar" )
{
    this->setLabel( tr( "Options logistique" ) );

    MOS_Options& options = MOS_MainWindow::GetMainWindow().GetOptions();

    MT_ToolListButton* pLinksButton = new MT_ToolListButton( MAKE_ICON( loglink ), tr( "Afficher les liens logistiques" ), "", this, SLOT( OnLogisticLinks( float ) ), this );
    this->FillButton( *pLinksButton, options.nDisplayLogLinks_ );

    MT_ToolListButton* pMissingLinksButton = new MT_ToolListButton( MAKE_ICON( missinglog ), tr( "Afficher les liens logistiques manquant" ), "", this, SLOT( OnMissingLogisticLinks( float ) ), this );
    this->FillButton( *pMissingLinksButton, options.nDisplayMissingLogLinks_ );
    
    pAutoSetLogisticLinksButton_ = new QToolButton( MAKE_ICON( autolog ), tr( "Calcul automatique des liens logistiques" ), "", this, SLOT( OnAutoSetLogisticLinks() ), this );
    }

// -----------------------------------------------------------------------------
// Name: MOS_LogisticToolbar destructor
// Created: HME 2005-11-02
// -----------------------------------------------------------------------------
MOS_LogisticToolbar::~MOS_LogisticToolbar()
{
}

// -----------------------------------------------------------------------------
// Name: MOS_LogisticToolbar::FillButton
// Created: HME 2005-11-02
// -----------------------------------------------------------------------------
void MOS_LogisticToolbar::FillButton( MT_ToolListButton& button, MOS_Options::E_State nState )
{
    button.AddItem( tr( "Sel." ),  (float)MOS_Options::eAuto, nState == MOS_Options::eAuto );
    button.AddItem( tr( "Tous" ),  (float)MOS_Options::eOn,   nState == MOS_Options::eOn );
    button.AddItem( tr( "Aucun" ), (float)MOS_Options::eOff,  nState == MOS_Options::eOff );
    button.setMinimumWidth( 65 );
}

// -----------------------------------------------------------------------------
// Name: MOS_LogisticToolbar::OnLogisticLinks
// Created: HME 2005-11-02
// -----------------------------------------------------------------------------
void MOS_LogisticToolbar::OnLogisticLinks( float rValue )
{
    MOS_Options& options = MOS_MainWindow::GetMainWindow().GetOptions();
    options.nDisplayLogLinks_ = (MOS_Options::E_State)(int)(rValue);
}

// -----------------------------------------------------------------------------
// Name: MOS_LogisticToolbar::OnMissingLogisticLinks
// Created: HME 2005-11-02
// -----------------------------------------------------------------------------
void MOS_LogisticToolbar::OnMissingLogisticLinks( float rValue )
{
    MOS_Options& options = MOS_MainWindow::GetMainWindow().GetOptions();
    options.nDisplayMissingLogLinks_ = (MOS_Options::E_State)(int)(rValue);
}


// -----------------------------------------------------------------------------
// Name: MOS_LogisticToolbar::OnAutoSetLogisticLinks
// Created: HME 2005-11-02
// -----------------------------------------------------------------------------
void MOS_LogisticToolbar::OnAutoSetLogisticLinks()
{
    MOS_AgentManager::CT_AgentMap& agentMap = MOS_App::GetApp().GetAgentManager().GetAgentList();
    //link the BLD to the BLT and the TC2 to the BLD
    for ( MOS_AgentManager::CIT_AgentMap itAgent = agentMap.begin(); itAgent != agentMap.end(); ++itAgent )
    {
        //found a BLD
        if ( itAgent->second->IsLogisticBLD() )
        {
            MOS_Agent* pAgent = itAgent->second;
            //find the nearest BLT
            int distMin = -1;
            MOS_Agent* pSelected = 0;
            for ( MOS_AgentManager::CIT_AgentMap itAgent2 = agentMap.begin(); itAgent2 != agentMap.end(); ++itAgent2 )
            {
                MOS_Agent* pAgent2 = itAgent2->second;
                if ( pAgent2->IsLogisticBLT()
                    && (   ( pAgent->IsLogisticMaintenance() && pAgent2->IsLogisticMaintenance() )
                         ||( pAgent->IsLogisticSante() && pAgent2->IsLogisticSante() )
                         ||( pAgent->IsLogisticRavitaillement() && pAgent2->IsLogisticRavitaillement() ) )
                    && ( ( pAgent->GetPos().Distance( pAgent2->GetPos() ) < distMin ) ) ||( distMin == -1 ) )
                {
                    distMin = pAgent->GetPos().Distance( pAgent2->GetPos() );
                    pSelected = pAgent2;
                }
            }
            if ( pSelected != 0 )
            {
                //send the message
                MOS_ASN_MsgChangeLiensLogistiques asnMsg;
                asnMsg.GetAsnMsg().oid_automate = pAgent->GetID();
                asnMsg.GetAsnMsg().m.oid_ravitaillementPresent = 0;
                asnMsg.GetAsnMsg().m.oid_maintenancePresent = 0;
                asnMsg.GetAsnMsg().m.oid_santePresent = 0;
                if( pSelected->IsLogisticMaintenance() )
                {
                        asnMsg.GetAsnMsg().m.oid_maintenancePresent = 1;
		                asnMsg.GetAsnMsg().oid_maintenance = pSelected->GetID();
                }
                else
		                asnMsg.GetAsnMsg().oid_maintenance = (uint)0;
                if( pSelected->IsLogisticRavitaillement() )
                {		 
                    asnMsg.GetAsnMsg().m.oid_ravitaillementPresent = 1;
                    asnMsg.GetAsnMsg().oid_ravitaillement = pSelected->GetID();
                }
                else
		                asnMsg.GetAsnMsg().oid_ravitaillement = (uint)0;
                if( pSelected->IsLogisticSante() )
                {		        
                    asnMsg.GetAsnMsg().m.oid_santePresent = 1;
                    asnMsg.GetAsnMsg().oid_sante = pSelected->GetID();
                }
                else
		                asnMsg.GetAsnMsg().oid_sante = (uint)0;
                asnMsg.Send( 57 );
            }
        }
        
        //found a TC2
        if ( itAgent->second->IsLogisticTC2() )
        {
            MOS_Agent* pAgent = itAgent->second;
            //find the nearest BLDs
            int distMinRav = -1; int distMinSan = -1; int distMinMai = -1;
            MOS_Agent* pSelectedRav = 0; MOS_Agent* pSelectedSan = 0; MOS_Agent* pSelectedMai = 0;
            for ( MOS_AgentManager::CIT_AgentMap itAgent2 = agentMap.begin(); itAgent2 != agentMap.end(); ++itAgent2 )
            {
                MOS_Agent* pAgent2 = itAgent2->second;
                if ( pAgent2->IsLogisticBLD()
                    && pAgent2->IsLogisticMaintenance()
                    && ( ( pAgent->GetPos().Distance( pAgent2->GetPos() ) < distMinMai ) ||( distMinMai == -1 ) ) )
                {
                    distMinMai = pAgent->GetPos().Distance( pAgent2->GetPos() );
                    pSelectedMai = pAgent2;
                }
                else if ( pAgent2->IsLogisticBLD()
                    && pAgent2->IsLogisticSante()
                    && ( ( pAgent->GetPos().Distance( pAgent2->GetPos() ) < distMinSan ) ||( distMinSan == -1 ) ) )
                {
                    distMinSan = pAgent->GetPos().Distance( pAgent2->GetPos() );
                    pSelectedSan = pAgent2;
                }
                else if ( pAgent2->IsLogisticBLD()
                    && pAgent2->IsLogisticRavitaillement()
                    && ( ( pAgent->GetPos().Distance( pAgent2->GetPos() ) < distMinRav ) ||( distMinRav == -1 ) ) )
                {
                    distMinRav = pAgent->GetPos().Distance( pAgent2->GetPos() );
                    pSelectedRav = pAgent2;
                }
            }
            //send the message
            MOS_ASN_MsgChangeLiensLogistiques asnMsg;
            asnMsg.GetAsnMsg().oid_automate = pAgent->GetID();
            asnMsg.GetAsnMsg().m.oid_ravitaillementPresent = 0;
            asnMsg.GetAsnMsg().m.oid_maintenancePresent = 0;
            asnMsg.GetAsnMsg().m.oid_santePresent = 0;
            if( pSelectedMai != 0 )
            {
                asnMsg.GetAsnMsg().oid_maintenance = pSelectedMai->GetID();
                asnMsg.GetAsnMsg().m.oid_maintenancePresent = 1;
            }
            else
                asnMsg.GetAsnMsg().oid_maintenance = (uint)0;
            if( pSelectedRav != 0 )
            {
                asnMsg.GetAsnMsg().m.oid_ravitaillementPresent = 1;
                asnMsg.GetAsnMsg().oid_ravitaillement = pSelectedRav->GetID();
            }
            else
                asnMsg.GetAsnMsg().oid_ravitaillement = (uint)0;
            if( pSelectedSan != 0 )
            {
                asnMsg.GetAsnMsg().oid_sante = pSelectedSan->GetID();
                asnMsg.GetAsnMsg().m.oid_santePresent = 1;
            }
            else
                asnMsg.GetAsnMsg().oid_sante = (uint)0;
            asnMsg.Send( 57 );
        }
    }
}


