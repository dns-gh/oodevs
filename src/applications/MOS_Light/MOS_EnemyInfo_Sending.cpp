//*****************************************************************************
//
// $Created: AGN 03-05-21 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_EnemyInfo_Sending.cpp $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 12 $
// $Workfile: MOS_EnemyInfo_Sending.cpp $
//
//*****************************************************************************

#include "MOS_Light_Pch.h"

#include "MOS_EnemyInfo_Sending.h"
#include "moc_MOS_EnemyInfo_Sending.cpp"

#ifndef MOS_USE_INLINE
#	include "MOS_EnemyInfo_Sending.inl"
#endif

#include "MOS_AgentListView.h"
#include "MOS_App.h"
#include "MOS_MainWindow.h"
#include "MOS_EnemyInfo.h"
#include "MOS_ASN_Messages.h"
#include "MOS_World.h"
#include "MOS_Tools.h"

//-----------------------------------------------------------------------------
// Name: MOS_EnemyInfo_Sending constructor
// Created: AGN 03-05-21
//-----------------------------------------------------------------------------
MOS_EnemyInfo_Sending::MOS_EnemyInfo_Sending( QWidget* pParent )
: QFrame( pParent, "Envoie Rens à" )
, pAgentView_( 0 )
, pEditedInfo_( 0 )
{
    Initialize();
}


//-----------------------------------------------------------------------------
// Name: MOS_EnemyInfo_Sending destructor
// Created: AGN 03-05-21
//-----------------------------------------------------------------------------
MOS_EnemyInfo_Sending::~MOS_EnemyInfo_Sending()
{
    Terminate();
}



//-----------------------------------------------------------------------------
// Name: MOS_EnemyInfo_Sending::Initialize
// Created: AGN 03-05-21
//-----------------------------------------------------------------------------
void MOS_EnemyInfo_Sending::Initialize()
{
    // pMain layout
    QVBoxLayout* pMainLayout = new QVBoxLayout( this );
    pMainLayout->setMargin( 2 );
    pMainLayout->setSpacing( 10 );

    // Agent list
    pAgentView_ = new MOS_AgentListView( this, eListViewMission );
    pAgentView_->setMargin( 5 );
    pAgentView_->setLineWidth( 2 );
    pAgentView_->setFrameStyle( QFrame::Sunken | QFrame::Box );
    pMainLayout->addWidget( pAgentView_ );
    pAgentView_->Initialize();
    

    // Second layout
    QHBoxLayout* pSecondLayout = new QHBoxLayout( pMainLayout );

    //--------------------
    // OK / Cancel
    QPushButton* pCancelButton = new QPushButton( tr("Cancel"), this );
    pSecondLayout->addWidget( pCancelButton );
    QPushButton* pOKButton = new QPushButton( tr("OK"), this );
    pSecondLayout->addWidget( pOKButton );
    pOKButton->setDefault( TRUE );
    
    connect( pCancelButton, SIGNAL( clicked() ), SLOT( SlotCancel() ) );
    connect( pOKButton    , SIGNAL( clicked() ), SLOT( SlotValidate() ) );    
}


