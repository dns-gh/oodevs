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
#include "PopulationCreationPanel.h"
#include "moc_PopulationCreationPanel.cpp"

#include "AgentManager.h"
#include "Team.h"
#include "App.h"
#include "MainWindow.h"
#include "ParamLocation.h"
#include "Population.h"
#include "TypePopulation.h"

// -----------------------------------------------------------------------------
// Name: PopulationCreationPanel constructor
// Created: HME 2005-10-17
// -----------------------------------------------------------------------------
PopulationCreationPanel::PopulationCreationPanel( QTabWidget* pParent )
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
    pType_ = new MT_ValuedComboBox< TypePopulation* >( this );
    pLayout->addWidget( pType_, 4, 1, Qt::AlignRight );  

    const AgentManager::T_TeamMap& teamMap = App::GetApp().GetAgentManager().GetTeams();
    for( AgentManager::CIT_TeamMap itTeam = teamMap.begin(); itTeam != teamMap.end(); ++itTeam )
        pTeamCombo_->AddItem( (*itTeam).second->GetName().c_str(), (*itTeam).first );

    pAttitudeCombo_->AddItem( ENT_Tr::ConvertFromPopulationAttitude( ePopulationAttitude_Calme ).c_str(), ePopulationAttitude_Calme );
    pAttitudeCombo_->AddItem( ENT_Tr::ConvertFromPopulationAttitude( ePopulationAttitude_Agitee ).c_str(), ePopulationAttitude_Calme );
    pAttitudeCombo_->AddItem( ENT_Tr::ConvertFromPopulationAttitude( ePopulationAttitude_Excitee ).c_str(), ePopulationAttitude_Calme );
    pAttitudeCombo_->AddItem( ENT_Tr::ConvertFromPopulationAttitude( ePopulationAttitude_Agressive ).c_str(), ePopulationAttitude_Calme );

    QPushButton* pOkButton = new QPushButton( tr( "Créer" ), this );
    pLayout->addWidget( pOkButton, 7, 1, Qt::AlignRight );

    pLayout->addWidget( new QLabel( tr( "Emplacement:" ), this ), 5, 0, Qt::AlignLeft );
    pLocation_ = new ParamLocation( asnLocation_, "", "Emplacement population", this, false );
    pLayout->addWidget( pLocation_, 5, 1, Qt::AlignRight );

    pTeamCombo_->setMinimumWidth( pAttitudeCombo_->width() );
    pNbPersons_->setMinimumWidth( pAttitudeCombo_->width() );
    pName_->setMinimumWidth( pAttitudeCombo_->width() );
    pType_->setMinimumWidth( pAttitudeCombo_->width() );

    const AgentManager::T_TypePopulationMap* typePopMap = App::GetApp().GetAgentManager().GetTypePopulations();
    for ( AgentManager::CIT_TypePopulationMap it = typePopMap->begin(); it != typePopMap->end(); ++it )
        pType_->AddItem( it->second->GetName().c_str(), it->second );     
    
    connect( &App::GetApp(), SIGNAL( TeamCreated( Team& ) ), this,   SLOT( OnTeamCreated( Team& ) ) );
    connect( &App::GetApp(), SIGNAL( TypePopulationCreated( TypePopulation& ) ), this,   SLOT( OnTypePopulationCreated( TypePopulation& ) ) );
    connect( pOkButton, SIGNAL( clicked() ), this, SLOT( OnOk() ) );
    connect( &MainWindow::GetMainWindow(), SIGNAL( NewPopupMenu( QPopupMenu&, const ActionContext& ) ), this,   SLOT( FillRemotePopupMenu( QPopupMenu&, const ActionContext& ) ) );
    connect( &App::GetApp(), SIGNAL( ConnexionStatusChanged( bool ) ), this,   SLOT( OnConnexionStatusChanged( bool ) ) );
    connect( &MainWindow::GetMainWindow(), SIGNAL( ElementSelected( SelectedElement& ) ), this,   SLOT( SetSelectedElement( SelectedElement& ) ) );
}

// -----------------------------------------------------------------------------
// Name: PopulationCreationPanel destructor
// Created: HME 2005-10-17
// -----------------------------------------------------------------------------
PopulationCreationPanel::~PopulationCreationPanel()
{
}


// -----------------------------------------------------------------------------
// Name: PopulationCreationPanel::FillRemotePopupMenu
// Created: HME 2005-10-17
// -----------------------------------------------------------------------------
void PopulationCreationPanel::FillRemotePopupMenu( QPopupMenu& popupMenu, const ActionContext& context )
{
    if( pParent_->currentPage() == this && this->isVisible() )
        pLocation_->FillRemotePopupMenu( popupMenu, context );
}

// -----------------------------------------------------------------------------
// Name: PopulationCreationPanel::OnOk
// Created: HME 2005-10-17
// -----------------------------------------------------------------------------
void PopulationCreationPanel::OnOk()
{
    if( ! pLocation_->CheckValidity() )
        return;
    if( App::GetApp().IsODBEdition() )
    {
        E_PopulationAttitude attitude = pAttitudeCombo_->GetValue();
        Team& team = *App::GetApp().GetAgentManager().FindTeam( pTeamCombo_->GetValue() );
        int persons = pNbPersons_->value();
        TypePopulation* type = pType_->GetValue();
        std::string name = (std::string) pName_->text();
        MT_Vector2D point = *(pLocation_->GetPointList().begin());
        Population& pop = *new Population( point, attitude, persons, team , name, type);
        App::GetApp().GetAgentManager().AddPopulation( pop );
        pLocation_->Clear();
    }
}

// -----------------------------------------------------------------------------
// Name: PopulationCreationPanel::OnTeamCreated
// Created: HME 2005-10-17
// -----------------------------------------------------------------------------
void PopulationCreationPanel::OnTeamCreated( Team& team )
{
    pTeamCombo_->Clear();

    const AgentManager::T_TeamMap& teamMap = App::GetApp().GetAgentManager().GetTeams();
    for( AgentManager::CIT_TeamMap itTeam = teamMap.begin(); itTeam != teamMap.end(); ++itTeam )
        pTeamCombo_->AddItem( (*itTeam).second->GetName().c_str(), (*itTeam).first );

    pTeamCombo_->AddItem( team.GetName().c_str(), team.GetID() );
}

// -----------------------------------------------------------------------------
// Name: PopulationCreationPanel::OnTypePopulationCreated
// Created: HME 2005-10-19
// -----------------------------------------------------------------------------
void PopulationCreationPanel::OnTypePopulationCreated( TypePopulation& type )
{
    pType_->AddItem( type.GetName().c_str(), &type );
    pType_->show();
}

// -----------------------------------------------------------------------------
// Name: PopulationCreationPanel::OnConnexionStatusChanged
// Created: HME 2005-10-17
// -----------------------------------------------------------------------------
void PopulationCreationPanel::OnConnexionStatusChanged( bool bConnected )
{
    if( !bConnected )
        pTeamCombo_->Clear();
}


// -----------------------------------------------------------------------------
// Name: PopulationCreationPanel::SetSelectedElement
// Created: HME 2006-01-03
// -----------------------------------------------------------------------------
void PopulationCreationPanel::SetSelectedElement( SelectedElement& selectedElement )
{
    selectedElement_ = selectedElement;
}