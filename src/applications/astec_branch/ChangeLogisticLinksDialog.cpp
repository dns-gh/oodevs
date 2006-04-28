//****************************************************************************
//
// $Created:  NLD 2002-01-03 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/ChangeLogisticLinksDialog.cpp $
// $Author: Nld $
// $Modtime: 8/07/05 15:55 $
// $Revision: 3 $
// $Workfile: ChangeLogisticLinksDialog.cpp $
//
//*****************************************************************************

#include "astec_pch.h"

#include "ChangeLogisticLinksDialog.h"
#include "moc_ChangeLogisticLinksDialog.cpp"

#include "App.h"
#include "ASN_Messages.h"
#include "Attr_Def.h"
#include "Agent.h"
#include "AgentManager.h"
#include "TypeAutomate.h"

// -----------------------------------------------------------------------------
// Name: ChangeLogisticLinksDialog constructor
// Created: NLD 2004-10-25
// -----------------------------------------------------------------------------
ChangeLogisticLinksDialog::ChangeLogisticLinksDialog( QWidget* pParent  )
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

    QLabel* lab_tc2 = new QLabel( "TC2"                     , this );
    lab_tc2->setPaletteBackgroundColor( QColor( "yellow" ) );
    pDataLayout->addWidget( lab_tc2 , 0, 0 );
    QLabel* lab_main = new QLabel( "Superieur maintenance"   , this );
    lab_main->setPaletteBackgroundColor( QColor( 128, 0, 0 ) );
    lab_main->setPaletteForegroundColor( QColor( "white" ) );
    pDataLayout->addWidget( lab_main, 1, 0 );
    QLabel* lab_san = new QLabel( "Superieur sant�"         , this );
    lab_san->setPaletteBackgroundColor( QColor( 255, 164, 200 ) );
    pDataLayout->addWidget( lab_san, 2, 0 );
    QLabel* lab_rav = new QLabel( "Superieur ravitaillement", this );
    lab_rav->setPaletteBackgroundColor( QColor( 255, 150, 10 ) );
    pDataLayout->addWidget( lab_rav , 3, 0 );
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

//    AgentManager::CT_AgentMap& agents = App::GetApp().GetAgentManager().GetAgentList();
//    for( AgentManager::CIT_AgentMap itAgent = agents.begin(); itAgent != agents.end(); ++itAgent )
//    {
//        const Agent& agent = *itAgent->second;
//        if( !agent.IsAutomate() )
//            continue;
//
//        if( agent.IsLogisticTC2() || agent.IsLogisticBLD() || agent.IsLogisticBLT() )
//        {
//            tc2ComboBoxIDs_.insert( std::make_pair( pTC2ComboBox_->count(), &agent ) );
//            pTC2ComboBox_->insertItem( agent.GetName().c_str() );
//        }
//        if( agent.IsLogisticTC2() || agent.IsLogisticMaintenance() )
//        {
//            maintenanceComboBoxIDs_.insert( std::make_pair( pMaintenanceComboBox_->count(), &agent ) );
//            pMaintenanceComboBox_->insertItem( agent.GetName().c_str() );
//        }
//        if( agent.IsLogisticTC2() || agent.IsLogisticSante() )  
//        {
//            medicalComboBoxIDs_.insert( std::make_pair( pMedicalComboBox_->count(), &agent ) );
//            pMedicalComboBox_->insertItem( agent.GetName().c_str() );
//        }
//        if( agent.IsLogisticTC2() || agent.IsLogisticRavitaillement() )
//        {
//            supplyComboBoxIDs_.insert( std::make_pair( pSupplyComboBox_->count(), &agent ) );
//            pSupplyComboBox_->insertItem( agent.GetName().c_str() );
//        }
//    }
}

// -----------------------------------------------------------------------------
// Name: ChangeLogisticLinksDialog destructor
// Created: NLD 2004-10-25
// -----------------------------------------------------------------------------
ChangeLogisticLinksDialog::~ChangeLogisticLinksDialog()
{

}

