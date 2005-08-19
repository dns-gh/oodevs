//*****************************************************************************
//
// $Created: FBD 02-12-17 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_LimaSelector.cpp $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 10 $
// $Workfile: MOS_LimaSelector.cpp $
//
//*****************************************************************************

#include "MOS_Light_pch.h"

#include "MOS_LimaSelector.h"
#include "moc_MOS_LimaSelector.cpp"

#ifndef MOS_USE_INLINE
#	include "MOS_LimaSelector.inl"
#endif


#include <QButtonGroup.h>
#include <QWidget.h>
#include <QCheckBox.h>
#include <QWidgetStack.h>

#include "MOS_App.h"
#include "MOS_World.h"
#include "MOS_WorldWidget.h"
#include "MOS_Mainwindow.h"
#include "MOS_LimaListView.h"
#include "MOS_LimaMission.h"

//-----------------------------------------------------------------------------
// Name: MOS_LimaSelector constructor
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
MOS_LimaSelector::MOS_LimaSelector( QWidget* pParent )
    : QFrame( pParent )
{
    setMargin( 80 );
    setLineWidth( 2 );
    setFrameStyle( QFrame::Sunken | QFrame::Box );

    // Main Layout
    QVBoxLayout* pLayout = new QVBoxLayout( this );

    // Title
    QHBoxLayout* pLabelLayout = new QHBoxLayout( pLayout );
    QLabel* pLabel = new QLabel( "LIMA Selector", this );
    pLabel->setAlignment(AlignHCenter );
    pLabel->setMargin( 5 );
    pLabel->setLineWidth( 2 );
    pLabel->setFrameStyle( QFrame::Sunken | QFrame::Box );
    pLabelLayout->addWidget( pLabel );

    // Listview
    pLimaListView_ = new MOS_LimaListView( this, eListViewSelector );
    pLimaListView_->setMargin( 5 );
    pLimaListView_->setLineWidth( 2 );
    pLimaListView_->setFrameStyle( QFrame::Sunken | QFrame::Box );
    pLimaListView_->setRootIsDecorated( false );
    pLayout->addWidget( pLimaListView_ );


    // CheckBox
    QHBoxLayout* pCheckBoxLayout = new QHBoxLayout( pLayout );
    pCheckBoxLayout->setMargin( 5 );
    QCheckBox* pCheckBoxAll = new QCheckBox( "Show All", this );
    pCheckBoxAll->setChecked( true );
    pCheckBoxLayout->addWidget( pCheckBoxAll );

    connect( pCheckBoxAll, SIGNAL( toggled( bool ) ), pLimaListView_, SLOT( SlotSetShowAll( bool ) ) );

    // Button
    QButtonGroup* pBGroup = new QButtonGroup( 1, Vertical, this );
    QPushButton* pSelectButton = new QPushButton( pBGroup );
    pSelectButton->setText( "Edit Lima" );
    QHBoxLayout* pBLayout = new QHBoxLayout( pLayout );
    pBLayout->addWidget( pBGroup );
    connect( pSelectButton, SIGNAL( clicked() ), pLimaListView_, SLOT( EnterLimaEditor() ) );

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
// Name: MOS_LimaSelector destructor
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
MOS_LimaSelector::~MOS_LimaSelector()
{
    
}

//-----------------------------------------------------------------------------
// Name: MOS_LimaSelector::Initialize
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_LimaSelector::Initialize( )
{
    assert( pLimaListView_ );
    pLimaListView_->Initialize();
}

//-----------------------------------------------------------------------------
// Name: MOS_LimaSelector::terminate
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_LimaSelector::Terminate()
{
}

//-----------------------------------------------------------------------------
// Name: MOS_LimaSelector::SlotAccept
// Created: FBD 02-11-29
//-----------------------------------------------------------------------------
void MOS_LimaSelector::SlotAccept()
{
    Accept();
}

//-----------------------------------------------------------------------------
// Name: MOS_LimaSelector::Accept
// Created: FBD 03-01-15
//-----------------------------------------------------------------------------
void MOS_LimaSelector::Accept()
{
    pMissionListView_->CreateLimaFromSelector( pLimaListView_ );
    MOS_App::GetApp().GetMainWindow().PopWidgetStackLeft();
}


//-----------------------------------------------------------------------------
// Name: MOS_LimaSelector::SlotReject
// Created: FBD 02-12-03
//-----------------------------------------------------------------------------
void MOS_LimaSelector::SlotReject()
{
    Cancel();
}
//-----------------------------------------------------------------------------
// Name: MOS_LimaSelector::Cancel
// Created: FBD 03-01-15
//-----------------------------------------------------------------------------
void MOS_LimaSelector::Cancel()
{
    MOS_App::GetApp().GetMainWindow().PopWidgetStackLeft();
}
//-----------------------------------------------------------------------------
// Name: MOS_LimaSelector::SetLimaMission
// Created: FBD 03-01-09
//-----------------------------------------------------------------------------
void MOS_LimaSelector::SetLimaMission( MOS_LimaListView& limaMission )
{
    pMissionListView_ = &limaMission;
}

//-----------------------------------------------------------------------------
// Name: MOS_LimaSelector::IsActive
// Created: FBD 03-01-10
//-----------------------------------------------------------------------------
bool MOS_LimaSelector::IsActive( bool /*bMove*/ )
{
    QWidgetStack* pStack = (QWidgetStack*)parent();
    if ( pStack->visibleWidget() == this )
    {
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------
// Name: MOS_LimaSelector::SetAgent
// Created: FBD 03-01-15
//-----------------------------------------------------------------------------
void MOS_LimaSelector::SetAgent( MOS_Agent* pAgent )
{
    pLimaListView_->SetAgent( pAgent );
}