// -----------------------------------------------------------------------------
// Name: MOS_EnemyInfo_Sending::Terminate
// Created: NLD 2003-10-09
// -----------------------------------------------------------------------------
void MOS_EnemyInfo_Sending::Terminate()
{
    if( pAgentView_ )
    {
        pAgentView_->Terminate();
        delete pAgentView_;
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_EnemyInfo_Sending::SlotValidate
// Created: AGN 03-05-21
//-----------------------------------------------------------------------------
void MOS_EnemyInfo_Sending::SlotValidate()
{
    // Send info to the selected automata
    /*assert( pEditedInfo_ != 0 );

    QListViewItem* pItem = pAgentView_->firstChild();
    if( pItem == 0 )
        return;

    // Creating the message
    MOS_ASN_MsgRensEnnemiCreation asnMsg;

    asnMsg.GetAsnMsg().oid_renseignement = pEditedInfo_->nId_;
    asnMsg.GetAsnMsg().niveau_de_confirmation = (ASN1T_EnumNiveauConfirmation)pEditedInfo_->nConfirmationLevel_;
    asnMsg.GetAsnMsg().niveau_hierarchique = (ASN1T_EnumSymboleNiveauLimite)pEditedInfo_->nHierarchicalLevel_;

    if( pEditedInfo_->nAttitude_ != MOS_EnemyInfo::eAttitude_NotDefined )
    {
        asnMsg.GetAsnMsg().m.attitudePresent = 1;
        asnMsg.GetAsnMsg().attitude = (ASN1T_EnumRensUnitAttitude)pEditedInfo_->nAttitude_;
    }
    asnMsg.GetAsnMsg().m.vitessePresent = 1;
    asnMsg.GetAsnMsg().vitesse = pEditedInfo_->nSpeed_;
    asnMsg.GetAsnMsg().m.directionPresent = 1;

    MT_Vector2D vNorth( 0, 1 );
    MT_Vector2D vDir = pEditedInfo_->endDir_ - pEditedInfo_->startDir_;
    vDir.Normalize();

    MT_Float rDotProduct = DotProduct( vNorth, vDir );
    MT_Float rAngle = rDotProduct / ( vNorth.Magnitude() * vDir.Magnitude() );
    rAngle = acos( rAngle ) * 180 / MT_PI;
    if( vDir.rX_ < 0 )
        rAngle = 360 - rAngle;

    asnMsg.GetAsnMsg().direction = rAngle;

    asnMsg.GetAsnMsg().m.campPresent = 1;
    asnMsg.GetAsnMsg().camp = MOS_App::GetApp().GetAgentManager().ConvertTeamSimToMos( pEditedInfo_->nCamp_ );

    asnMsg.GetAsnMsg().m.nature_armePresent = 1;
    asnMsg.GetAsnMsg().nature_arme = (ASN1T_EnumUnitNatureWeapon)pEditedInfo_->nWeapon_;
    asnMsg.GetAsnMsg().m.nature_categoriePresent = 1;
    asnMsg.GetAsnMsg().nature_categorie = (ASN1T_EnumUnitNatureCategory)pEditedInfo_->nCategory_;
    asnMsg.GetAsnMsg().m.nature_mobilitePresent = 1;
    asnMsg.GetAsnMsg().nature_mobilite = (ASN1T_EnumUnitNatureMobility)pEditedInfo_->nMobility_;
    asnMsg.GetAsnMsg().m.nature_qualificatifPresent = 1;
    asnMsg.GetAsnMsg().nature_qualificatif = (ASN1T_EnumUnitNatureQualifier)pEditedInfo_->nQualifier_;
    asnMsg.GetAsnMsg().m.nature_specialisationPresent = 1;
    asnMsg.GetAsnMsg().nature_specialisation = (ASN1T_EnumUnitNatureSpecialization)pEditedInfo_->nSpecialization_;

    // Localisation
    if( pEditedInfo_->pointList_.size() == 1 )
        asnMsg.GetAsnMsg().localisation.type = EnumTypeLocalisation::Root::pt;
    else
    if( pEditedInfo_->pointList_.size() == 2 )
        asnMsg.GetAsnMsg().localisation.type = EnumTypeLocalisation::Root::li32;
    else
        asnMsg.GetAsnMsg().localisation.type = EnumTypeLocalisation::Root::po32;

    asnMsg.GetAsnMsg().localisation.vecteur_point.n    = pEditedInfo_->pointList_.size();
    asnMsg.GetAsnMsg().localisation.vecteur_point.elem = new ASN1T_CoordUTM[ pEditedInfo_->pointList_.size() ];
    for( IT_PointVector it = pEditedInfo_->pointList_.begin(); it != pEditedInfo_->pointList_.end(); ++it )
    {
        std::string strMGRS;
        const MT_Vector2D& vPos = *it;
        MOS_App::GetApp().GetWorld().SimToMosMgrsCoord( vPos, strMGRS );
        
        asnMsg.GetAsnMsg().localisation.vecteur_point.elem[it - pEditedInfo_->pointList_.begin()] = strMGRS.c_str();
    }

    std::string sParam;
    pAgentView_->WriteMsg( asnMsg.GetAsnMsg().oid_automates, sParam );

    while( pItem != 0 )
    {
        MOS_AgentListViewItem_ABC* pMyItem = static_cast< MOS_AgentListViewItem_ABC* >( pItem );
        assert( pMyItem != 0 );
        pEditedInfo_->AddReferecingAutomata( pMyItem->GetAgent() );
        pItem = pItem->firstChild();
    }

    asnMsg.Send( (MIL_MOSContextID)this );
    delete[] asnMsg.GetAsnMsg().localisation.vecteur_point.elem;
    delete[] asnMsg.GetAsnMsg().oid_automates.elem;
    
    
    MOS_App::GetApp().GetMainWindow().PopWidgetStackLeft();    */
}


//-----------------------------------------------------------------------------
// Name: MOS_EnemyInfo_Sending::SlotCancel
// Created: AGN 03-05-21
//-----------------------------------------------------------------------------
void MOS_EnemyInfo_Sending::SlotCancel()
{
    MOS_App::GetApp().GetMainWindow().PopWidgetStackLeft();    
}