// -----------------------------------------------------------------------------
// Name: ChangeLogisticLinksDialog::SetAgent
// Created: NLD 2004-11-30
// -----------------------------------------------------------------------------
void ChangeLogisticLinksDialog::SetAgent( Agent& agent )
{
    pAgent_ = &agent;

    pTC2ComboBox_        ->setEnabled( false );
    pMaintenanceComboBox_->setEnabled( false );
    pMedicalComboBox_    ->setEnabled( false );
    pSupplyComboBox_     ->setEnabled( false );

    if( !pAgent_->IsAutomate() )
        return;
	
	std::string strii = pAgent_->GetTypeAutomate()->GetType();	

    //if( !agent.IsLogisticTC2() )  // $$$$ SBO 2005-10-28: TC2 parent can be something else than itself
    pTC2ComboBox_->setEnabled( true );
    if( agent.IsLogisticTC2() || agent.IsLogisticMaintenance() )
        pMaintenanceComboBox_->setEnabled( true );
    if( agent.IsLogisticTC2() || agent.IsLogisticSante() )
        pMedicalComboBox_->setEnabled( true );
    if( agent.IsLogisticTC2() || agent.IsLogisticRavitaillement() )
        pSupplyComboBox_->setEnabled( true );

	pTC2ComboBox_        ->setCurrentItem( 0 );
	pMaintenanceComboBox_->setCurrentItem( 0 );
	pMedicalComboBox_    ->setCurrentItem( 0 );
	pSupplyComboBox_     ->setCurrentItem( 0 );

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
// Name: ChangeLogisticLinksDialog::Validate
// Created: NLD 2004-10-25
// -----------------------------------------------------------------------------
void ChangeLogisticLinksDialog::Validate()
{
    assert( pAgent_ );

	std::string strii = pAgent_->GetTypeAutomate()->GetType();

    if( App::GetApp().IsODBEdition() )
    {
        if( pTC2ComboBox_->isEnabled() )
        {
            CIT_AgentIDMap it = tc2ComboBoxIDs_.find( pTC2ComboBox_->currentItem() );
            if( it != tc2ComboBoxIDs_.end() )
                pAgent_->nTC2_ = it->second->GetID();
            else
                pAgent_->nTC2_ = (uint)0;
        }
        if( pMaintenanceComboBox_->isEnabled() )
        {
            CIT_AgentIDMap it = maintenanceComboBoxIDs_.find( pMaintenanceComboBox_->currentItem() );
            if( it != maintenanceComboBoxIDs_.end() )
                pAgent_->nLogMaintenanceSuperior_ =  it->second->GetID();
            else
                pAgent_->nLogMaintenanceSuperior_ = (uint)0;
        }
        if( pMedicalComboBox_->isEnabled() )
        {
            CIT_AgentIDMap it = medicalComboBoxIDs_.find( pMedicalComboBox_->currentItem() );
            if( it != medicalComboBoxIDs_.end() )
                pAgent_->nLogMedicalSuperior_ =  it->second->GetID();
            else
                pAgent_->nLogMedicalSuperior_ = (uint)0;
        }
        if( pSupplyComboBox_->isEnabled() )
		{
			CIT_AgentIDMap it = supplyComboBoxIDs_.find( pSupplyComboBox_->currentItem() );
			if( it != supplyComboBoxIDs_.end() )
				pAgent_->nLogSupplySuperior_ =  it->second->GetID();
			else
				pAgent_->nLogSupplySuperior_ = (uint)0;
		}
	}
	else
	{
		ASN_MsgChangeLiensLogistiques asnMsg;
		asnMsg.GetAsnMsg().oid_automate = pAgent_->GetID();

		if( pTC2ComboBox_->isEnabled() )
		{
			asnMsg.GetAsnMsg().m.oid_tc2Present = 1;
			CIT_AgentIDMap it = tc2ComboBoxIDs_.find( pTC2ComboBox_->currentItem() );
			if( it == tc2ComboBoxIDs_.end() )
                asnMsg.GetAsnMsg().oid_tc2 = (uint)0;
            else
                asnMsg.GetAsnMsg().oid_tc2 = it->second->GetID();
		}

		if( pMaintenanceComboBox_->isEnabled() )
		{
			asnMsg.GetAsnMsg().m.oid_maintenancePresent = 1;
			CIT_AgentIDMap it = maintenanceComboBoxIDs_.find( pMaintenanceComboBox_->currentItem() );
			if( it == maintenanceComboBoxIDs_.end() )
				asnMsg.GetAsnMsg().oid_maintenance = 0;
			else
				asnMsg.GetAsnMsg().oid_maintenance = it->second->GetID();

		}

		if( pMedicalComboBox_->isEnabled() )
		{
			asnMsg.GetAsnMsg().m.oid_santePresent = 1;
			CIT_AgentIDMap it = medicalComboBoxIDs_.find( pMedicalComboBox_->currentItem() );
			if( it == medicalComboBoxIDs_.end() )
				asnMsg.GetAsnMsg().oid_sante = 0;
			else
				asnMsg.GetAsnMsg().oid_sante = it->second->GetID();

		}

		if( pSupplyComboBox_->isEnabled() )
		{
			asnMsg.GetAsnMsg().m.oid_ravitaillementPresent = 1;
			CIT_AgentIDMap it = supplyComboBoxIDs_.find( pSupplyComboBox_->currentItem() );
			if( it == supplyComboBoxIDs_.end() )
				asnMsg.GetAsnMsg().oid_ravitaillement = 0;
			else
				asnMsg.GetAsnMsg().oid_ravitaillement = it->second->GetID();

		}
		asnMsg.Send();
	}
    pAgent_ = 0;
    hide();
}

// -----------------------------------------------------------------------------
// Name: ChangeLogisticLinksDialog::Reject
// Created: NLD 2004-10-25
// -----------------------------------------------------------------------------
void ChangeLogisticLinksDialog::Reject()
{
    pAgent_ = 0;
    hide();
}

