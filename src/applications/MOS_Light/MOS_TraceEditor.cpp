//*****************************************************************************
//
// $Created: FBD 02-12-17 $
// $ATracehive: /MVW_v10/Build/SDK/MOS_Light/sTrace/MOS_TraceEditor.cpp $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 9 $
// $Workfile: MOS_TraceEditor.cpp $
//
//*****************************************************************************

#include "MOS_Light_Pch.h"

#include "MOS_TraceEditor.h"
#include "moc_MOS_TraceEditor.cpp"

#ifndef MOS_USE_INLINE
#	include "MOS_TraceEditor.inl"
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
#include "MOS_TraceListView.h"
#include "MOS_LineManager.h"
#include "MOS_AttrEditor.h"

//-----------------------------------------------------------------------------
// Name: MOS_TraceEditor constructor
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
MOS_TraceEditor::MOS_TraceEditor(  MOS_AttrEditor& attrEditor, QWidget* pParent )
    : QFrame( pParent )
    , attrEditor_( attrEditor )
    , pWidgetTab_( pParent )
{
    setMargin( 80 );
    setLineWidth( 2 );
    setFrameStyle( QFrame::Sunken | QFrame::Box );

    // Main Layout
    QVBoxLayout* pLayout = new QVBoxLayout( this );

    // Title
    QHBoxLayout* pLabelLayout = new QHBoxLayout( pLayout );
    QLabel* pLabel = new QLabel( "Trace", this );
    pLabel->setAlignment(AlignHCenter );
    pLabel->setMargin( 5 );
    pLabel->setLineWidth( 2 );
    pLabel->setFrameStyle( QFrame::Sunken | QFrame::Box );
    pLabelLayout->addWidget( pLabel );

    // Listview
    pTraceListView_ = new MOS_TraceListView( this, eListViewEditor );
    pTraceListView_->setMargin( 5 );
    pTraceListView_->setLineWidth( 2 );
    pTraceListView_->setFrameStyle( QFrame::Sunken | QFrame::Box );
    pLayout->addWidget( pTraceListView_ );

    // CheckBox

    QHBoxLayout* pCheckBoxLayout = new QHBoxLayout( pLayout );
    pCheckBoxLayout->setMargin( 5 );
    QCheckBox* pCheckBoxAll = new QCheckBox( "Show All", this );
    pCheckBoxAll->setChecked( false );
    pCheckBoxLayout->addWidget( pCheckBoxAll );
    connect( pCheckBoxAll, SIGNAL( toggled( bool ) ), pTraceListView_, SLOT( SlotSetShowAll( bool ) ) );

    setFocusPolicy( QWidget::StrongFocus );

}

//-----------------------------------------------------------------------------
// Name: MOS_TraceEditor destructor
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
MOS_TraceEditor::~MOS_TraceEditor()
{
    
}

//-----------------------------------------------------------------------------
// Name: MOS_TraceEditor::Initialize
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_TraceEditor::Initialize()
{
    assert( pTraceListView_ );
    pTraceListView_->Initialize();
}

//-----------------------------------------------------------------------------
// Name: MOS_TraceEditor::terminate
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_TraceEditor::Terminate()
{
    assert( pTraceListView_ );
    pTraceListView_->Terminate();
}


//-----------------------------------------------------------------------------
// Name: MOS_TraceEditor::IsActive
// Created: FBD 03-01-10
//-----------------------------------------------------------------------------
bool MOS_TraceEditor::IsActive( bool bMove )
{
    bool bActive = attrEditor_.IsActive( bMove );
    if ( bActive && bMove )   
       bActive = attrEditor_.IsActiveTab( pWidgetTab_ );
       if ( bActive )
           bActive = pTraceListView_->hasFocus();
    return bActive;
}

//-----------------------------------------------------------------------------
// Name: MOS_TraceEditor::SetAgent
// Created: FBD 03-01-15
//-----------------------------------------------------------------------------
void MOS_TraceEditor::SetAgent( MOS_Agent* pAgent )
{
//    if( pAgent == 0 )
//        hide();
//    else
//        show();

    pTraceListView_->SetAgent( pAgent );
}
