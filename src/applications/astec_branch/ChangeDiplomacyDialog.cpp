//****************************************************************************
//
// $Created:  NLD 2002-01-03 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/ChangeDiplomacyDialog.cpp $
// $Author: Age $
// $Modtime: 6/04/05 15:32 $
// $Revision: 2 $
// $Workfile: ChangeDiplomacyDialog.cpp $
//
//*****************************************************************************

#include "astec_pch.h"

#include "ChangeDiplomacyDialog.h"
#include "moc_ChangeDiplomacyDialog.cpp"

#include "App.h"
#include "AgentManager.h"
#include "ASN_Messages.h"
#include "Team.h"
#include "Model.h"

// -----------------------------------------------------------------------------
// Name: ChangeDiplomacyDialog constructor
// Created: NLD 2004-10-25
// -----------------------------------------------------------------------------
ChangeDiplomacyDialog::ChangeDiplomacyDialog( QWidget* pParent  )
    : QDialog( pParent )
{
    setCaption( tr( "Diplomatie" ) );
    QVBoxLayout* pMainLayout = new QVBoxLayout( this );
    QHBoxLayout* pDataLayout = new QHBoxLayout( pMainLayout );
    pDataLayout->setMargin( 10 );
    pDataLayout->setSpacing( 10 );

    pDataLayout->addWidget( new QLabel( tr( "L'armée" ), this ) );

    pArmy1ComboBox_ = new QComboBox( FALSE, this );
    FillArmyComboBox( *pArmy1ComboBox_ );
    pDataLayout->addWidget( pArmy1ComboBox_ );

    pDataLayout->addWidget( new QLabel( tr( " est " ) , this ) );

    pDiplomacyComboBox_ = new QComboBox( FALSE, this );
    pDiplomacyComboBox_->insertItem( tr( "inconnue" ) );
    pDiplomacyComboBox_->insertItem( tr( "amie" ) );
    pDiplomacyComboBox_->insertItem( tr( "ennemie" ) );
    pDiplomacyComboBox_->insertItem( tr( "neutre" ) );
    pDataLayout->addWidget( pDiplomacyComboBox_ );

    pDataLayout->addWidget( new QLabel( tr( "avec l'armée" ), this ) );

    pArmy2ComboBox_ = new QComboBox( FALSE, this );
    FillArmyComboBox( *pArmy2ComboBox_ );
    pDataLayout->addWidget( pArmy2ComboBox_ );



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
// Name: ChangeDiplomacyDialog destructor
// Created: NLD 2004-10-25
// -----------------------------------------------------------------------------
ChangeDiplomacyDialog::~ChangeDiplomacyDialog()
{

}

// -----------------------------------------------------------------------------
// Name: ChangeDiplomacyDialog::Initialize
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
void ChangeDiplomacyDialog::Initialize( Team& team )
{
    FillArmyComboBox( *pArmy1ComboBox_, &team );
    FillArmyComboBox( *pArmy2ComboBox_ );
}

// -----------------------------------------------------------------------------
// Name: ChangeDiplomacyDialog::FillArmyComboBox
// Created: NLD 2004-10-25
// -----------------------------------------------------------------------------
void ChangeDiplomacyDialog::FillArmyComboBox( QComboBox& comboBox, Team* pSelectedTeam )
{
    comboBox.clear();
    unsigned int nSelected = 0;
    const AgentManager::T_TeamMap& teams = App::GetApp().GetAgentManager().GetTeams();
    for( AgentManager::CIT_TeamMap itTeam = teams.begin(); itTeam != teams.end(); ++itTeam )
    {
        Team* pTeam = itTeam->second;
        if( pTeam == pSelectedTeam )
            nSelected = comboBox.count();
        comboBox.insertItem( pTeam->GetName().c_str() );
    }
    comboBox.setCurrentItem( nSelected );
}

// -----------------------------------------------------------------------------
// Name: ChangeDiplomacyDialog::Validate
// Created: NLD 2004-10-25
// -----------------------------------------------------------------------------
void ChangeDiplomacyDialog::Validate()
{
    ASN_MsgChangeDiplomatie asn;

    asn.GetAsnMsg().oid_camp1  = GetSelectedTeam( *pArmy1ComboBox_ ).GetID();
    asn.GetAsnMsg().oid_camp2  = GetSelectedTeam( *pArmy2ComboBox_ ).GetID();
    asn.GetAsnMsg().diplomatie = (ASN1T_EnumDiplomatie)pDiplomacyComboBox_->currentItem();
    asn.Send( 6897 );
    hide();
}

// -----------------------------------------------------------------------------
// Name: ChangeDiplomacyDialog::Reject
// Created: NLD 2004-10-25
// -----------------------------------------------------------------------------
void ChangeDiplomacyDialog::Reject()
{
    hide();
}

// -----------------------------------------------------------------------------
// Name: AgentListView::MustBeDrawn
// Created: AGN 2003-10-08
// -----------------------------------------------------------------------------
const Team& ChangeDiplomacyDialog::GetSelectedTeam( const QComboBox& comboBox ) const
{
    // $$$$ AGE 2006-02-13: 
    return * App::GetApp().GetModel().FindTeam( comboBox.currentText().ascii() );
}