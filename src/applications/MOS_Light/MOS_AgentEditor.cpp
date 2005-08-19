//*****************************************************************************
//
// $Created: FBD 02-12-17 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_AgentEditor.cpp $
// $Author: Nld $
// $Modtime: 15/03/05 18:58 $
// $Revision: 40 $
// $Workfile: MOS_AgentEditor.cpp $
//
//*****************************************************************************

#include "MOS_Light_pch.h"

#include "MOS_AgentEditor.h"
#include "moc_MOS_AgentEditor.cpp"

#ifndef MOS_USE_INLINE
#	include "MOS_AgentEditor.inl"
#endif


#include <QButtonGroup.h>
#include <QWidget.h>
#include <QCheckBox.h>
#include <QWidgetStack.h>
#include <QPushButton.h>
#include <QTabWidget.h>
#include <QHeader.h>
#include <QLineEdit.h>

#include "MOS_App.h"
#include "MOS_World.h"
#include "MOS_WorldWidget.h"
#include "MOS_Mainwindow.h"
#include "MOS_AgentListView.h"
#include "MOS_ODBListView.h"
#include "MOS_EnemyInfo_List.h"
#include "MOS_DynaObjectManager.h"
#include "MOS_Meteo_Manager.h"
#include "MOS_Value.h"
#include "MOS_ASN_Messages.h"
#include "MOS_Team.h"
#include "MOS_ChangeDiplomacyDialog.h"

//$$$ C'est le bordel la dedans ...

//-----------------------------------------------------------------------------
// Name: MOS_AgentEditor constructor
// Created: FBD 02-12-17
// Last modified: JVT 03-04-08
//-----------------------------------------------------------------------------
MOS_AgentEditor::MOS_AgentEditor( QWidget* pParent )
    : QWidget( pParent )
    , pRensEni_( 0 )
    , pDrawTeamList_( 0 )
    , pAgentListView_( 0 )
    , pChangeDiplomacyDialog_( 0 )
{


}

//-----------------------------------------------------------------------------
// Name: MOS_AgentEditor destructor
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
MOS_AgentEditor::~MOS_AgentEditor()
{

}

