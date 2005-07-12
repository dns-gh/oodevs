//****************************************************************************
//
// $Created:  NLD 2002-01-03 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_SurrenderDialog.cpp $
// $Author: Age $
// $Modtime: 6/04/05 15:32 $
// $Revision: 2 $
// $Workfile: MOS_SurrenderDialog.cpp $
//
//*****************************************************************************

#include "MOS_Light2_Pch.h"

#include "MOS_SurrenderDialog.h"
#include "moc_MOS_SurrenderDialog.cpp"

#include "MOS_App.h"
#include "MOS_ASN_Messages.h"
#include "MOS_Attr_Def.h"
#include "MOS_Agent.h"
#include "MOS_DynaObjectManager.h"
#include "MOS_DynaObject.h"

// -----------------------------------------------------------------------------
// Name: MOS_SurrenderDialog constructor
// Created: NLD 2004-10-25
// -----------------------------------------------------------------------------
MOS_SurrenderDialog::MOS_SurrenderDialog( QWidget* pParent  )
    : QDialog( pParent, "surrender" )
    , pAgent_( 0 )
    , pPrisonerCampComboBox_( 0 )
    , prisonerCampComboBoxIDs_()
    , bInitialized_( false )
{
    setCaption( tr( "Se Rendre" ) );
    resize( 400, 150 );
    QVBoxLayout* pMainLayout = new QVBoxLayout( this );
    QGridLayout* pDataLayout = new QGridLayout( pMainLayout, 3, 2 );
    pDataLayout->setMargin ( 10 );
    pDataLayout->setSpacing( 10 );

    pDataLayout->addWidget( new QLabel( tr( "Camp de prisonnier" ), this ), 0, 0 );
    pPrisonerCampComboBox_ = new QComboBox( FALSE, this );
    pDataLayout->addWidget( pPrisonerCampComboBox_, 0, 1 );

    pPrisonerCampComboBox_->setEnabled( false );
  
    QHBoxLayout* pButtonLayout = new QHBoxLayout( pMainLayout );
    QPushButton* pCancelButton = new QPushButton( tr("Annuler"), this );
    QPushButton* pOKButton     = new QPushButton( tr("OK")    , this );
    pButtonLayout->addWidget( pCancelButton );
    pButtonLayout->addWidget( pOKButton     );
    pOKButton->setDefault( TRUE );

    connect( pCancelButton, SIGNAL( clicked() ), SLOT( Reject() ) );
    connect( pOKButton    , SIGNAL( clicked() ), SLOT( Validate() ) );    
}

// -----------------------------------------------------------------------------
// Name: MOS_SurrenderDialog destructor
// Created: NLD 2004-10-25
// -----------------------------------------------------------------------------
MOS_SurrenderDialog::~MOS_SurrenderDialog()
{

}

// -----------------------------------------------------------------------------
// Name: MOS_SurrenderDialog::SetAgent
// Created: NLD 2004-11-30
// -----------------------------------------------------------------------------
void MOS_SurrenderDialog::SetAgent( const MOS_Agent& agent )
{
    prisonerCampComboBoxIDs_.clear();
    pPrisonerCampComboBox_->clear();

    const MOS_DynaObjectManager::T_DynaObjectMap& objects = MOS_App::GetApp().GetDynaObjectManager().GetDynaObjects();
    for( MOS_DynaObjectManager::CIT_DynaObjectMap itObject = objects.begin(); itObject != objects.end(); ++itObject )
    {
        const MOS_DynaObject& object = *itObject->second;
        
        if( object.GetType() != EnumObjectType::camp_prisonniers )
            continue;

        prisonerCampComboBoxIDs_.insert( std::make_pair( pPrisonerCampComboBox_->count(), &object ) );
        pPrisonerCampComboBox_->insertItem( QString( tr( "Objet " ) + "%1" ).arg( object.GetID() ) );
    }
    bInitialized_ = true;

    assert( !pAgent_ );
    pAgent_ = &agent;

    pPrisonerCampComboBox_->setEnabled( true );
}

// -----------------------------------------------------------------------------
// Name: MOS_SurrenderDialog::Validate
// Created: NLD 2004-10-25
// -----------------------------------------------------------------------------
void MOS_SurrenderDialog::Validate()
{
    assert( pAgent_ );

    MOS_ASN_MsgUnitMagicAction asnMsg;
    asnMsg.GetAsnMsg().oid                = pAgent_->GetAgentID();
    asnMsg.GetAsnMsg().action.t           = T_MsgUnitMagicAction_action_se_rendre;
    asnMsg.GetAsnMsg().action.u.se_rendre = prisonerCampComboBoxIDs_.find( pPrisonerCampComboBox_->currentItem() )->second->GetID();
    asnMsg.Send( 547 );

    hide();
}

// -----------------------------------------------------------------------------
// Name: MOS_SurrenderDialog::Reject
// Created: NLD 2004-10-25
// -----------------------------------------------------------------------------
void MOS_SurrenderDialog::Reject()
{
    pAgent_ = 0;
    hide();
}

