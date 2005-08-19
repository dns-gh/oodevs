//****************************************************************************
//
// $Created:  NLD 2002-01-03 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_ChangeLogisticLinksDialog.cpp $
// $Author: Nld $
// $Modtime: 8/07/05 15:55 $
// $Revision: 3 $
// $Workfile: MOS_ChangeLogisticLinksDialog.cpp $
//
//*****************************************************************************

#include "MOS_Light2_pch.h"

#include "MOS_ChangeLogisticLinksDialog.h"
#include "moc_MOS_ChangeLogisticLinksDialog.cpp"

#include "MOS_App.h"
#include "MOS_ASN_Messages.h"
#include "MOS_Attr_Def.h"
#include "MOS_Agent.h"
#include "MOS_AgentManager.h"
#include "MOS_TypeAutomate.h"

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

		std::string strii = agent.GetTypeAutomate()->GetType();
		if( strii == std::string("Automate LOG TC2")
			|| strii == std::string("Automate LOG BLD Maintenance")
			|| strii == std::string("Automate LOG BLT Maintenance")
			|| strii == std::string("Automate LOG BLD Sante")
			|| strii == std::string("Automate LOG BLT Sante")
			|| strii == std::string("Automate LOG BLD Ravitaillement")
			|| strii == std::string("Automate LOG BLT Ravitaillement"))
        //if( agent.pMaintenanceData_ || agent.pMedicalData_ || agent.pSupplyData_ )
        {
            tc2ComboBoxIDs_.insert( std::make_pair( pTC2ComboBox_->count(), &agent ) );
            pTC2ComboBox_->insertItem( agent.GetName().c_str() );
        }

		if( strii == std::string("Automate LOG BLD Maintenance")
			|| strii == std::string("Automate LOG BLT Maintenance"))
		//if( agent.pMaintenanceData_ )
        {
            maintenanceComboBoxIDs_.insert( std::make_pair( pMaintenanceComboBox_->count(), &agent ) );
            pMaintenanceComboBox_->insertItem( agent.GetName().c_str() );
        }

		if( strii == std::string("Automate LOG BLD Sante")
			|| strii == std::string("Automate LOG BLT Sante"))  
		//if( agent.pMedicalData_ )
        {
            medicalComboBoxIDs_.insert( std::make_pair( pMedicalComboBox_->count(), &agent ) );
            pMedicalComboBox_->insertItem( agent.GetName().c_str() );
        }

		if( strii == std::string("Automate LOG BLD Ravitaillement")
			|| strii == std::string("Automate LOG BLT Ravitaillement"))
        //if( agent.pSupplyData_ )
        {
            supplyComboBoxIDs_.insert( std::make_pair( pSupplyComboBox_->count(), &agent ) );
            pSupplyComboBox_->insertItem( agent.GetName().c_str() );
        }
    }
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
void MOS_ChangeLogisticLinksDialog::SetAgent( MOS_Agent& agent )
{
    pAgent_ = &agent;

    pTC2ComboBox_        ->setEnabled( false );
    pMaintenanceComboBox_->setEnabled( false );
    pMedicalComboBox_    ->setEnabled( false );
    pSupplyComboBox_     ->setEnabled( false );

    if( !pAgent_->IsAutomate() )
        return;
	
	std::string strii = pAgent_->GetTypeAutomate()->GetType();	
	if( strii != std::string("Automate LOG TC2")
		&& strii != std::string("Automate LOG BLD Maintenance")
		&& strii != std::string("Automate LOG BLT Maintenance")
		&& strii != std::string("Automate LOG BLD Sante")
		&& strii != std::string("Automate LOG BLT Sante")
		&& strii != std::string("Automate LOG BLD Ravitaillement")
		&& strii != std::string("Automate LOG BLT Ravitaillement"))
		pTC2ComboBox_->setEnabled( true );
	if( strii == std::string("Automate LOG TC2")
		|| strii == std::string("Automate LOG BLD Maintenance")
		|| strii == std::string("Automate LOG BLT Maintenance"))
        pMaintenanceComboBox_->setEnabled( true );
	if( strii == std::string("Automate LOG TC2")
		|| strii == std::string("Automate LOG BLD Sante")
		|| strii == std::string("Automate LOG BLT Sante"))
        pMedicalComboBox_->setEnabled( true );
	if( strii == std::string("Automate LOG TC2")
		|| strii == std::string("Automate LOG BLD Ravitaillement")
		|| strii == std::string("Automate LOG BLT Ravitaillement")
		|| strii == std::string("Automate LOG BLD Sante")
		|| strii == std::string("Automate LOG BLT Sante")
		|| strii == std::string("Automate LOG BLD Maintenance")
		|| strii == std::string("Automate LOG BLT Maintenance"))
        pSupplyComboBox_->setEnabled( true );

	pTC2ComboBox_->setCurrentItem( 0 );
	pMaintenanceComboBox_->setCurrentItem( 0 );
	pMedicalComboBox_->setCurrentItem( 0 );
	pSupplyComboBox_->setCurrentItem( 0 );

    ///$$$ TMP DEGUEU
    CIT_AgentIDMap it;
    for( it = tc2ComboBoxIDs_.begin(); it != tc2ComboBoxIDs_.end(); ++it )
    {    
        if( agent.nTC2_ && agent.nTC2_ == it->second->nAgentID_ )
            pTC2ComboBox_->setCurrentItem( it->first );
    }

    for( it = maintenanceComboBoxIDs_.begin(); it != maintenanceComboBoxIDs_.end(); ++it )
    {    
        if( agent.nLogMaintenanceSuperior_ && agent.nLogMaintenanceSuperior_ == it->second->nAgentID_ )
            pMaintenanceComboBox_->setCurrentItem( it->first );
    }

    for( it = medicalComboBoxIDs_.begin(); it != medicalComboBoxIDs_.end(); ++it )
    {    
        if( agent.nLogMedicalSuperior_ && agent.nLogMedicalSuperior_ == it->second->nAgentID_ )
            pMedicalComboBox_->setCurrentItem( it->first );
    }

    for( it = supplyComboBoxIDs_.begin(); it != supplyComboBoxIDs_.end(); ++it )
    {    
        if( agent.nLogSupplySuperior_ && agent.nLogSupplySuperior_ == it->second->nAgentID_ )
            pSupplyComboBox_->setCurrentItem( it->first );
    }
}

