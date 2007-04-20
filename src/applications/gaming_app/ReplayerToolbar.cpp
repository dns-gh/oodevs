// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ReplayerToolbar.h"
#include "moc_ReplayerToolbar.cpp"
#include "gaming/Simulation.h"
#include "gaming/ASN_Messages.h"
#include "clients_gui/resources.h"
#include "clients_kernel/Controllers.h"

// -----------------------------------------------------------------------------
// Name: ReplayerToolbar constructor
// Created: AGE 2007-04-11
// -----------------------------------------------------------------------------
ReplayerToolbar::ReplayerToolbar( QMainWindow* pParent, kernel::Controllers& controllers, Publisher_ABC& network )
    : QToolBar( pParent, "replay control toolbar" )
    , controllers_( controllers )
    , network_( network )
    , slider_( 0 )
    , previousTickCount_( unsigned( -2 ) )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ReplayerToolbar destructor
// Created: AGE 2007-04-11
// -----------------------------------------------------------------------------
ReplayerToolbar::~ReplayerToolbar()
{
    controllers_.Remove( *this );
}

// -----------------------------------------------------------------------------
// Name: ReplayerToolbar::NotifyUpdated
// Created: AGE 2007-04-11
// -----------------------------------------------------------------------------
void ReplayerToolbar::NotifyUpdated( const Simulation& simulation )
{
    if( simulation.GetTickCount() != unsigned( -1 ) )
    {
        if( previousTickCount_ != simulation.GetTickCount() )
        {
            delete slider_;
            slider_ = 0;
        }
        if( ! slider_ )
        {
            previousTickCount_ = simulation.GetTickCount();
            slider_ = new QSlider( 0, simulation.GetTickCount(), 1, simulation.GetCurrentTick(), Qt::Horizontal, this );
            slider_->setTracking( false );
            connect( slider_, SIGNAL( valueChanged( int ) ), SLOT( OnSliderMove( int ) ) );
        }
        if( ! isVisible() )
            show();
    }
    else if( isVisible() )
        hide();
}

// -----------------------------------------------------------------------------
// Name: ReplayerToolbar::OnSliderMove
// Created: AGE 2007-04-11
// -----------------------------------------------------------------------------
void ReplayerToolbar::OnSliderMove( int frame )
{
    ASN_MsgCtrlSkipToTick skip;
    skip() = frame;
    skip.Send( network_ );
}
