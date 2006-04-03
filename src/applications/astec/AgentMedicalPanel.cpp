// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2005-04-01 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/AgentMedicalPanel.cpp $
// $Author: Nld $
// $Modtime: 27/04/05 18:28 $
// $Revision: 3 $
// $Workfile: AgentMedicalPanel.cpp $
//
// *****************************************************************************

#include "astec_pch.h"
#include "AgentMedicalPanel.h"
#include "LogConsignListView.h"
#include "LogMedicalConsign.h"
#include "LogMedicalConsign_ListView_Item.h"
#include "Agent.h"
#include "App.h"

// -----------------------------------------------------------------------------
// Name: AgentMedicalPanel constructor
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
AgentMedicalPanel::AgentMedicalPanel( QWidget* pParent )
    : AgentLogisticPanel_ABC( pParent )
{
    pConsignListView_       = new T_List( this, false );
    pConsignHandledListView_= new T_List( this, true );
    pConsignHandledListView_->hide();

    pState_ = new QListView( this, tr( "Etat chaine medical" ) );
    pState_->addColumn( tr( "Etat" ) );
    pState_->addColumn( "" );
    pState_->setMargin( 5 );
    pState_->setLineWidth( 2 );
    pState_->setFrameStyle( QFrame::Sunken | QFrame::Box );
    pStateChainEnabled_      = new QListViewItem( pState_, tr( "Etat chaine" ), " - " );
    pStatePriorites_         = new QListViewItem( pState_, pStateChainEnabled_, tr( "Priorités" ), " - " );  
    pStateTacticalPriorites_ = new QListViewItem( pState_, pStatePriorites_   , tr( "Priorités tactiques" ), " - " );  
    pState_->hide();
    
    pDispoReleveAmbulances_ = new QListView( this, tr( "Disponibilités ambulances relève" ) );
    pDispoReleveAmbulances_->addColumn( tr( "Disponibilités ambulances relève" ) );
    pDispoReleveAmbulances_->addColumn( tr( "Total" ) );
    pDispoReleveAmbulances_->addColumn( tr( "Disponibles" ) );
    pDispoReleveAmbulances_->addColumn( tr( "Au travail" ) );
    pDispoReleveAmbulances_->addColumn( tr( "Au repos" ) );
    pDispoReleveAmbulances_->setMargin( 5 );
    pDispoReleveAmbulances_->setLineWidth( 2 );
    pDispoReleveAmbulances_->setFrameStyle( QFrame::Sunken | QFrame::Box );
    pDispoReleveAmbulances_->setSorting( -1, FALSE );
    pDispoReleveAmbulances_->hide();

    pDispoRamassageAmbulances_ = new QListView( this, tr( "Disponibilités ambulances ramassage" ) );
    pDispoRamassageAmbulances_->addColumn( tr( "Disponibilités ambulances ramassage" ) );
    pDispoRamassageAmbulances_->addColumn( tr( "Total" ) );
    pDispoRamassageAmbulances_->addColumn( tr( "Disponibles" ) );
    pDispoRamassageAmbulances_->addColumn( tr( "Au travail" ) );
    pDispoRamassageAmbulances_->addColumn( tr( "Au repos" ) );
    pDispoRamassageAmbulances_->setMargin( 5 );
    pDispoRamassageAmbulances_->setLineWidth( 2 );
    pDispoRamassageAmbulances_->setFrameStyle( QFrame::Sunken | QFrame::Box );
    pDispoRamassageAmbulances_->setSorting( -1, FALSE );
    pDispoRamassageAmbulances_->hide();

    pDispoDoctors_ = new QListView( this, tr( "Disponibilités médecins" ) );
    pDispoDoctors_->addColumn( tr( "Disponibilités médecins" ) );
    pDispoDoctors_->addColumn( tr( "Total" ) );
    pDispoDoctors_->addColumn( tr( "Disponibles" ) );
    pDispoDoctors_->addColumn( tr( "Au travail" ) );
    pDispoDoctors_->addColumn( tr( "Au repos" ) );
    pDispoDoctors_->setMargin( 5 );
    pDispoDoctors_->setLineWidth( 2 );
    pDispoDoctors_->setFrameStyle( QFrame::Sunken | QFrame::Box );
    pDispoDoctors_->setSorting( -1, FALSE );
    pDispoDoctors_->hide();

    connect( &App::GetApp(), SIGNAL( AgentUpdated( Agent& ) ), this, SLOT( OnAgentUpdated( Agent& ) ) );
}

