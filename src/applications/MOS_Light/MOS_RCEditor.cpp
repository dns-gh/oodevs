//*****************************************************************************
//
// $Created: FBD 02-12-17 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_RCEditor.cpp $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 9 $
// $Workfile: MOS_RCEditor.cpp $
//
//*****************************************************************************

#include "MOS_Light_pch.h"

#include "MOS_RCEditor.h"
#include "moc_MOS_RCEditor.cpp"

#ifndef MOS_USE_INLINE
#	include "MOS_RCEditor.inl"
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
#include "MOS_RCListView.h"
#include "MOS_LineManager.h"
#include "MOS_AttrEditor.h"

//-----------------------------------------------------------------------------
// Name: MOS_RCEditor constructor
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
MOS_RCEditor::MOS_RCEditor( MOS_AttrEditor& attrEditor, QWidget* pParent )
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
    QLabel* pLabel = new QLabel( "Comptes Rendus", this );
    pLabel->setAlignment(AlignHCenter );
    pLabel->setMargin( 5 );
    pLabel->setLineWidth( 2 );
    pLabel->setFrameStyle( QFrame::Sunken | QFrame::Box );
    pLabelLayout->addWidget( pLabel );

    // Listview
    pRCListView_ = new MOS_RCListView( this, eListViewEditor );
    pRCListView_->setMargin( 5 );
    pRCListView_->setLineWidth( 2 );
    pRCListView_->setFrameStyle( QFrame::Sunken | QFrame::Box );
    pLayout->addWidget( pRCListView_ );

    // CheckBox
    QHBoxLayout* pCheckBoxLayout = new QHBoxLayout( pLayout );
    pCheckBoxLayout->setMargin( 5 );
    QCheckBox* pCheckBoxAll = new QCheckBox( "Show All", this );
    pCheckBoxAll->setChecked( false );
    pCheckBoxLayout->addWidget( pCheckBoxAll );
    connect( pCheckBoxAll, SIGNAL( toggled( bool ) ), pRCListView_, SLOT( SlotSetShowAll( bool ) ) );

    setFocusPolicy( QWidget::StrongFocus );

}

//-----------------------------------------------------------------------------
// Name: MOS_RCEditor destructor
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
MOS_RCEditor::~MOS_RCEditor()
{
    
}

//-----------------------------------------------------------------------------
// Name: MOS_RCEditor::Initialize
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_RCEditor::Initialize()
{
    assert( pRCListView_ );
    pRCListView_->Initialize();
}

//-----------------------------------------------------------------------------
// Name: MOS_RCEditor::terminate
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_RCEditor::Terminate()
{
    assert( pRCListView_ );
    pRCListView_->Terminate();
}


//-----------------------------------------------------------------------------
// Name: MOS_RCEditor::IsActive
// Created: FBD 03-01-10
//-----------------------------------------------------------------------------
bool MOS_RCEditor::IsActive( bool bMove )
{
    bool bActive = attrEditor_.IsActive( bMove );
    if ( bActive && bMove )   
       bActive = attrEditor_.IsActiveTab( pWidgetTab_ );
       if ( bActive )
           bActive = pRCListView_->hasFocus();
    return bActive;
}

//-----------------------------------------------------------------------------
// Name: MOS_RCEditor::SetAgent
// Created: FBD 03-01-15
//-----------------------------------------------------------------------------
void MOS_RCEditor::SetAgent( MOS_Agent* pAgent )
{
    pRCListView_->SetAgent( pAgent );
}
