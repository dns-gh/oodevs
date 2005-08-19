//*****************************************************************************
//
// $Created: FBD 02-12-17 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_LimitEditor.cpp $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 10 $
// $Workfile: MOS_LimitEditor.cpp $
//
//*****************************************************************************

#include "MOS_Light_pch.h"

#include "MOS_LimitEditor.h"
#include "moc_MOS_LimitEditor.cpp"

#ifndef MOS_USE_INLINE
#	include "MOS_LimitEditor.inl"
#endif


#include <QButtonGroup.h>
#include <QWidget.h>
#include <QCheckBox.h>
#include <QWidgetStack.h>
#include <QFrame.h>

#include "MOS_App.h"
#include "MOS_World.h"
#include "MOS_WorldWidget.h"
#include "MOS_Mainwindow.h"
#include "MOS_LimitListView.h"
#include "MOS_LineManager.h"

//-----------------------------------------------------------------------------
// Name: MOS_LimitEditor constructor
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
MOS_LimitEditor::MOS_LimitEditor( QWidget* pParent )
    : QFrame( pParent )
{
    setMargin( 80 );
    setLineWidth( 2 );
    setFrameStyle( QFrame::Sunken | QFrame::Box );

    // Main Layout
    QVBoxLayout* pLayout = new QVBoxLayout( this );

    // Title
    QHBoxLayout* pLabelLayout = new QHBoxLayout( pLayout );
    QLabel* pLabel = new QLabel( "Limit Editor", this );
    pLabel->setAlignment(AlignHCenter );
    pLabel->setMargin( 5 );
    pLabel->setLineWidth( 2 );
    pLabel->setFrameStyle( QFrame::Sunken | QFrame::Box );
    pLabelLayout->addWidget( pLabel );

    // Listview
    pLimitListView_ = new MOS_LimitListView( this, eListViewEditor );
    pLimitListView_->setMargin( 5 );
    pLimitListView_->setLineWidth( 2 );
    pLimitListView_->setFrameStyle( QFrame::Sunken | QFrame::Box );
    pLayout->addWidget( pLimitListView_ );

    // CheckBox
    QHBoxLayout* pCheckBoxLayout = new QHBoxLayout( pLayout );
    pCheckBoxLayout->setMargin( 5 );
    QCheckBox* pCheckBoxAll = new QCheckBox( "Show All", this );
    pCheckBoxAll->setChecked( true );
    pCheckBoxLayout->addWidget( pCheckBoxAll );
    connect( pCheckBoxAll, SIGNAL( toggled( bool ) ), pLimitListView_, SLOT( SlotSetShowAll( bool ) ) );

    // Button Group OK/CANCEL
    QButtonGroup* pButtonGroup = new QButtonGroup( 1, Vertical, this );
    QPushButton* pCancelPushButton = new QPushButton( pButtonGroup );
    pCancelPushButton->setText( "Cancel" );
    pCancelPushButton->setAccel( Key_Escape );
    QPushButton* pOkPushButton = new QPushButton( pButtonGroup );
    pOkPushButton->setText( "OK" );
    connect( pOkPushButton, SIGNAL( clicked() ), this, SLOT( SlotAccept() ) );
    connect( pCancelPushButton, SIGNAL( clicked() ), this, SLOT( SlotReject() ) );

    QHBoxLayout* pButtonLayout = new QHBoxLayout( pLayout );
    pButtonLayout->addWidget( pButtonGroup );

    setFocusPolicy( QWidget::StrongFocus );

}

//-----------------------------------------------------------------------------
// Name: MOS_LimitEditor destructor
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
MOS_LimitEditor::~MOS_LimitEditor()
{
    
}

//-----------------------------------------------------------------------------
// Name: MOS_LimitEditor::Initialize
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_LimitEditor::Initialize()
{
    assert( pLimitListView_ );
    pLimitListView_->Initialize();
}

//-----------------------------------------------------------------------------
// Name: MOS_LimitEditor::terminate
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_LimitEditor::Terminate()
{
    assert( pLimitListView_ );
    pLimitListView_->Terminate();
}

//-----------------------------------------------------------------------------
// Name: MOS_LimitEditor::SlotAccept
// Created: FBD 02-11-29
//-----------------------------------------------------------------------------
void MOS_LimitEditor::SlotAccept()
{
    Accept();
}


//-----------------------------------------------------------------------------
// Name: MOS_LimitEditor::Accept
// Created: FBD 03-01-15
//-----------------------------------------------------------------------------
void MOS_LimitEditor::Accept()
{
    if ( MOS_App::GetApp().GetLineManager().UpdateLimitToSim() )
        return;

    pLimitListView_->Write();

    MOS_App::GetApp().GetMainWindow().PopWidgetStackLeft();

}


//-----------------------------------------------------------------------------
// Name: MOS_LimitEditor::SlotReject
// Created: FBD 02-12-03
//-----------------------------------------------------------------------------
void MOS_LimitEditor::SlotReject()
{
    Cancel();
}
//-----------------------------------------------------------------------------
// Name: MOS_LimitEditor::Cancel
// Created: FBD 03-01-15
//-----------------------------------------------------------------------------
void MOS_LimitEditor::Cancel()
{
    MOS_App::GetApp().GetMainWindow().PopWidgetStackLeft();
}


//-----------------------------------------------------------------------------
// Name: MOS_LimitEditor::IsActive
// Created: FBD 03-01-10
//-----------------------------------------------------------------------------
bool MOS_LimitEditor::IsActive( bool /*bMove*/ )
{
    QWidgetStack* pStack = (QWidgetStack*)parent();
    if ( pStack->visibleWidget() == this )
    {
        return true;
    }
    return false;
}
//-----------------------------------------------------------------------------
// Name: MOS_LimitEditor::SetAgent
// Created: FBD 03-01-15
//-----------------------------------------------------------------------------
void MOS_LimitEditor::SetAgent( MOS_Agent* pAgent )
{
    pLimitListView_->SetAgent( pAgent );
}


//-----------------------------------------------------------------------------
// Name: MOS_LimitEditor::Read
// Created: FBD 03-01-15
//-----------------------------------------------------------------------------
void MOS_LimitEditor::Read()
{
    pLimitListView_->Read();
    
}

//-----------------------------------------------------------------------------
// Name: MOS_LimitEditor::Write
// Created: FBD 03-01-15
//-----------------------------------------------------------------------------
void MOS_LimitEditor::Write()
{
    pLimitListView_->Write();
    
}

