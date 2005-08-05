//*****************************************************************************
// 
// $Created: AGN 03-05-23 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_AgentKnowledge_Editor.cpp $
// $Author: Nld $
// $Modtime: 10/03/05 16:27 $
// $Revision: 17 $
// $Workfile: MOS_AgentKnowledge_Editor.cpp $
// 
//*****************************************************************************

#include "MOS_Light_Pch.h"   

#include "MOS_AgentKnowledge_Editor.h"
#include "moc_MOS_AgentKnowledge_Editor.cpp"

#ifndef MOS_USE_INLINE
#   include "MOS_AgentKnowledge_Editor.inl"
#endif

#include "MOS_AgentKnowledge.h"
#include "MOS_PointListViewItem.h"
#include "MOS_EnemyInfo_Direction.h"
#include "MOS_EnemyInfo_Localisation.h"
#include "MOS_Value.h"
#include "MOS_App.h"
#include "MOS_MainWindow.h"
#include "MOS_AgentManager.h"
#include "MOS_Agent.h"
#include "MOS_Tools.h"
#include "MOS_Team.h"

//-----------------------------------------------------------------------------
// Name: MOS_AgentKnowledge_Editor constructor
// Created: AGN 03-05-23
//-----------------------------------------------------------------------------
MOS_AgentKnowledge_Editor::MOS_AgentKnowledge_Editor( QWidget* pParent )
    : QFrame( pParent, "Rens Eni" )
    , pAssociatedAgentLabel_( 0 )
    , pTeamLabel_( 0 )
    , pLevelLabel_( 0 )
    , pWeaponLabel_( 0 )
    , pCategoryLabel_( 0 )
    , pQualifierLabel_( 0 )
    , pSpecializationLabel_( 0 )
    , pMobilityLabel_( 0 )
    , pIsHQLabel_( 0 )
    , pPerceptionLevelLabel_( 0 )
    , pMaxPerceptionLevelLabel_( 0 )
    , pEtatOpsLabel_( 0 )
    , pDirectionLabel_( 0 )
    , pPositionLabel_( 0 )
    , pSpeedLabel_( 0 )
    , pRelevanceLabel_( 0 )
    , pMissionCapacityLabel_( 0 )
    , pPrisonerLabel_( 0 )
    , pSurrenderedLabel_( 0 )
    , pRefugeesManagedLabel_( 0 )
{
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentKnowledge_Editor destructor
// Created: AGN 03-05-23
//-----------------------------------------------------------------------------
MOS_AgentKnowledge_Editor::~MOS_AgentKnowledge_Editor()
{

}


//-----------------------------------------------------------------------------
// Name: MOS_AgentKnowledge_Editor::Initialize
// Created: AGN 03-05-23
//-----------------------------------------------------------------------------
void MOS_AgentKnowledge_Editor::Initialize()
{
    QGridLayout* pMainLayout = new QGridLayout( this, 10, 10 );
    pMainLayout->setMargin( 2 );
    pMainLayout->setSpacing( 10 );

    int nCurRow = 0;

    pMainLayout->addWidget( new QLabel( "ID", this ), nCurRow, 0 );
    pIDLabel_ = new QLabel( "-", this );
    pMainLayout->addWidget( pIDLabel_, nCurRow++, 1 );

    pMainLayout->addWidget( new QLabel( "Associated agent", this ), nCurRow, 0 );
    pAssociatedAgentLabel_ = new QLabel( "-", this );
    pMainLayout->addWidget( pAssociatedAgentLabel_, nCurRow++, 1 );

    pMainLayout->addWidget( new QLabel( "Team", this ), nCurRow, 0 );
    pTeamLabel_= new QLabel( "-", this );;
    pMainLayout->addWidget( pTeamLabel_, nCurRow++, 1 );

    pMainLayout->addWidget( new QLabel( "Niveau", this ), nCurRow, 0 );
    pLevelLabel_ = new QLabel( "-", this );
    pMainLayout->addWidget( pLevelLabel_, nCurRow++, 1 );

    pMainLayout->addWidget( new QLabel( "Arme", this ), nCurRow, 0 );
    pWeaponLabel_ = new QLabel( "-", this );
    pMainLayout->addWidget( pWeaponLabel_, nCurRow++, 1 );

    pMainLayout->addWidget( new QLabel( "Categorie", this ), nCurRow, 0 );
    pCategoryLabel_ = new QLabel( "-", this );
    pMainLayout->addWidget( pCategoryLabel_, nCurRow++, 1 );

    pMainLayout->addWidget( new QLabel( "Qualification", this ), nCurRow, 0 );
    pQualifierLabel_ = new QLabel( "-", this );
    pMainLayout->addWidget( pQualifierLabel_, nCurRow++, 1 );

    pMainLayout->addWidget( new QLabel( "Specialisation", this ), nCurRow, 0 );
    pSpecializationLabel_ = new QLabel( "-", this );
    pMainLayout->addWidget( pSpecializationLabel_, nCurRow++, 1 );

    pMainLayout->addWidget( new QLabel( "Mobilite", this ), nCurRow, 0 );
    pMobilityLabel_ = new QLabel( "-", this );
    pMainLayout->addWidget( pMobilityLabel_, nCurRow++, 1 );

    pMainLayout->addWidget( new QLabel( "Capacite Mission", this ), nCurRow, 0 );
    pMissionCapacityLabel_ = new QLabel( "-", this );
    pMainLayout->addWidget( pMissionCapacityLabel_, nCurRow++, 1 );

    pMainLayout->addWidget( new QLabel( "PC", this ), nCurRow, 0 );
    pIsHQLabel_ = new QLabel( "-", this );
    pMainLayout->addWidget( pIsHQLabel_, nCurRow++, 1 );

    pMainLayout->addWidget( new QLabel( "Rendu", this ), nCurRow, 0 );
    pSurrenderedLabel_ = new QLabel( "-", this );
    pMainLayout->addWidget( pSurrenderedLabel_, nCurRow++, 1 );

    pMainLayout->addWidget( new QLabel( "Prisonier", this ), nCurRow, 0 );
    pPrisonerLabel_ = new QLabel( "-", this );
    pMainLayout->addWidget( pPrisonerLabel_, nCurRow++, 1 );

    pMainLayout->addWidget( new QLabel( "Réfugiés pris en compte", this ), nCurRow, 0 );
    pRefugeesManagedLabel_ = new QLabel( "-", this );
    pMainLayout->addWidget( pRefugeesManagedLabel_, nCurRow++, 1 );

    pMainLayout->addWidget( new QLabel( "Niveau de perception courant", this ), nCurRow, 0 );
    pPerceptionLevelLabel_ = new QLabel( "-", this );
    pMainLayout->addWidget( pPerceptionLevelLabel_, nCurRow++, 1 );

    pMainLayout->addWidget( new QLabel( "Niveau de perception MAX", this ), nCurRow, 0 );
    pMaxPerceptionLevelLabel_ = new QLabel( "-", this );
    pMainLayout->addWidget( pMaxPerceptionLevelLabel_, nCurRow++, 1 );

    pMainLayout->addWidget( new QLabel( "Etat ops", this ), nCurRow, 0 );
    pEtatOpsLabel_ = new QLabel( "-", this );
    pMainLayout->addWidget( pEtatOpsLabel_, nCurRow++, 1 );

    pMainLayout->addWidget( new QLabel( "Direction", this ), nCurRow, 0 );
    pDirectionLabel_ = new QLabel( "-", this );
    pMainLayout->addWidget( pDirectionLabel_, nCurRow++, 1 );

    pMainLayout->addWidget( new QLabel( "Vitesse", this ), nCurRow, 0 );
    pSpeedLabel_ = new QLabel( "-", this );
    pMainLayout->addWidget( pSpeedLabel_, nCurRow++, 1 );

    pMainLayout->addWidget( new QLabel( "Position", this ), nCurRow, 0 );
    pPositionLabel_ = new QLabel( "-", this );
    pMainLayout->addWidget( pPositionLabel_, nCurRow++, 1 );

    pMainLayout->addWidget( new QLabel( "Pertinence", this ), nCurRow, 0 );
    pRelevanceLabel_ = new QLabel( "-", this );
    pMainLayout->addWidget( pRelevanceLabel_, nCurRow++, 1 );
 
    pAutomatePerceptionsListView_ = new QListView( this );
    pAutomatePerceptionsListView_ ->addColumn( "Automate" );
    pAutomatePerceptionsListView_ ->addColumn( "Level" );
    pMainLayout->addMultiCellWidget ( pAutomatePerceptionsListView_, nCurRow, nCurRow, 0, 1 );
    ++nCurRow;

    QPushButton* pOKButton = new QPushButton( tr("OK"), this );
    connect( pOKButton    , SIGNAL( clicked() ), SLOT( SlotValidate() ) );
    pMainLayout->addMultiCellWidget ( pOKButton, nCurRow, nCurRow, 0, 1 );
    pOKButton->setDefault( TRUE );
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentKnowledge_Editor::SetEnemyInfo
// Created: AGN 03-05-23
//-----------------------------------------------------------------------------
void MOS_AgentKnowledge_Editor::SetEnemyInfo( MOS_AgentKnowledge* pEditedInfo )
{
    assert( pEditedInfo != 0 );
    pEditedInfo_ = pEditedInfo;

    pEditedInfo_->SetEditor( this );

    Update();
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentKnowledge_Editor::Update
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void MOS_AgentKnowledge_Editor::Update()
{
    pAssociatedAgentLabel_->setText( MT_FormatString( "%d", pEditedInfo_->GetAgentID() ).c_str() );
    pIDLabel_             ->setText( MT_FormatString( "%d", pEditedInfo_->GetID() ).c_str() );

    pTeamLabel_                  ->setText( "-" );
    pLevelLabel_                 ->setText( "-" ); 
    pWeaponLabel_                ->setText( "-" ); 
    pCategoryLabel_              ->setText( "-" ); 
    pQualifierLabel_             ->setText( "-" ); 
    pSpecializationLabel_        ->setText( "-" ); 
    pMobilityLabel_              ->setText( "-" ); 
    pIsHQLabel_                  ->setText( "-" ); 
    pPerceptionLevelLabel_       ->setText( "-" ); 
    pMaxPerceptionLevelLabel_    ->setText( "-" ); 
    pEtatOpsLabel_               ->setText( "-" ); 
    pSpeedLabel_                 ->setText( "-" ); 
    pDirectionLabel_             ->setText( "-" ); 
    pPositionLabel_              ->setText( "-" );
    pMissionCapacityLabel_       ->setText( "-" );
    pAutomatePerceptionsListView_->clear();

    if( pEditedInfo_->IsValid( MOS_AgentKnowledge::eUpdated_Team ) )
        pTeamLabel_->setText(  MOS_App::GetApp().GetAgentManager().FindTeam( pEditedInfo_->nTeam_)->GetName().c_str() );

    if( pEditedInfo_->IsValid( MOS_AgentKnowledge::eUpdated_Level ) )
        pLevelLabel_->setText( ENT_Tr::ConvertFromNatureLevel( pEditedInfo_->nLevel_ ).c_str() );

    if( pEditedInfo_->IsValid( MOS_AgentKnowledge::eUpdated_Weapon ) )
        pWeaponLabel_->setText( ENT_Tr::ConvertFromUnitNatureWeapon( pEditedInfo_->nWeapon_ ).c_str() );

    if( pEditedInfo_->IsValid( MOS_AgentKnowledge::eUpdated_Category ) )
        pCategoryLabel_->setText( ENT_Tr::ConvertFromUnitNatureCategory( pEditedInfo_->nCategory_ ).c_str() );

    if( pEditedInfo_->IsValid( MOS_AgentKnowledge::eUpdated_Qualifier ) )
        pQualifierLabel_->setText( ENT_Tr::ConvertFromUnitNatureQualifier( pEditedInfo_->nQualifier_ ).c_str() );

    if( pEditedInfo_->IsValid( MOS_AgentKnowledge::eUpdated_Specialization ) )
        pSpecializationLabel_->setText( ENT_Tr::ConvertFromUnitNatureSpecialization( pEditedInfo_->nSpecialization_ ).c_str() );

    if( pEditedInfo_->IsValid( MOS_AgentKnowledge::eUpdated_Mobility ) )
        pMobilityLabel_->setText( ENT_Tr::ConvertFromUnitNatureMobility( pEditedInfo_->nMobility_ ).c_str() );

    if ( pEditedInfo_->IsValid( MOS_AgentKnowledge::eUpdated_MissionCapacity ) )
        pMissionCapacityLabel_->setText( ENT_Tr::ConvertFromUnitCapaciteMission( pEditedInfo_->nMissionCapacity_ ).c_str() );

    if( pEditedInfo_->IsValid( MOS_AgentKnowledge::eUpdated_IsPC ) )
        pIsHQLabel_->setText(  pEditedInfo_->bIsPC_ ? "True" : "False" );

    if( pEditedInfo_->IsValid( MOS_AgentKnowledge::eUpdated_Prisoner ) )
        pPrisonerLabel_->setText(  pEditedInfo_->bPrisoner_ ? "True" : "False" );

    if( pEditedInfo_->IsValid( MOS_AgentKnowledge::eUpdated_Surrendered ) )
        pSurrenderedLabel_->setText(  pEditedInfo_->bSurrendered_ ? "True" : "False" );

    if( pEditedInfo_->IsValid( MOS_AgentKnowledge::eUpdated_CurrentPerceptionLevel ) )
        pPerceptionLevelLabel_->setText(  MOS_Tools::ConvertPerceptionLevel( pEditedInfo_->nCurrentPerceptionLevel_ ).c_str() );

    if( pEditedInfo_->IsValid( MOS_AgentKnowledge::eUpdated_MaxPerceptionLevel ) )
        pMaxPerceptionLevelLabel_->setText(  MOS_Tools::ConvertPerceptionLevel( pEditedInfo_->nMaxPerceptionLevel_ ).c_str() );

    if( pEditedInfo_->IsValid( MOS_AgentKnowledge::eUpdated_EtatOps ) )
        pEtatOpsLabel_->setText( MT_FormatString( "%d%%", pEditedInfo_->nEtatOps_ ).c_str() );

    if( pEditedInfo_->IsValid( MOS_AgentKnowledge::eUpdated_Speed) )
        pSpeedLabel_->setText( MT_FormatString( "%d", pEditedInfo_->nSpeed_ ).c_str() );

    if( pEditedInfo_->IsValid( MOS_AgentKnowledge::eUpdated_Direction ) )
        pDirectionLabel_->setText( MT_FormatString( "%d°", pEditedInfo_->nDirection_ ).c_str() );

    if( pEditedInfo_->IsValid( MOS_AgentKnowledge::eUpdated_Position ) )
        pPositionLabel_->setText( pEditedInfo_->strPosition_.c_str() );

    if( pEditedInfo_->IsValid( MOS_AgentKnowledge::eUpdated_Relevance ) )
        pRelevanceLabel_->setText( MT_FormatString( "%d%%", pEditedInfo_->nRelevance_ ).c_str() );

    if( pEditedInfo_->IsValid( MOS_AgentKnowledge::eUpdated_AutomatePerception ) )
    {
        for( MOS_AgentKnowledge::CIT_AutomatePerceptionMap it = pEditedInfo_->automatePerceptionMap_.begin(); it != pEditedInfo_->automatePerceptionMap_.end(); ++it )
            new QListViewItem( pAutomatePerceptionsListView_, it->first->GetName().c_str(), MOS_Tools::ConvertPerceptionLevel( it->second ).c_str()  );
    }
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentKnowledge_Editor::SlotValidate
// Created: AGN 03-05-23
//-----------------------------------------------------------------------------
void MOS_AgentKnowledge_Editor::SlotValidate()
{
    assert( pEditedInfo_ != 0 );
    MOS_App::GetApp().GetMainWindow().PopWidgetStackRight();
    pEditedInfo_->SetEditor( 0 );
    pEditedInfo_ = 0;
}
    
