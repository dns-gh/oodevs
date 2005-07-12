//*****************************************************************************
//
// $Created: FBD 02-12-17 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_LimitSelector.cpp $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 11 $
// $Workfile: MOS_LimitSelector.cpp $
//
//*****************************************************************************

#include "MOS_Light_Pch.h"

#include "MOS_LimitSelector.h"
#include "moc_MOS_LimitSelector.cpp"

#ifndef MOS_USE_INLINE
#	include "MOS_LimitSelector.inl"
#endif


#include <QButtonGroup.h>
#include <QWidget.h>
#include <QCheckBox.h>
#include <QWidgetStack.h>

#include "MOS_App.h"
#include "MOS_World.h"
#include "MOS_WorldWidget.h"
#include "MOS_Mainwindow.h"
#include "MOS_LimitListView.h"
#include "MOS_LimitMission.h"

//-----------------------------------------------------------------------------
// Name: MOS_LimitSelector constructor
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
MOS_LimitSelector::MOS_LimitSelector( QWidget* pParent )
    : QFrame( pParent )
{
    setMargin( 80 );
    setLineWidth( 2 );
    setFrameStyle( QFrame::Sunken | QFrame::Box );

    // Main Layout
    QVBoxLayout* pLayout = new QVBoxLayout( this );

    // Title
    QHBoxLayout* pLabelLayout = new QHBoxLayout( pLayout );
    QLabel* pLabel = new QLabel( "Limit Selector", this );
    pLabel->setAlignment(AlignHCenter );
    pLabel->setMargin( 5 );
    pLabel->setLineWidth( 2 );
    pLabel->setFrameStyle( QFrame::Sunken | QFrame::Box );
    pLabelLayout->addWidget( pLabel );

    // Listview
    pLimitListView_ = new MOS_LimitListView( this, eListViewSelector );
    pLimitListView_->setMargin( 5 );
    pLimitListView_->setLineWidth( 2 );
    pLimitListView_->setFrameStyle( QFrame::Sunken | QFrame::Box );
    pLimitListView_->setRootIsDecorated( false );
    pLayout->addWidget( pLimitListView_ );

    // CheckBox
    QHBoxLayout* pCheckBoxLayout = new QHBoxLayout( pLayout );
    pCheckBoxLayout->setMargin( 5 );
    QCheckBox* pCheckBoxAll = new QCheckBox( "Show All", this );
    pCheckBoxAll->setChecked( true );
    pCheckBoxLayout->addWidget( pCheckBoxAll );

    connect( pCheckBoxAll, SIGNAL( toggled( bool ) ), pLimitListView_, SLOT( SlotSetShowAll( bool ) ) );

   // Button
    QButtonGroup* pBGroup = new QButtonGroup( 1, Vertical, this );
    QPushButton* pSelectButton = new QPushButton( pBGroup );
    pSelectButton->setText( "Edit Limit" );
    QHBoxLayout* pBLayout = new QHBoxLayout( pLayout );
    pBLayout->addWidget( pBGroup );
    connect( pSelectButton, SIGNAL( clicked() ), pLimitListView_, SLOT( EnterLimitEditor() ) );
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
// Name: MOS_LimitSelector destructor
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
MOS_LimitSelector::~MOS_LimitSelector()
{
    
}

//-----------------------------------------------------------------------------
// Name: MOS_LimitSelector::Initialize
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_LimitSelector::Initialize( )
{
    assert( pLimitListView_ );
    pLimitListView_->Initialize();
}

//-----------------------------------------------------------------------------
// Name: MOS_LimitSelector::terminate
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_LimitSelector::Terminate()
{
    assert( pLimitListView_ );
    pLimitListView_->Terminate();
}

//-----------------------------------------------------------------------------
// Name: MOS_LimitSelector::SlotAccept
// Created: FBD 02-11-29
//-----------------------------------------------------------------------------
void MOS_LimitSelector::SlotAccept()
{
    Accept();
}
//-----------------------------------------------------------------------------
// Name: MOS_LimitSelector::Accept
// Created: FBD 03-01-15
//-----------------------------------------------------------------------------
void MOS_LimitSelector::Accept()
{
    pMissionListView_->CreateLimitFromSelector( pLimitListView_ );
    MOS_App::GetApp().GetMainWindow().PopWidgetStackLeft();
}


//-----------------------------------------------------------------------------
// Name: MOS_LimitSelector::SlotReject
// Created: FBD 02-12-03
//-----------------------------------------------------------------------------
void MOS_LimitSelector::SlotReject()
{
    Cancel();
}
//-----------------------------------------------------------------------------
// Name: MOS_LimitSelector::Cancel
// Created: FBD 03-01-15
//-----------------------------------------------------------------------------
void MOS_LimitSelector::Cancel()
{
    MOS_App::GetApp().GetMainWindow().PopWidgetStackLeft();
}


//-----------------------------------------------------------------------------
// Name: MOS_LimitSelector::SetLimitMission
// Created: FBD 03-01-09
//-----------------------------------------------------------------------------
void MOS_LimitSelector::SetLimitMission( MOS_LimitListView& LimitMission )
{
    pMissionListView_ = &LimitMission;
}

//-----------------------------------------------------------------------------
// Name: MOS_LimitSelector::IsActive
// Created: FBD 03-01-10
//-----------------------------------------------------------------------------
bool MOS_LimitSelector::IsActive( bool /*bMove*/ )
{
    QWidgetStack* pStack = (QWidgetStack*)parent();
    if ( pStack->visibleWidget() == this )
    {
        return true;
    }
    return false;
}


//-----------------------------------------------------------------------------
// Name: MOS_LimitSelector::SetAgent
// Created: FBD 03-01-15
//-----------------------------------------------------------------------------
void MOS_LimitSelector::SetAgent( MOS_Agent* pAgent )
{
    pLimitListView_->SetAgent( pAgent );
}