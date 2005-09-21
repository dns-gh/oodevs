// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2005-04-05 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_InfoPanel_ABC.cpp $
// $Author: Age $
// $Modtime: 5/04/05 19:08 $
// $Revision: 1 $
// $Workfile: MOS_InfoPanel_ABC.cpp $
//
// *****************************************************************************

#include "MOS_Light2_pch.h"
#include "MOS_InfoPanel_ABC.h"
#include "moc_MOS_InfoPanel_ABC.cpp"
#include "MOS_App.h"
#include <qobjectlist.h>

// -----------------------------------------------------------------------------
// Name: MOS_InfoPanel_ABC constructor
// Created: AGE 2005-04-05
// -----------------------------------------------------------------------------
MOS_InfoPanel_ABC::MOS_InfoPanel_ABC( QWidget* pParent )
    : QScrollView( pParent )
    , pBox_( new QVBox( viewport() ) )
    , bVisible_( false )
{
    setHScrollBarMode( QScrollView::AlwaysOff );
    pBox_->setMargin( 5 );
    pBox_->setSpacing( 5 );
    addChild( pBox_ );
    setResizePolicy( AutoOneFit );
    setFrameStyle( QFrame::Box | QFrame::Sunken );
    connect( &MOS_App::GetApp(), SIGNAL( AgentUpdated( MOS_Agent& ) ),           this, SLOT( OnAgentUpdated( MOS_Agent& ) ) );
    connect( &MOS_App::GetApp(), SIGNAL( ObjectUpdated( MOS_Object_ABC& ) ), this, SLOT( OnObjectUpdated( MOS_Object_ABC& ) ) );
}

// -----------------------------------------------------------------------------
// Name: MOS_InfoPanel_ABC destructor
// Created: AGE 2005-04-05
// -----------------------------------------------------------------------------
MOS_InfoPanel_ABC::~MOS_InfoPanel_ABC()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: MOS_InfoPanel_ABC::SetSelection
// Created: AGE 2005-04-05
// -----------------------------------------------------------------------------
void MOS_InfoPanel_ABC::SetSelection( MOS_SelectedElement& item )
{
    if( selectedItem_ == item )
        return;

    selectedItem_ = item;
    OnUpdate();
}

// -----------------------------------------------------------------------------
// Name: MOS_InfoPanel_ABC::insertChild
// Created: AGE 2005-09-21
// -----------------------------------------------------------------------------
void MOS_InfoPanel_ABC::insertChild( QObject* pObj )
{
    pBox_->insertChild( pObj );
}

// -----------------------------------------------------------------------------
// Name: MOS_InfoPanel_ABC::OnAgentUpdated
// Created: AGE 2005-04-05
// -----------------------------------------------------------------------------
void MOS_InfoPanel_ABC::OnAgentUpdated( MOS_Agent& )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MOS_InfoPanel_ABC::OnObjectUpdated
// Created: AGE 2005-04-05
// -----------------------------------------------------------------------------
void MOS_InfoPanel_ABC::OnObjectUpdated( MOS_Object_ABC& )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MOS_InfoPanel_ABC::OnUpdate
// Created: AGE 2005-04-05
// -----------------------------------------------------------------------------
void MOS_InfoPanel_ABC::OnUpdate()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MOS_InfoPanel_ABC::ShouldDisplay
// Created: AGE 2005-04-05
// -----------------------------------------------------------------------------
bool MOS_InfoPanel_ABC::ShouldDisplay( MOS_Agent& agent )
{
    return isVisible() && selectedItem_.pAgent_ != 0 && selectedItem_.pAgent_ == &agent;
}

// -----------------------------------------------------------------------------
// Name: MOS_InfoPanel_ABC::ShouldDisplay
// Created: AGE 2005-04-05
// -----------------------------------------------------------------------------
bool MOS_InfoPanel_ABC::ShouldDisplay( MOS_Object_ABC& object )
{
    return isVisible() && selectedItem_.pObject_ != 0 && selectedItem_.pObject_ == &object;
}

// -----------------------------------------------------------------------------
// Name: MOS_InfoPanel_ABC::showEvent
// Created: AGE 2005-04-05
// -----------------------------------------------------------------------------
void MOS_InfoPanel_ABC::showEvent( QShowEvent* )
{
    OnUpdate();
}