// -----------------------------------------------------------------------------
// Name: MOS_ChangeLogisticLinksDialog::Validate
// Created: NLD 2004-10-25
// -----------------------------------------------------------------------------
void MOS_ChangeLogisticLinksDialog::Validate()
{
    assert( pAgent_ );

    if( MOS_App::GetApp().IsODBEdition() )
	{
		if( pTC2ComboBox_->isEnabled() )
		{
			CIT_AgentIDMap it = tc2ComboBoxIDs_.find( pTC2ComboBox_->currentItem() );
			if( it != tc2ComboBoxIDs_.end() )
				pAgent_->nTC2_ = it->second->GetAgentID();
			else
				pAgent_->nTC2_ = (uint)0;
		}
	    if( pMaintenanceComboBox_->isEnabled() )
		{
			CIT_AgentIDMap it = maintenanceComboBoxIDs_.find( pMaintenanceComboBox_->currentItem() );
			if( it != maintenanceComboBoxIDs_.end() )
				pAgent_->nLogMaintenanceSuperior_ =  it->second->GetAgentID();
			else
				pAgent_->nLogMaintenanceSuperior_ = (uint)0;
		}
	    if( pMedicalComboBox_->isEnabled() )
		{
			CIT_AgentIDMap it = medicalComboBoxIDs_.find( pMedicalComboBox_->currentItem() );
			if( it != medicalComboBoxIDs_.end() )
				pAgent_->nLogMedicalSuperior_ =  it->second->GetAgentID();
			else
				pAgent_->nLogMedicalSuperior_ = (uint)0;
		}
	    if( pSupplyComboBox_->isEnabled() )
		{
			CIT_AgentIDMap it = supplyComboBoxIDs_.find( pSupplyComboBox_->currentItem() );
			if( it != supplyComboBoxIDs_.end() )
				pAgent_->nLogSupplySuperior_ =  it->second->GetAgentID();
			else
				pAgent_->nLogSupplySuperior_ = (uint)0;
		}
	}
	else
	{
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
	}
    pAgent_ = 0;
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

