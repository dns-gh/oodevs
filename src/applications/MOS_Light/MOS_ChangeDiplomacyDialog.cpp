//****************************************************************************
//
// $Created:  NLD 2002-01-03 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_ChangeDiplomacyDialog.cpp $
// $Author: Nld $
// $Modtime: 14/02/05 15:57 $
// $Revision: 3 $
// $Workfile: MOS_ChangeDiplomacyDialog.cpp $
//
//*****************************************************************************

#include "MOS_Light_Pch.h"

#include "MOS_ChangeDiplomacyDialog.h"
#include "moc_MOS_ChangeDiplomacyDialog.cpp"

#include "MOS_App.h"
#include "MOS_AgentManager.h"
#include "MOS_ASN_Messages.h"
#include "MOS_Team.h"

// -----------------------------------------------------------------------------
// Name: MOS_ChangeDiplomacyDialog::FillArmyComboBox
// Created: NLD 2004-10-25
// -----------------------------------------------------------------------------
void MOS_ChangeDiplomacyDialog::FillArmyComboBox( QComboBox& comboBox )
{
    comboBox.clear();
    const MOS_AgentManager::T_TeamIDVector& teams = MOS_App::GetApp().GetAgentManager().GetTeams();
    for( MOS_AgentManager::CIT_TeamIDVector itTeam = teams.begin(); itTeam != teams.end(); ++itTeam )
        comboBox.insertItem( (*itTeam)->GetName().c_str() );
}

// -----------------------------------------------------------------------------
// Name: MOS_ChangeDiplomacyDialog constructor
// Created: NLD 2004-10-25
// -----------------------------------------------------------------------------
MOS_ChangeDiplomacyDialog::MOS_ChangeDiplomacyDialog( QWidget* pParent  )
    : QDialog( pParent )
{
    QVBoxLayout* pMainLayout = new QVBoxLayout( this );
    QHBoxLayout* pDataLayout = new QHBoxLayout( pMainLayout );
    pDataLayout->setMargin( 10 );
    pDataLayout->setSpacing( 10 );

    pDataLayout->addWidget( new QLabel( "Army", this ) );

    pArmy1ComboBox_ = new QComboBox( FALSE, this );
    FillArmyComboBox( *pArmy1ComboBox_ );
    pDataLayout->addWidget( pArmy1ComboBox_ );

    pDataLayout->addWidget( new QLabel( "is" , this ) );

    pDiplomacyComboBox_ = new QComboBox( FALSE, this );
    pDiplomacyComboBox_->insertItem( "unknown" );
    pDiplomacyComboBox_->insertItem( "friend" );
    pDiplomacyComboBox_->insertItem( "enemy" );
    pDiplomacyComboBox_->insertItem( "neutral" );
    pDataLayout->addWidget( pDiplomacyComboBox_ );

    pDataLayout->addWidget( new QLabel( "with army", this ) );

    pArmy2ComboBox_ = new QComboBox( FALSE, this );
    FillArmyComboBox( *pArmy2ComboBox_ );
    pDataLayout->addWidget( pArmy2ComboBox_ );



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
void MOS_ChangeDiplomacyDialog::Initialize()
{
    FillArmyComboBox( *pArmy1ComboBox_ );
    FillArmyComboBox( *pArmy2ComboBox_ );
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
    return *MOS_App::GetApp().GetAgentManager().GetTeams()[ comboBox.currentItem() ];
}