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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_AgentMedicalPanel.cpp $
// $Author: Nld $
// $Modtime: 27/04/05 18:28 $
// $Revision: 3 $
// $Workfile: MOS_AgentMedicalPanel.cpp $
//
// *****************************************************************************

#include "MOS_Light2_pch.h"
#include "MOS_AgentMedicalPanel.h"
#include "MOS_LogConsignListView.h"
#include "MOS_LogMedicalConsign.h"
#include "MOS_LogMedicalConsign_ListView_Item.h"
#include "MOS_Agent.h"
#include "MOS_App.h"

// -----------------------------------------------------------------------------
// Name: MOS_AgentMedicalPanel constructor
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
MOS_AgentMedicalPanel::MOS_AgentMedicalPanel( QWidget* pParent )
    : MOS_AgentLogisticPanel_ABC( pParent )
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
    pStateTempsBordee_       = new QListViewItem( pState_, pStateChainEnabled_, tr( "Temps de bordée" ), " - " );
    pStatePriorites_         = new QListViewItem( pState_, pStateTempsBordee_ , tr( "Priorités" ), " - " );  
    pStateTacticalPriorites_ = new QListViewItem( pState_, pStatePriorites_   , tr( "Priorités tactiques" ), " - " );  
    pState_->hide();
    
    pDispoReleveAmbulances_ = new QListView( this, tr( "Disponibilités ambulances relève" ) );
    pDispoReleveAmbulances_->addColumn( tr( "Disponibilités ambulances relève" ) );
    pDispoReleveAmbulances_->addColumn( "" );
    pDispoReleveAmbulances_->setMargin( 5 );
    pDispoReleveAmbulances_->setLineWidth( 2 );
    pDispoReleveAmbulances_->setFrameStyle( QFrame::Sunken | QFrame::Box );
    pDispoReleveAmbulances_->setSorting( -1, FALSE );
    pDispoReleveAmbulances_->hide();

    pDispoRamassageAmbulances_ = new QListView( this, tr( "Disponibilités ambulances ramassage" ) );
    pDispoRamassageAmbulances_->addColumn( tr( "Disponibilités ambulances ramassage" ) );
    pDispoRamassageAmbulances_->addColumn( "" );
    pDispoRamassageAmbulances_->setMargin( 5 );
    pDispoRamassageAmbulances_->setLineWidth( 2 );
    pDispoRamassageAmbulances_->setFrameStyle( QFrame::Sunken | QFrame::Box );
    pDispoRamassageAmbulances_->setSorting( -1, FALSE );
    pDispoRamassageAmbulances_->hide();

    pDispoDoctors_ = new QListView( this, tr( "Disponibilités médecins" ) );
    pDispoDoctors_->addColumn( tr( "Disponibilités médecins" ) );
    pDispoDoctors_->addColumn( "" );
    pDispoDoctors_->setMargin( 5 );
    pDispoDoctors_->setLineWidth( 2 );
    pDispoDoctors_->setFrameStyle( QFrame::Sunken | QFrame::Box );
    pDispoDoctors_->setSorting( -1, FALSE );
    pDispoDoctors_->hide();

    connect( &MOS_App::GetApp(), SIGNAL( AgentUpdated( MOS_Agent& ) ), this, SLOT( OnAgentUpdated( MOS_Agent& ) ) );
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentMedicalPanel destructor
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
MOS_AgentMedicalPanel::~MOS_AgentMedicalPanel()
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
// Name: MOS_AgentMedicalPanel::OnUpdate
// Created: AGE 2005-04-05
// -----------------------------------------------------------------------------
void MOS_AgentMedicalPanel::OnUpdate()
{
    OnAgentUpdated( *selectedItem_.pAgent_ );
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentMedicalPanel::OnAgentUpdated
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void MOS_AgentMedicalPanel::OnAgentUpdated( MOS_Agent& agent )
{
    if( ! ShouldDisplay( agent ) )
        return;

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

    MOS_Agent::T_MedicalData& data = *agent.pMedicalData_;
    pStateChainEnabled_->setText( 1, data.bChainEnabled_ ? tr( "Activée" ) : tr( "Désactivée" ) );
    pStateTempsBordee_ ->setText( 1, ( QString( "%1 " ) + tr( "heures" ) ).arg( data.nTempsBordee_ ) );
    DisplayPriorities( data.priorities_        , *pStatePriorites_        , MedicalResolver() );
    DisplayPriorities( data.tacticalPriorities_, *pStateTacticalPriorites_, AutomateResolver() );

    pState_->show();

    DisplayAvailabilities( data.dispoDoctors_, *pDispoDoctors_, EquipmentResolver(), "%" );
    DisplayAvailabilities( data.dispoReleveAmbulances_, *pDispoReleveAmbulances_, EquipmentResolver(), "%" );
    DisplayAvailabilities( data.dispoRamassageAmbulances_, *pDispoRamassageAmbulances_, EquipmentResolver(), "%" );
}
