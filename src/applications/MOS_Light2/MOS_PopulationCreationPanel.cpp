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
#include "MOS_PopulationCreationPanel.h"
#include "moc_MOS_PopulationCreationPanel.cpp"
#include "MOS_AgentManager.h"
#include "MOS_Team.h"
#include "MOS_App.h"
#include "MOS_MainWindow.h"
#include "MOS_ParamLocation.h"
#include "MOS_Population.h"
#include "MOS_TypePopulation.h"

// -----------------------------------------------------------------------------
// Name: MOS_PopulationCreationPanel constructor
// Created: HME 2005-10-17
// -----------------------------------------------------------------------------
MOS_PopulationCreationPanel::MOS_PopulationCreationPanel( QTabWidget* pParent )
    : QWidget ( pParent )
    , pParent_( pParent )
{
    QGridLayout* pLayout = new QGridLayout( this, 8, 2, 4 );
    pLayout->setRowStretch( 6, 1 );

    pLayout->addWidget( new QLabel( tr( "Camp:" ), this ), 0, 0, Qt::AlignLeft );
    pTeamCombo_ = new MT_ValuedComboBox< uint >( this );
    pLayout->addWidget( pTeamCombo_, 0, 1, Qt::AlignRight );

    pLayout->addWidget( new QLabel( tr( "Attitude:" ), this ), 1, 0, Qt::AlignLeft );
    pAttitudeCombo_ = new MT_ValuedComboBox< E_PopulationAttitude >( this );
    pLayout->addWidget( pAttitudeCombo_, 1, 1, Qt::AlignRight );

    pLayout->addWidget( new QLabel( tr( "Nb de Personnes:" ), this ), 2, 0, Qt::AlignLeft );
    pNbPersons_ = new QSpinBox( 1, 900000, 1, this );
    pLayout->addWidget( pNbPersons_, 2, 1, Qt::AlignRight );

    pLayout->addWidget( new QLabel( tr( "Nom:" ), this ), 3, 0, Qt::AlignLeft );
    pName_ = new QLineEdit( tr(""), this );
    pLayout->addWidget( pName_, 3, 1, Qt::AlignRight );   

    pLayout->addWidget( new QLabel( tr( "Type:" ), this ), 4, 0, Qt::AlignLeft );
    pType_ = new MT_ValuedComboBox< MOS_TypePopulation* >( this );
    pLayout->addWidget( pType_, 4, 1, Qt::AlignRight );  

    const MOS_AgentManager::T_TeamMap& teamMap = MOS_App::GetApp().GetAgentManager().GetTeams();
    for( MOS_AgentManager::CIT_TeamMap itTeam = teamMap.begin(); itTeam != teamMap.end(); ++itTeam )
        pTeamCombo_->AddItem( (*itTeam).second->GetName().c_str(), (*itTeam).first );

    pAttitudeCombo_->AddItem( ENT_Tr::ConvertFromPopulationAttitude( ePopulationAttitude_Calme ).c_str(), ePopulationAttitude_Calme );
    pAttitudeCombo_->AddItem( ENT_Tr::ConvertFromPopulationAttitude( ePopulationAttitude_Agitee ).c_str(), ePopulationAttitude_Calme );
    pAttitudeCombo_->AddItem( ENT_Tr::ConvertFromPopulationAttitude( ePopulationAttitude_Excitee ).c_str(), ePopulationAttitude_Calme );
    pAttitudeCombo_->AddItem( ENT_Tr::ConvertFromPopulationAttitude( ePopulationAttitude_Agressive ).c_str(), ePopulationAttitude_Calme );

    QPushButton* pOkButton = new QPushButton( tr( "Créer" ), this );
    pLayout->addWidget( pOkButton, 7, 1, Qt::AlignRight );

    pLayout->addWidget( new QLabel( tr( "Emplacement:" ), this ), 5, 0, Qt::AlignLeft );
    pLocation_ = new MOS_ParamLocation( asnLocation_, "", "Emplacement population", this );
    pLayout->addWidget( pLocation_, 5, 1, Qt::AlignRight );

    pTeamCombo_->setMinimumWidth( pAttitudeCombo_->width() );
    pNbPersons_->setMinimumWidth( pAttitudeCombo_->width() );
    pName_->setMinimumWidth( pAttitudeCombo_->width() );
    pType_->setMinimumWidth( pAttitudeCombo_->width() );

    const MOS_AgentManager::T_TypePopulationMap* typePopMap = MOS_App::GetApp().GetAgentManager().GetTypePopulations();
    for ( MOS_AgentManager::CIT_TypePopulationMap it = typePopMap->begin(); it != typePopMap->end(); ++it )
        pType_->AddItem( it->second->GetName().c_str(), it->second );     
    
    connect( &MOS_App::GetApp(), SIGNAL( TeamCreated( MOS_Team& ) ), this,   SLOT( OnTeamCreated( MOS_Team& ) ) );
    connect( &MOS_App::GetApp(), SIGNAL( TypePopulationCreated( MOS_TypePopulation& ) ), this,   SLOT( OnTypePopulationCreated( MOS_TypePopulation& ) ) );
    connect( pOkButton, SIGNAL( clicked() ), this, SLOT( OnOk() ) );
    connect( &MOS_MainWindow::GetMainWindow(), SIGNAL( NewPopupMenu( QPopupMenu&, const MOS_ActionContext& ) ), this,   SLOT( FillRemotePopupMenu( QPopupMenu&, const MOS_ActionContext& ) ) );
    connect( &MOS_App::GetApp(), SIGNAL( ConnexionStatusChanged( bool ) ), this,   SLOT( OnConnexionStatusChanged( bool ) ) );
}

