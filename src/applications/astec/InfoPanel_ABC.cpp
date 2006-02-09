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
// $Archive: /MVW_v10/Build/SDK/Light2/src/InfoPanel_ABC.cpp $
// $Author: Age $
// $Modtime: 5/04/05 19:08 $
// $Revision: 1 $
// $Workfile: InfoPanel_ABC.cpp $
//
// *****************************************************************************

#include "astec_pch.h"
#include "InfoPanel_ABC.h"
#include "moc_InfoPanel_ABC.cpp"
#include "App.h"

#include "PopulationConcentration.h"
#include "PopulationFlow.h"

#include <qobjectlist.h>

// -----------------------------------------------------------------------------
// Name: InfoPanel_ABC constructor
// Created: AGE 2005-04-05
// -----------------------------------------------------------------------------
InfoPanel_ABC::InfoPanel_ABC( QWidget* pParent )
    : QScrollView( pParent )
    , pBox_( new QVBox( viewport() ) )
    //, bVisible_( false )
{
    setHScrollBarMode( QScrollView::AlwaysOff );
    pBox_->setMargin( 5 );
    pBox_->setSpacing( 5 );
    addChild( pBox_ );
    setResizePolicy( AutoOneFit );
    setFrameStyle( QFrame::Box | QFrame::Sunken );
    connect( &App::GetApp(), SIGNAL( AgentUpdated( Agent& ) ),       this, SLOT( OnAgentUpdated( Agent& ) ) );
    connect( &App::GetApp(), SIGNAL( ObjectUpdated( Object_ABC& ) ), this, SLOT( OnObjectUpdated( Object_ABC& ) ) );
    connect( &App::GetApp(), SIGNAL( PopulationUpdated( Population& ) ), this, SLOT( OnPopulationUpdated( Population& ) ) );
}

// -----------------------------------------------------------------------------
// Name: InfoPanel_ABC destructor
// Created: AGE 2005-04-05
// -----------------------------------------------------------------------------
InfoPanel_ABC::~InfoPanel_ABC()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: InfoPanel_ABC::SetSelection
// Created: AGE 2005-04-05
// -----------------------------------------------------------------------------
void InfoPanel_ABC::SetSelection( SelectedElement& item )
{
    if( selectedItem_ == item )
        return;

    selectedItem_ = item;
    OnUpdate();
}

// -----------------------------------------------------------------------------
// Name: InfoPanel_ABC::insertChild
// Created: AGE 2005-09-21
// -----------------------------------------------------------------------------
void InfoPanel_ABC::insertChild( QObject* pObj )
{
    pBox_->insertChild( pObj );
}

// -----------------------------------------------------------------------------
// Name: InfoPanel_ABC::OnAgentUpdated
// Created: AGE 2005-04-05
// -----------------------------------------------------------------------------
void InfoPanel_ABC::OnAgentUpdated( Agent& )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InfoPanel_ABC::OnAgentUpdated
// Created: HME 2005-10-06
// -----------------------------------------------------------------------------
void InfoPanel_ABC::OnPopulationUpdated( Population& )
{
    OnUpdate();
}

// -----------------------------------------------------------------------------
// Name: InfoPanel_ABC::OnObjectUpdated
// Created: AGE 2005-04-05
// -----------------------------------------------------------------------------
void InfoPanel_ABC::OnObjectUpdated( Object_ABC& )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InfoPanel_ABC::OnUpdate
// Created: AGE 2005-04-05
// -----------------------------------------------------------------------------
void InfoPanel_ABC::OnUpdate()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InfoPanel_ABC::layout
// Created: SBO 2005-09-23
// -----------------------------------------------------------------------------
QLayout* InfoPanel_ABC::layout()
{
    if( !pBox_ )
        return 0;
    return pBox_->layout();
}

// -----------------------------------------------------------------------------
// Name: InfoPanel_ABC::ShouldDisplay
// Created: AGE 2005-04-05
// -----------------------------------------------------------------------------
bool InfoPanel_ABC::ShouldDisplay( const Agent& agent ) const
{
    return isVisible() && selectedItem_.pAgent_ != 0 && selectedItem_.pAgent_ == &agent;
}

// -----------------------------------------------------------------------------
// Name: InfoPanel_ABC::ShouldDisplay
// Created: AGE 2005-04-05
// -----------------------------------------------------------------------------
bool InfoPanel_ABC::ShouldDisplay( const Object_ABC& object ) const
{
    return isVisible() && selectedItem_.pObject_ != 0 && selectedItem_.pObject_ == &object;
}

// -----------------------------------------------------------------------------
// Name: InfoPanel_ABC::ShouldDisplay
// Created: HME 2005-10-06
// -----------------------------------------------------------------------------
bool InfoPanel_ABC::ShouldDisplay( const Population& population ) const
{
    bool bPopulation    = selectedItem_.pPopulation_ != 0 && selectedItem_.pPopulation_ == &population;
    bool bConcentration = selectedItem_.pPopulationConcentration_ != 0 
                       && &selectedItem_.pPopulationConcentration_->GetPopulation() == &population;
    bool bFlow          = selectedItem_.pPopulationFlow_ != 0 
                       && &selectedItem_.pPopulationFlow_->GetPopulation() == &population;
    return isVisible() && ( bPopulation || bConcentration || bFlow );
}


// -----------------------------------------------------------------------------
// Name: InfoPanel_ABC::showEvent
// Created: AGE 2005-04-05
// -----------------------------------------------------------------------------
void InfoPanel_ABC::showEvent( QShowEvent* )
{
    OnUpdate();
}
