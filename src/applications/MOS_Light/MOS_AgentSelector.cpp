//*****************************************************************************
//
// $Created: FBD 02-12-17 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_AgentSelector.cpp $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 6 $
// $Workfile: MOS_AgentSelector.cpp $
//
//*****************************************************************************

#include "MOS_Light_pch.h"

#include "MOS_AgentSelector.h"
#include "moc_MOS_AgentSelector.cpp"

#ifndef MOS_USE_INLINE
#	include "MOS_AgentSelector.inl"
#endif


#include <QButtonGroup.h>
#include <QWidget.h>
#include <QWidgetStack.h>

#include "MOS_App.h"
#include "MOS_World.h"
#include "MOS_WorldWidget.h"
#include "MOS_Mainwindow.h"
#include "MOS_AgentListView.h"
#include "MOS_AgentMission.h"

//-----------------------------------------------------------------------------
// Name: MOS_AgentSelector constructor
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
MOS_AgentSelector::MOS_AgentSelector( QWidget* pParent )
    : QFrame( pParent )
{
    setMargin( 80 );
    setLineWidth( 2 );
    setFrameStyle( QFrame::Sunken | QFrame::Box );

    // Main Layout
    QVBoxLayout* pLayout = new QVBoxLayout( this );

    // Title
    QHBoxLayout* pLabelLayout = new QHBoxLayout( pLayout );
    QLabel* pLabel = new QLabel( "Agent Selector", this );
    pLabel->setAlignment(AlignHCenter );
    pLabel->setMargin( 5 );
    pLabel->setLineWidth( 2 );
    pLabel->setFrameStyle( QFrame::Sunken | QFrame::Box );
    pLabelLayout->addWidget( pLabel );

    // Listview
    pAgentListView_ = new MOS_AgentListView( this, eListViewSelector );
    pAgentListView_->setMargin( 5 );
    pAgentListView_->setLineWidth( 2 );
    pAgentListView_->setFrameStyle( QFrame::Sunken | QFrame::Box );
    pAgentListView_->setRootIsDecorated( false );
    pLayout->addWidget( pAgentListView_ );

    // Button Group OK/CANCEL
    QButtonGroup* pButtonGroup = new QButtonGroup( 1, Vertical, this );
    QPushButton* pCancelPushButton = new QPushButton( pButtonGroup );
    pCancelPushButton->setText( "Cancel" );
    pCancelPushButton->setAccel( Key_Escape );
    QPushButton* pOkPushButton = new QPushButton( pButtonGroup );
    pOkPushButton->setText( "Select" );
    connect( pOkPushButton, SIGNAL( clicked() ), this, SLOT( SlotAccept() ) );
    connect( pCancelPushButton, SIGNAL( clicked() ), this, SLOT( SlotReject() ) );

    QHBoxLayout* pButtonLayout = new QHBoxLayout( pLayout );
    pButtonLayout->addWidget( pButtonGroup );


    setFocusPolicy( QWidget::ClickFocus );
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentSelector destructor
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
MOS_AgentSelector::~MOS_AgentSelector()
{
    
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentSelector::Initialize
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_AgentSelector::Initialize( )
{
    assert( pAgentListView_ );
    pAgentListView_->Initialize();
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentSelector::terminate
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_AgentSelector::Terminate()
{
    assert( pAgentListView_ );
    pAgentListView_->Terminate();
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentSelector::SlotAccept
// Created: FBD 02-11-29
//-----------------------------------------------------------------------------
void MOS_AgentSelector::SlotAccept()
{
    Accept();
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentSelector::Accept
// Created: FBD 03-01-15
//-----------------------------------------------------------------------------
void MOS_AgentSelector::Accept()
{
    pMissionListView_->CreateAgentFromSelector( pAgentListView_ );
    MOS_App::GetApp().GetMainWindow().PopWidgetStackLeft();
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentSelector::SlotReject
// Created: FBD 02-12-03
//-----------------------------------------------------------------------------
void MOS_AgentSelector::SlotReject()
{
    Cancel();
}
//-----------------------------------------------------------------------------
// Name: MOS_AgentSelector::Cancel
// Created: FBD 03-01-15
//-----------------------------------------------------------------------------
void MOS_AgentSelector::Cancel()
{
    MOS_App::GetApp().GetMainWindow().PopWidgetStackLeft();
}



//-----------------------------------------------------------------------------
// Name: MOS_AgentSelector::SetAgentMission
// Created: FBD 03-01-09
//-----------------------------------------------------------------------------
void MOS_AgentSelector::SetAgentMission( MOS_AgentListView& AgentMission )
{
    pMissionListView_ = &AgentMission;
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentSelector::IsActive
// Created: FBD 03-01-10
//-----------------------------------------------------------------------------
bool MOS_AgentSelector::IsActive( bool /*bMove*/ )
{
    QWidgetStack* pStack = (QWidgetStack*)parent();
    if ( pStack->visibleWidget() == this )
    {
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentSelector::SetAgent
// Created: FBD 03-01-15
//-----------------------------------------------------------------------------
void MOS_AgentSelector::SetAgent( MOS_Agent* pAgent )
{
    pAgentListView_->SetAgent( pAgent );
}
