//*****************************************************************************
//
// $Created: FBD 02-12-17 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_LimaEditor.cpp $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 10 $
// $Workfile: MOS_LimaEditor.cpp $
//
//*****************************************************************************

#include "MOS_Light_Pch.h"

#include "MOS_LimaEditor.h"
#include "moc_MOS_LimaEditor.cpp"

#ifndef MOS_USE_INLINE
#	include "MOS_LimaEditor.inl"
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
#include "MOS_LimaListView.h"
#include "MOS_LineManager.h"

//-----------------------------------------------------------------------------
// Name: MOS_LimaEditor constructor
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
MOS_LimaEditor::MOS_LimaEditor( QWidget* pParent )
    : QFrame( pParent )
{
    setMargin( 80 );
    setLineWidth( 2 );
    setFrameStyle( QFrame::Sunken | QFrame::Box );

    // Main Layout
    QVBoxLayout* pLayout = new QVBoxLayout( this );

    // Title
    QHBoxLayout* pLabelLayout = new QHBoxLayout( pLayout );
    QLabel* pLabel = new QLabel( "LIMA Editor", this );
    pLabel->setAlignment(AlignHCenter );
    pLabel->setMargin( 5 );
    pLabel->setLineWidth( 2 );
    pLabel->setFrameStyle( QFrame::Sunken | QFrame::Box );
    pLabelLayout->addWidget( pLabel );

    // Listview
    pLimaListView_ = new MOS_LimaListView( this, eListViewEditor );
    pLimaListView_->setMargin( 5 );
    pLimaListView_->setLineWidth( 2 );
    pLimaListView_->setFrameStyle( QFrame::Sunken | QFrame::Box );
    pLayout->addWidget( pLimaListView_ );

    // CheckBox
    QHBoxLayout* pCheckBoxLayout = new QHBoxLayout( pLayout );
    pCheckBoxLayout->setMargin( 5 );
    QCheckBox* pCheckBoxAll = new QCheckBox( "Show All", this );
    pCheckBoxAll->setChecked( true );
    pCheckBoxLayout->addWidget( pCheckBoxAll );
    connect( pCheckBoxAll, SIGNAL( toggled( bool ) ), pLimaListView_, SLOT( SlotSetShowAll( bool ) ) );

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
// Name: MOS_LimaEditor destructor
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
MOS_LimaEditor::~MOS_LimaEditor()
{
    
}

//-----------------------------------------------------------------------------
// Name: MOS_LimaEditor::Initialize
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_LimaEditor::Initialize()
{
    assert( pLimaListView_ );
    pLimaListView_->Initialize();
}

//-----------------------------------------------------------------------------
// Name: MOS_LimaEditor::terminate
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_LimaEditor::Terminate()
{
    assert( pLimaListView_ );
    pLimaListView_->Terminate();
}

//-----------------------------------------------------------------------------
// Name: MOS_LimaEditor::SlotAccept
// Created: FBD 02-11-29
//-----------------------------------------------------------------------------
void MOS_LimaEditor::SlotAccept()
{
    Accept();
}


//-----------------------------------------------------------------------------
// Name: MOS_LimaEditor::Accept
// Created: FBD 03-01-15
//-----------------------------------------------------------------------------
void MOS_LimaEditor::Accept()
{
    if ( MOS_App::GetApp().GetLineManager().UpdateLimaToSim() )
        return;
    
    pLimaListView_->Write();


    MOS_App::GetApp().GetMainWindow().PopWidgetStackLeft();
}
//-----------------------------------------------------------------------------
// Name: MOS_LimaEditor::SlotReject
// Created: FBD 02-12-03
//-----------------------------------------------------------------------------
void MOS_LimaEditor::SlotReject()
{
    Cancel();
}

//-----------------------------------------------------------------------------
// Name: MOS_LimaEditor::Cancel
// Created: FBD 03-01-15
//-----------------------------------------------------------------------------
void MOS_LimaEditor::Cancel()
{
    MOS_App::GetApp().GetMainWindow().PopWidgetStackLeft();
}


//-----------------------------------------------------------------------------
// Name: MOS_LimaEditor::IsActive
// Created: FBD 03-01-10
//-----------------------------------------------------------------------------
bool MOS_LimaEditor::IsActive( bool /*bMove*/ )
{
    QWidgetStack* pStack = (QWidgetStack*)parent();
    if ( pStack->visibleWidget() == this )
    {
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------
// Name: MOS_LimaEditor::SetAgent
// Created: FBD 03-01-15
//-----------------------------------------------------------------------------
void MOS_LimaEditor::SetAgent( MOS_Agent* pAgent )
{
    pLimaListView_->SetAgent( pAgent );
}

//-----------------------------------------------------------------------------
// Name: MOS_LimaEditor::Read
// Created: FBD 03-01-15
//-----------------------------------------------------------------------------
void MOS_LimaEditor::Read()
{
    pLimaListView_->Read();
    
}

//-----------------------------------------------------------------------------
// Name: MOS_LimaEditor::Write
// Created: FBD 03-01-15
//-----------------------------------------------------------------------------
void MOS_LimaEditor::Write()
{
    pLimaListView_->Write();
    
}