// -----------------------------------------------------------------------------
// Name: AgentMedicalPanel destructor
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
AgentMedicalPanel::~AgentMedicalPanel()
{
    //NOTHING
}

namespace
{
    class MedicalResolver
    {
    public:
        std::string GetName( unsigned int nUrgency ) const
        {
            switch( nUrgency )
            {
                case EnumHumanWound::blesse_urgence_1      : return "U1";
                case EnumHumanWound::blesse_urgence_2      : return "U2";
                case EnumHumanWound::blesse_urgence_3      : return "U3";
                case EnumHumanWound::blesse_urgence_extreme: return "UE";
                case EnumHumanWound::mort                  : return "Morts";
                case EnumHumanWound::non_blesse            : return "Non blessés";
            }
            return "";
        };
    };
};

// -----------------------------------------------------------------------------
// Name: AgentMedicalPanel::OnUpdate
// Created: AGE 2005-04-05
// -----------------------------------------------------------------------------
void AgentMedicalPanel::OnUpdate()
{
    if( selectedItem_.pAgent_ != 0 )
        OnAgentUpdated( *selectedItem_.pAgent_ );
    else
        OnClearSelection();
}

// -----------------------------------------------------------------------------
// Name: AgentMedicalPanel::OnClearSelection
// Created: SBO 2005-09-22
// -----------------------------------------------------------------------------
void AgentMedicalPanel::OnClearSelection()
{
    pConsignListView_         ->clear();
    pConsignListView_         ->hide();
    pConsignHandledListView_  ->clear();
    pConsignHandledListView_  ->hide();
    pState_                   ->hide();
    pDispoReleveAmbulances_   ->hide();
    pDispoRamassageAmbulances_->hide();
    pDispoDoctors_            ->hide();
}

// -----------------------------------------------------------------------------
// Name: AgentMedicalPanel::OnAgentUpdated
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentMedicalPanel::OnAgentUpdated( Agent& agent )
{
    if( ! ShouldDisplay( agent ) )
        return;

    OnClearSelection();

    DisplayConsigns( agent.requestedMedical_, *pConsignListView_ );
    DisplayConsigns( agent.handledMedical_, *pConsignHandledListView_ );
    
    if( ! agent.pMedicalData_ )
    {
        pState_->hide();
        pDispoReleveAmbulances_->hide();
        pDispoRamassageAmbulances_->hide();
        pDispoDoctors_->hide();
        return;
    }

    Agent::T_MedicalData& data = *agent.pMedicalData_;
    pStateChainEnabled_->setText( 1, data.bChainEnabled_ ? tr( "Activée" ) : tr( "Désactivée" ) );
    DisplayPriorities( data.priorities_        , *pStatePriorites_        , MedicalResolver() );
    DisplayPriorities( data.tacticalPriorities_, *pStateTacticalPriorites_, AutomateResolver() );

    pState_->show();

    DisplayLogAvailabilities( data.dispoDoctors_, *pDispoDoctors_, EquipmentResolver() );
    DisplayLogAvailabilities( data.dispoReleveAmbulances_, *pDispoReleveAmbulances_, EquipmentResolver() );
    DisplayLogAvailabilities( data.dispoRamassageAmbulances_, *pDispoRamassageAmbulances_, EquipmentResolver() );
}