// -----------------------------------------------------------------------------
// Name: MOS_PopulationCreationPanel destructor
// Created: HME 2005-10-17
// -----------------------------------------------------------------------------
MOS_PopulationCreationPanel::~MOS_PopulationCreationPanel()
{
}


// -----------------------------------------------------------------------------
// Name: MOS_PopulationCreationPanel::FillRemotePopupMenu
// Created: HME 2005-10-17
// -----------------------------------------------------------------------------
void MOS_PopulationCreationPanel::FillRemotePopupMenu( QPopupMenu& popupMenu, const MOS_ActionContext& context )
{
    if( pParent_->currentPage() == this && this->isVisible() )
        pLocation_->FillRemotePopupMenu( popupMenu, context );
}

// -----------------------------------------------------------------------------
// Name: MOS_PopulationCreationPanel::OnOk
// Created: HME 2005-10-17
// -----------------------------------------------------------------------------
void MOS_PopulationCreationPanel::OnOk()
{
    if( ! pLocation_->CheckValidity() )
        return;
    if( MOS_App::GetApp().IsODBEdition() )
    {
        E_PopulationAttitude attitude = pAttitudeCombo_->GetValue();
        MOS_Team& team = *MOS_App::GetApp().GetAgentManager().FindTeam( pTeamCombo_->GetValue() );
        int persons = pNbPersons_->value();
        MOS_TypePopulation* type = pType_->GetValue();
        std::string name = (std::string) pName_->text();
        MT_Vector2D point = *(pLocation_->GetPointList().begin());
        MOS_Population& pop = *new MOS_Population( point, attitude, persons, team , name, type);
        MOS_App::GetApp().GetAgentManager().AddPopulation( pop );
        pLocation_->Clear();
    }
}

// -----------------------------------------------------------------------------
// Name: MOS_PopulationCreationPanel::SetSelectedElement
// Created: HME 2005-10-17
// -----------------------------------------------------------------------------
void MOS_PopulationCreationPanel::SetSelectedElement( MOS_SelectedElement& selectedElement )
{
}

// -----------------------------------------------------------------------------
// Name: MOS_PopulationCreationPanel::OnTeamCreated
// Created: HME 2005-10-17
// -----------------------------------------------------------------------------
void MOS_PopulationCreationPanel::OnTeamCreated           ( MOS_Team& team )
{
    pTeamCombo_->Clear();

    const MOS_AgentManager::T_TeamMap& teamMap = MOS_App::GetApp().GetAgentManager().GetTeams();
    for( MOS_AgentManager::CIT_TeamMap itTeam = teamMap.begin(); itTeam != teamMap.end(); ++itTeam )
        pTeamCombo_->AddItem( (*itTeam).second->GetName().c_str(), (*itTeam).first );

    pTeamCombo_->AddItem( team.GetName().c_str(), team.GetID() );
}

// -----------------------------------------------------------------------------
// Name: MOS_PopulationCreationPanel::OnTypePopulationCreated
// Created: HME 2005-10-19
// -----------------------------------------------------------------------------
void MOS_PopulationCreationPanel::OnTypePopulationCreated( MOS_TypePopulation& type )
{
    pType_->AddItem( type.GetName().c_str(), &type );
    pType_->show();
}

// -----------------------------------------------------------------------------
// Name: MOS_PopulationCreationPanel::OnConnexionStatusChanged
// Created: HME 2005-10-17
// -----------------------------------------------------------------------------
void MOS_PopulationCreationPanel::OnConnexionStatusChanged( bool bConnected )
{
    if( !bConnected )
        pTeamCombo_->Clear();
}


// -----------------------------------------------------------------------------
// Name: MOS_PopulationCreationPanel::OnDeletePopulation
// Created: HME 2005-10-17
// -----------------------------------------------------------------------------
void MOS_PopulationCreationPanel::OnDeletePopulation()
{
}
