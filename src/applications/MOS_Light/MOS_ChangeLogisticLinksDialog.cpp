//****************************************************************************
//
// $Created:  NLD 2002-01-03 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_ChangeLogisticLinksDialog.cpp $
// $Author: Nld $
// $Modtime: 11/05/05 15:00 $
// $Revision: 5 $
// $Workfile: MOS_ChangeLogisticLinksDialog.cpp $
//
//*****************************************************************************

#include "MOS_Light_Pch.h"

#include "MOS_ChangeLogisticLinksDialog.h"
#include "moc_MOS_ChangeLogisticLinksDialog.cpp"

#include "MOS_App.h"
#include "MOS_ASN_Messages.h"
#include "MOS_Attr_Def.h"
#include "MOS_Agent.h"

// -----------------------------------------------------------------------------
// Name: MOS_ChangeLogisticLinksDialog constructor
// Created: NLD 2004-10-25
// -----------------------------------------------------------------------------
MOS_ChangeLogisticLinksDialog::MOS_ChangeLogisticLinksDialog( QWidget* pParent  )
    : QDialog( pParent, "Changer liens logistiques" )
    , pAgent_( 0 )
    , pTC2ComboBox_( 0 )
    , pMaintenanceComboBox_( 0 )
    , pMedicalComboBox_( 0 )
    , pSupplyComboBox_( 0 )
    , bInitialized_( false )
    , maintenanceComboBoxIDs_()
    , medicalComboBoxIDs_()
    , supplyComboBoxIDs_()
    , tc2ComboBoxIDs_()
{
    resize( 400, 150 );
    QVBoxLayout* pMainLayout = new QVBoxLayout( this );
    QGridLayout* pDataLayout = new QGridLayout( pMainLayout, 3, 2 );
    pDataLayout->setMargin ( 10 );
    pDataLayout->setSpacing( 10 );

    pDataLayout->addWidget( new QLabel( "TC2"                     , this ), 0, 0 );
    pDataLayout->addWidget( new QLabel( "Superieur maintenance"   , this ), 1, 0 );
    pDataLayout->addWidget( new QLabel( "Superieur santé"         , this ), 2, 0 );
    pDataLayout->addWidget( new QLabel( "Superieur ravitaillement", this ), 3, 0 );
    pTC2ComboBox_           = new QComboBox( FALSE, this );
    pMaintenanceComboBox_   = new QComboBox( FALSE, this );
    pMedicalComboBox_       = new QComboBox( FALSE, this );  
    pSupplyComboBox_        = new QComboBox( FALSE, this );
    pDataLayout->addWidget( pTC2ComboBox_        , 0, 1 );
    pDataLayout->addWidget( pMaintenanceComboBox_, 1, 1 );
    pDataLayout->addWidget( pMedicalComboBox_    , 2, 1 );
    pDataLayout->addWidget( pSupplyComboBox_     , 3, 1 );

    pTC2ComboBox_        ->setEnabled( false );
    pMaintenanceComboBox_->setEnabled( false );
    pMedicalComboBox_    ->setEnabled( false );
    pSupplyComboBox_     ->setEnabled( false );
   
    QHBoxLayout* pButtonLayout = new QHBoxLayout( pMainLayout );
    QPushButton* pCancelButton = new QPushButton( tr("Cancel"), this );
    QPushButton* pOKButton     = new QPushButton( tr("OK")    , this );
    pButtonLayout->addWidget( pCancelButton );
    pButtonLayout->addWidget( pOKButton     );
    pOKButton->setDefault( TRUE );

    connect( pCancelButton, SIGNAL( clicked() ), SLOT( Reject() ) );
    connect( pOKButton    , SIGNAL( clicked() ), SLOT( Validate() ) );    
}

// -----------------------------------------------------------------------------
// Name: MOS_ChangeLogisticLinksDialog destructor
// Created: NLD 2004-10-25
// -----------------------------------------------------------------------------
MOS_ChangeLogisticLinksDialog::~MOS_ChangeLogisticLinksDialog()
{

}

