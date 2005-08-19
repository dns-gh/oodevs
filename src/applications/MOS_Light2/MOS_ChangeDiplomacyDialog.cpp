//****************************************************************************
//
// $Created:  NLD 2002-01-03 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_ChangeDiplomacyDialog.cpp $
// $Author: Age $
// $Modtime: 6/04/05 15:32 $
// $Revision: 2 $
// $Workfile: MOS_ChangeDiplomacyDialog.cpp $
//
//*****************************************************************************

#include "MOS_Light2_pch.h"

#include "MOS_ChangeDiplomacyDialog.h"
#include "moc_MOS_ChangeDiplomacyDialog.cpp"

#include "MOS_App.h"
#include "MOS_AgentManager.h"
#include "MOS_ASN_Messages.h"
#include "MOS_Team.h"

// -----------------------------------------------------------------------------
// Name: MOS_ChangeDiplomacyDialog constructor
// Created: NLD 2004-10-25
// -----------------------------------------------------------------------------
MOS_ChangeDiplomacyDialog::MOS_ChangeDiplomacyDialog( QWidget* pParent  )
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
// Name: MOS_ChangeDiplomacyDialog destructor
// Created: NLD 2004-10-25
// -----------------------------------------------------------------------------
MOS_ChangeDiplomacyDialog::~MOS_ChangeDiplomacyDialog()
{

}

// -----------------------------------------------------------------------------
// Name: MOS_ChangeDiplomacyDialog::Initialize
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
void MOS_ChangeDiplomacyDialog::Initialize( MOS_Team& team )
{
    FillArmyComboBox( *pArmy1ComboBox_, &team );
    FillArmyComboBox( *pArmy2ComboBox_ );
}

// -----------------------------------------------------------------------------
// Name: MOS_ChangeDiplomacyDialog::FillArmyComboBox
// Created: NLD 2004-10-25
// -----------------------------------------------------------------------------
void MOS_ChangeDiplomacyDialog::FillArmyComboBox( QComboBox& comboBox, MOS_Team* pSelectedTeam )
{
    comboBox.clear();
    unsigned int nSelected = 0;
    const MOS_AgentManager::T_TeamMap& teams = MOS_App::GetApp().GetAgentManager().GetTeams();
    for( MOS_AgentManager::CIT_TeamMap itTeam = teams.begin(); itTeam != teams.end(); ++itTeam )
    {
        MOS_Team* pTeam = itTeam->second;
        if( pTeam == pSelectedTeam )
            nSelected = comboBox.count();
        comboBox.insertItem( pTeam->GetName().c_str() );
    }
    comboBox.setCurrentItem( nSelected );
}

// -----------------------------------------------------------------------------
// Name: MOS_ChangeDiplomacyDialog::Validate
// Created: NLD 2004-10-25
// -----------------------------------------------------------------------------
void MOS_ChangeDiplomacyDialog::Validate()
{
    MOS_ASN_MsgChangeDiplomatie asn;

    asn.GetAsnMsg().oid_camp1  = GetSelectedTeam( *pArmy1ComboBox_ ).GetID();
    asn.GetAsnMsg().oid_camp2  = GetSelectedTeam( *pArmy2ComboBox_ ).GetID();
    asn.GetAsnMsg().diplomatie = (ASN1T_EnumDiplomatie)pDiplomacyComboBox_->currentItem();
    asn.Send( 6897 );
    hide();
}

// -----------------------------------------------------------------------------
// Name: MOS_ChangeDiplomacyDialog::Reject
// Created: NLD 2004-10-25
// -----------------------------------------------------------------------------
void MOS_ChangeDiplomacyDialog::Reject()
{
    hide();
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentListView::MustBeDrawn
// Created: AGN 2003-10-08
// -----------------------------------------------------------------------------
const MOS_Team& MOS_ChangeDiplomacyDialog::GetSelectedTeam( const QComboBox& comboBox ) const
{

    return *MOS_App::GetApp().GetAgentManager().FindTeam( comboBox.currentText().ascii() );
}