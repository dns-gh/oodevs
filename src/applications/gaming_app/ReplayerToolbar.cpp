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
#include "clients_kernel/Controllers.h"
#include "icons.h"

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
    , userMove_( true )
{
    setLabel( tr( "Replay control" ) );

    QLabel* label = new QLabel( this );    
    label->setPixmap( MAKE_PIXMAP( replayer ) );
    addSeparator();
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
            slider_ = new QSlider( 0, simulation.GetTickCount() - 1, 1, simulation.GetCurrentTick(), Qt::Horizontal, this );
            slider_->setMinimumWidth( 200 );
            slider_->setTickInterval( slider_->maxValue() / 20 );
            slider_->setTickmarks( QSlider::Below );
            addSeparator();
            value_ = new QLabel( this );
            value_->setMargin( 5 );
            connect( slider_, SIGNAL( sliderReleased() ), SLOT( OnSliderReleased() ) );
            connect( slider_, SIGNAL( valueChanged( int ) ), SLOT( OnSliderMoved( int ) ) );
        }
        userMove_ = false;
        slider_->setValue( simulation.GetCurrentTick() );
        OnSliderMoved( simulation.GetCurrentTick() );
        userMove_ = true;
        if( ! isVisible() )
            show();
    }
    else if( isVisible() )
        hide();
}

// -----------------------------------------------------------------------------
// Name: ReplayerToolbar::OnSliderMoved
// Created: AGE 2007-04-11
// -----------------------------------------------------------------------------
void ReplayerToolbar::OnSliderMoved( int frame )
{
    value_->setText( tr( "Tick %1" ).arg( frame + 1 ) );
}

// -----------------------------------------------------------------------------
// Name: ReplayerToolbar::OnSliderReleased
// Created: SBO 2007-06-20
// -----------------------------------------------------------------------------
void ReplayerToolbar::OnSliderReleased()
{
    if( userMove_ )
    {
        ASN_MsgControlSkipToTick skip;
        skip() = slider_->value();
        skip.Send( network_ );
    }
}