// -----------------------------------------------------------------------------
// Name: MOS_ChangeLogisticLinksDialog::SetAgent
// Created: NLD 2004-11-30
// -----------------------------------------------------------------------------
void MOS_ChangeLogisticLinksDialog::SetAgent( const MOS_Agent& agent )
{
    if( !bInitialized_ )
    {
        maintenanceComboBoxIDs_.clear();
        medicalComboBoxIDs_.clear();
        supplyComboBoxIDs_.clear();
        tc2ComboBoxIDs_.clear();

        pTC2ComboBox_        ->insertItem( "Aucun" );
        pMaintenanceComboBox_->insertItem( "Aucun" );
        pMedicalComboBox_    ->insertItem( "Aucun" );
        pSupplyComboBox_     ->insertItem( "Aucun" );

        MOS_AgentManager::CT_AgentMap& agents = MOS_App::GetApp().GetAgentManager().GetAgentList();
        for( MOS_AgentManager::CIT_AgentMap itAgent = agents.begin(); itAgent != agents.end(); ++itAgent )
        {
            const MOS_Agent& agent = *itAgent->second;
            if( !agent.IsAutomate() )
                continue;

            if( agent.IsMaintenancePionLog() || agent.IsMedicalPionLog() || agent.IsSupplyPionLog() )
            {
                tc2ComboBoxIDs_.insert( std::make_pair( pTC2ComboBox_->count(), &agent ) );
                pTC2ComboBox_->insertItem( agent.GetName().c_str() );
            }

            if( agent.IsMaintenancePionLog() )
            {
                maintenanceComboBoxIDs_.insert( std::make_pair( pMaintenanceComboBox_->count(), &agent ) );
                pMaintenanceComboBox_->insertItem( agent.GetName().c_str() );
            }

            if( agent.IsMedicalPionLog() )
            {
                medicalComboBoxIDs_.insert( std::make_pair( pMedicalComboBox_->count(), &agent ) );
                pMedicalComboBox_->insertItem( agent.GetName().c_str() );
            }

            if( agent.IsSupplyPionLog() )
            {
                supplyComboBoxIDs_.insert( std::make_pair( pSupplyComboBox_->count(), &agent ) );
                pSupplyComboBox_->insertItem( agent.GetName().c_str() );
            }
        }
        bInitialized_ = true;
    }


    assert( !pAgent_ );
    pAgent_ = &agent;

    pTC2ComboBox_        ->setEnabled( false );
    pMaintenanceComboBox_->setEnabled( false );
    pMedicalComboBox_    ->setEnabled( false );
    pSupplyComboBox_     ->setEnabled( false );

    if( !pAgent_->IsAutomate() )
        return;

    pTC2ComboBox_->setEnabled( true );
    if( pAgent_->IsMaintenancePionLog() )
        pMaintenanceComboBox_->setEnabled( true );
    if( pAgent_->IsMedicalPionLog() )
        pMedicalComboBox_->setEnabled( true );
    if( pAgent_->IsSupplyPionLog() )
        pSupplyComboBox_->setEnabled( true );
}

// -----------------------------------------------------------------------------
// Name: MOS_ChangeLogisticLinksDialog::Validate
// Created: NLD 2004-10-25
// -----------------------------------------------------------------------------
void MOS_ChangeLogisticLinksDialog::Validate()
{
    assert( pAgent_ );

    MOS_ASN_MsgChangeLiensLogistiques asnMsg;
    asnMsg.GetAsnMsg().oid_automate = pAgent_->GetAgentID();

    if( pTC2ComboBox_->isEnabled() )
    {
        asnMsg.GetAsnMsg().m.oid_tc2Present = 1;
        CIT_AgentIDMap it = tc2ComboBoxIDs_.find( pTC2ComboBox_->currentItem() );
        if( it == tc2ComboBoxIDs_.end() )
            asnMsg.GetAsnMsg().oid_tc2 = 0;
        else
            asnMsg.GetAsnMsg().oid_tc2 = it->second->GetAgentID();
    }

    if( pMaintenanceComboBox_->isEnabled() )
    {
        asnMsg.GetAsnMsg().m.oid_maintenancePresent = 1;
        CIT_AgentIDMap it = maintenanceComboBoxIDs_.find( pMaintenanceComboBox_->currentItem() );
        if( it == maintenanceComboBoxIDs_.end() )
            asnMsg.GetAsnMsg().oid_maintenance = 0;
        else
            asnMsg.GetAsnMsg().oid_maintenance = it->second->GetAgentID();

    }

    if( pMedicalComboBox_->isEnabled() )
    {
        asnMsg.GetAsnMsg().m.oid_santePresent = 1;
        CIT_AgentIDMap it = medicalComboBoxIDs_.find( pMedicalComboBox_->currentItem() );
        if( it == medicalComboBoxIDs_.end() )
            asnMsg.GetAsnMsg().oid_sante = 0;
        else
            asnMsg.GetAsnMsg().oid_sante = it->second->GetAgentID();

    }

    if( pSupplyComboBox_->isEnabled() )
    {
        asnMsg.GetAsnMsg().m.oid_ravitaillementPresent = 1;
        CIT_AgentIDMap it = supplyComboBoxIDs_.find( pSupplyComboBox_->currentItem() );
        if( it == supplyComboBoxIDs_.end() )
            asnMsg.GetAsnMsg().oid_ravitaillement = 0;
        else
            asnMsg.GetAsnMsg().oid_ravitaillement = it->second->GetAgentID();

    }
    
    asnMsg.Send( 57 );
    hide();
}

// -----------------------------------------------------------------------------
// Name: MOS_ChangeLogisticLinksDialog::Reject
// Created: NLD 2004-10-25
// -----------------------------------------------------------------------------
void MOS_ChangeLogisticLinksDialog::Reject()
{
    pAgent_ = 0;
    hide();
}