//-----------------------------------------------------------------------------
// Name: MOS_AgentEditor::Initialize
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_AgentEditor::Initialize()
{
    // Main Layout
    QVBoxLayout* pLayout = new QVBoxLayout( this );

    pTabWidget_ = new QTabWidget( this );
    pLayout->addWidget( pTabWidget_ );

    // Title

    // Agent list
    pAgentListView_ = new MOS_AgentListView( this, eListViewEditor );
    pAgentListView_->setMargin( 5 );
    pAgentListView_->setLineWidth( 2 );
    pAgentListView_->setFrameStyle( QFrame::Sunken | QFrame::Box );
    pTabWidget_->addTab( pAgentListView_, "Agents" );

    // Object list
    QListView* pObjectListView = MOS_App::GetApp().GetDynaObjectManager().CreateListView( this );
    pObjectListView->setMargin( 5 );
    pObjectListView->setLineWidth( 2 );
    pObjectListView->setFrameStyle( QFrame::Sunken | QFrame::Box );
    pTabWidget_->addTab( pObjectListView, "Objets" );

    // Meteo
    QWidget* pWeatherGUI = MOS_App::GetApp().GetWeatherManager().CreateWeatherGUI( this );
    pTabWidget_->addTab( pWeatherGUI, "Météo" );

    // Button
    QPushButton* pTestButton = new QPushButton( "Test all missions", this );
    pLayout->addWidget( pTestButton );
    connect( pTestButton, SIGNAL( clicked() ),pAgentListView_, SLOT( SlotTestMission() ) );

    pSettingsWidget_ = new QWidget( this );
    pTabWidget_->addTab( pSettingsWidget_, "Settings" );
    QVBoxLayout* pSettingsLayout = new QVBoxLayout( pSettingsWidget_ );
    pSettingsLayout->setAlignment( Qt::AlignTop );

    // CheckBoxs
    QGridLayout* pCheckBoxLayout = new QGridLayout( pSettingsLayout );
    pCheckBoxLayout->setAlignment( Qt::AlignTop );
//    pCheckBoxLayout->setMargin( 5 );

    int nWidgetIndex = 0;

    QCheckBox* pCheckBoxItineraire = new QCheckBox( "View Itineraire", pSettingsWidget_ );
    pCheckBoxItineraire->setChecked( true );
    pCheckBoxLayout->addWidget( pCheckBoxItineraire, nWidgetIndex++, 0 );
    connect( pCheckBoxItineraire, SIGNAL( toggled( bool ) ), pAgentListView_, SLOT( SlotSetItineraire( bool ) ) );

    QCheckBox* pCheckBoxAllItineraire = new QCheckBox( "All Itineraire", pSettingsWidget_ );
    pCheckBoxAllItineraire->setChecked( false );
    pCheckBoxLayout->addWidget( pCheckBoxAllItineraire, nWidgetIndex++, 0  );
    connect( pCheckBoxAllItineraire, SIGNAL( toggled( bool ) ), pAgentListView_, SLOT( SlotSetAllItineraire( bool ) ) );

    QCheckBox* pCheckBoxOldPath = new QCheckBox( "Chemin parcouru", pSettingsWidget_ );
    pCheckBoxOldPath->setChecked( false );
    pCheckBoxLayout->addWidget( pCheckBoxOldPath, nWidgetIndex++, 0 );
    connect( pCheckBoxOldPath, SIGNAL( toggled( bool ) ), pAgentListView_, SLOT( SlotSetDrawOldPath( bool ) ) );

    QCheckBox* pCheckBoxFollow = new QCheckBox( "Follow Agent", pSettingsWidget_ );
    pCheckBoxFollow->setChecked( false );
    pCheckBoxLayout->addWidget( pCheckBoxFollow, nWidgetIndex++, 0 );
    connect( pCheckBoxFollow, SIGNAL( toggled( bool ) ), pAgentListView_, SLOT( SlotSetFollow( bool ) ) );

    QCheckBox* pCheckBoxVision = new QCheckBox( "Vision Agent", pSettingsWidget_ );
    pCheckBoxVision->setChecked( true );
    pCheckBoxLayout->addWidget( pCheckBoxVision, nWidgetIndex++, 0 );
    connect( pCheckBoxVision, SIGNAL( toggled( bool ) ), pAgentListView_, SLOT( SlotSetAllVision( bool ) ) );
    
    pCheckBoxProfiling_ = new QCheckBox( "Profiling activé", pSettingsWidget_ );
    pCheckBoxProfiling_->setChecked( false );
    pCheckBoxLayout->addWidget( pCheckBoxProfiling_, nWidgetIndex++, 0 );
    connect( pCheckBoxProfiling_, SIGNAL( toggled( bool ) ), pAgentListView_, SLOT( SlotEnableProfiling( bool ) ) );

    pCheckBoxCone_ = new QCheckBox( "Cône Agent", pSettingsWidget_ );
    pCheckBoxCone_->setChecked( false );
    pCheckBoxLayout->addWidget( pCheckBoxCone_, nWidgetIndex++, 0 );
    connect( pCheckBoxCone_, SIGNAL( toggled( bool ) ), pAgentListView_, SLOT( SlotSetAllCone( bool ) ) );


    QCheckBox* pCheckBoxDrawAltitude = new QCheckBox( "Altitudes", pSettingsWidget_ );
    pCheckBoxDrawAltitude->setChecked( true );
    pCheckBoxLayout->addWidget( pCheckBoxDrawAltitude, nWidgetIndex++, 0 );
    connect( pCheckBoxDrawAltitude, SIGNAL( toggled( bool ) ), pAgentListView_, SLOT( SlotSetDrawAltitude( bool ) ) );

    QCheckBox* pCheckBoxDrawLima = new QCheckBox( "Limas", pSettingsWidget_ );
    pCheckBoxDrawLima->setChecked( false );
    pCheckBoxLayout->addWidget( pCheckBoxDrawLima, nWidgetIndex++, 0 );
    connect( pCheckBoxDrawLima, SIGNAL( toggled( bool ) ), pAgentListView_, SLOT( SlotSetDrawLima( bool ) ) );

    QCheckBox* pCheckBoxDrawLimit = new QCheckBox( "Limits", pSettingsWidget_ );
    pCheckBoxDrawLimit->setChecked( false );
    pCheckBoxLayout->addWidget( pCheckBoxDrawLimit, nWidgetIndex++, 0 );
    connect( pCheckBoxDrawLimit, SIGNAL( toggled( bool ) ), pAgentListView_, SLOT( SlotSetDrawLimit( bool ) ) );

    QCheckBox* pCheckBoxDrawText = new QCheckBox( "Texte", pSettingsWidget_ );
    pCheckBoxDrawText->setChecked( false );
    pCheckBoxLayout->addWidget( pCheckBoxDrawText, nWidgetIndex++, 0 );
    connect( pCheckBoxDrawText, SIGNAL( toggled( bool ) ), pAgentListView_, SLOT( SlotSetDrawText( bool ) ) );

    QCheckBox* pCheckBoxScrollTrace = new QCheckBox( "Scroll trace", pSettingsWidget_ );
    pCheckBoxScrollTrace->setChecked( true );
    pCheckBoxLayout->addWidget( pCheckBoxScrollTrace, nWidgetIndex++, 0 );
    connect( pCheckBoxScrollTrace, SIGNAL( toggled( bool ) ), pAgentListView_, SLOT( SlotSetScrollTrace( bool ) ) );

    QCheckBox* pCheckBoxPointEdition = new QCheckBox( "Save points", pSettingsWidget_ );
    pCheckBoxPointEdition->setChecked( false );
    pCheckBoxLayout->addWidget( pCheckBoxPointEdition, nWidgetIndex++, 0 );
    connect( pCheckBoxPointEdition, SIGNAL( toggled( bool ) ), pAgentListView_, SLOT( SlotSetPointEdition( bool ) ) );

    // ODB
    pODBWidget_ = new QWidget( this );
    pTabWidget_->addTab( pODBWidget_, "ODB" );
    QVBoxLayout* pODBLayout = new QVBoxLayout( pODBWidget_ );
    pOdbListView_ = new MOS_ODBListView( pODBWidget_ );
    pODBLayout->addWidget( pOdbListView_ );

    QPushButton* pODBWrite = new QPushButton( "Write", pODBWidget_ );
    pODBLayout->addWidget( pODBWrite );
    connect( pODBWrite, SIGNAL( clicked() ), pOdbListView_, SLOT( SlotWriteODB() ) );

    // Rens Eni
    pInfoEnemyWidget_ = new QWidget( this );
    pTabWidget_->addTab( pInfoEnemyWidget_, "Rens. Eni" );
    QVBoxLayout* pInfoLayout = new QVBoxLayout( pInfoEnemyWidget_ );
    pRensEni_ = new MOS_EnemyInfo_List( pInfoEnemyWidget_ );
    pInfoLayout->addWidget( pRensEni_ );
    pRensEni_->header()->hide();

    QCheckBox* pDrawRensEni = new QCheckBox( "Affiche Rens Eni", pSettingsWidget_ );
    pDrawRensEni->setChecked( false );
    pCheckBoxLayout->addWidget( pDrawRensEni, nWidgetIndex++, 0 );
    connect( pDrawRensEni, SIGNAL( toggled( bool ) ), pRensEni_, SLOT( SlotDrawRensEni( bool ) ) );

    // Checkpoints frequency
    pCheckPointFrequency_ = new MOS_Value( eTypeValueInt, pSettingsWidget_, "Checkpoint frequency", MOSToolValueModif | MOSToolValueSpeedo | MOSToolValueTitle | MOSToolValueText );
    pCheckPointFrequency_ ->SetValueMinWidget( 0 );
    pCheckPointFrequency_ ->SetValueWidget   ( 10 );
    pCheckPointFrequency_ ->SetValueMaxWidget( 120 );
    pCheckPointFrequency_->setMaximumWidth( 100 );
    pSettingsLayout->addWidget( pCheckPointFrequency_ );
    connect( pCheckPointFrequency_->GetSpeedo(), SIGNAL( dialReleased() ), this, SLOT( SlotChangeCheckPointFrequency() ) );

    // Checkpoint Save NOW
    QHBoxLayout* pCheckPointLayout = new QHBoxLayout( pSettingsLayout ); 
    pButtonCheckPointSaveNow_ = new QPushButton( "Checkpoint save NOW", pSettingsWidget_ );
    pCheckPointName_          = new QLineEdit  ( pSettingsWidget_ );    
    pCheckPointLayout->addWidget( pCheckPointName_  );
    pCheckPointLayout->addWidget( pButtonCheckPointSaveNow_ );
    connect( pButtonCheckPointSaveNow_, SIGNAL( clicked() ), this, SLOT( SlotCheckPointSaveNow() ) );

    // Draw Teams
    pDrawTeamList_ = new QListView( pSettingsWidget_ );
    pSettingsLayout->addWidget( pDrawTeamList_ );
    pDrawTeamList_->addColumn( "Affichage des camps" );

    const MOS_AgentManager::T_TeamIDVector& teams = MOS_App::GetApp().GetAgentManager().GetTeams();
    for( MOS_AgentManager::CIT_TeamIDVector itTeam = teams.begin(); itTeam != teams.end(); ++itTeam )
    {
        QCheckListItem* pItem = new QCheckListItem( pDrawTeamList_, (**itTeam).GetName().c_str(), QCheckListItem::CheckBox );
        pItem->setOn( true );
    }

    // Change diplomacy
    pButtonCheckPointSaveNow_ = new QPushButton( "Change diplomacy", pSettingsWidget_ );
    pSettingsLayout->addWidget( pButtonCheckPointSaveNow_ );
    pChangeDiplomacyDialog_ = new MOS_ChangeDiplomacyDialog( this );
    pChangeDiplomacyDialog_->hide();
    connect( pButtonCheckPointSaveNow_, SIGNAL( clicked() ), this, SLOT( SlotChangeDiplomacy() ) );


    assert( pAgentListView_ );
    pAgentListView_->Initialize();
    pOdbListView_->Initialize();
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentEditor::Terminate
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_AgentEditor::Terminate()
{
    assert( pAgentListView_ );
    pAgentListView_->Terminate();
    
    delete pRensEni_;
    pRensEni_ = 0;

    delete pDrawTeamList_;
    pDrawTeamList_ = 0;

    delete pChangeDiplomacyDialog_;
    pChangeDiplomacyDialog_ = 0;
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentEditor::IsActive
// Created: FBD 03-01-10
//-----------------------------------------------------------------------------
bool MOS_AgentEditor::IsActive( bool /*bMove*/ )
{
    QWidgetStack* pStack = (QWidgetStack*)parent();
    if ( pStack->visibleWidget() == this )
    {
        return true;
    }
    return false;
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentEditor::NotifyDeleteAgent
// Created: FBD 03-01-15
//-----------------------------------------------------------------------------
void MOS_AgentEditor::NotifyDeleteAgent( MOS_Agent& agent )
{
    assert( pAgentListView_ );
    pAgentListView_->NotifyDeleteAgent( agent );
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentEditor::NotifyAgentHasMoved
// Created: FBD 03-02-05
//-----------------------------------------------------------------------------
void MOS_AgentEditor::NotifyAgentHasMoved( MOS_Agent& agent, const MT_Vector2D& vPos )
{
    assert( pAgentListView_ );
    pAgentListView_->NotifyAgentHasMoved( agent, vPos );
}
//-----------------------------------------------------------------------------
// Name: MOS_AgentEditor::NotifyAgentMissionAck
// Created: FBD 03-02-06
//-----------------------------------------------------------------------------
void MOS_AgentEditor::NotifyAgentMissionAck( ASN1T_EnumOrderErrorCode nErrorCode )
{
    assert( pAgentListView_ );
    pAgentListView_->NotifyAgentMissionAck( nErrorCode );
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentEditor::Draw
// Created: AGN 03-05-21
//-----------------------------------------------------------------------------
void MOS_AgentEditor::Draw()
{
    if( pRensEni_ != 0 )
        pRensEni_->Draw();
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentEditor::SlotChangeCheckPointFrequency
// Created: NLD 2003-08-05
// -----------------------------------------------------------------------------
void MOS_AgentEditor::SlotChangeCheckPointFrequency()
{
    MOS_ASN_MsgCtrlCheckPointSetFrequency asnMsg;
    asnMsg.GetAsnMsg() = pCheckPointFrequency_->GetValueWidget();
    asnMsg.Send();
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentEditor::SlotChangeDiplomacy
// Created: NLD 2004-10-25
// -----------------------------------------------------------------------------
void MOS_AgentEditor::SlotChangeDiplomacy()
{
    pChangeDiplomacyDialog_->Initialize();
    pChangeDiplomacyDialog_->show();
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentEditor::SlotCheckPointSaveNow
// Created: NLD 2003-08-05
// -----------------------------------------------------------------------------
void MOS_AgentEditor::SlotCheckPointSaveNow()
{
    MOS_ASN_MsgCtrlCheckPointSaveNow asnMsg;

    if( !pCheckPointName_->text().isEmpty() )
    {
        asnMsg.GetAsnMsg().m.nomPresent = 1;
        asnMsg.GetAsnMsg().nom = pCheckPointName_->text();
    }
    asnMsg.Send();
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentListView::MustBeDrawn
/** @param  nTeamId 
    @return 
*/
// Created: AGN 2003-10-08
// -----------------------------------------------------------------------------
bool MOS_AgentEditor::MustBeDrawn( const MOS_Team& team ) const
{
    return true;/*
    QListViewItem* pItem = pDrawTeamList_->firstChild();
    uint nCounter = 0;
    while( pItem != 0 )
    {
        if( nCounter == team.GetIdx() )
        {
            QCheckListItem* pCItem = static_cast< QCheckListItem* >( pItem );
            return pCItem->isOn();
        }
        ++nCounter;
        pItem = pItem->nextSibling();
    }

    return false;*/
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentEditor::NotifyEnableUnitVisionCones
// Created: NLD 2003-10-24
// -----------------------------------------------------------------------------
void MOS_AgentEditor::NotifyEnableUnitVisionCones( bool bEnable )
{
    pCheckBoxCone_ ->setChecked( bEnable );
    pAgentListView_->SetDrawVisionCones( bEnable );
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentEditor::NotifyEnableProfiling
// Created: NLD 2005-03-15
// -----------------------------------------------------------------------------
void MOS_AgentEditor::NotifyEnableProfiling( bool bEnable )
{
    pCheckBoxProfiling_->setChecked( bEnable );
}
