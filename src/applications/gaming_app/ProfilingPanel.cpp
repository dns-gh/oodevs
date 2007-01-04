// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ProfilingPanel.h"
#include "StatisticsWidget.h"
#include "gaming/Network.h"
#include "clients_kernel/Controllers.h"

#include <sstream>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ProfilingPanel constructor
// Created: SBO 2007-01-04
// -----------------------------------------------------------------------------
ProfilingPanel::ProfilingPanel( QWidget* parent, kernel::Controllers& controllers, const Network& network )
    : QVBox( parent )
    , controllers_( controllers )
    , network_( network )
    , previousTotalReceived_( 0 )
    , previousTotalSent_( 0 )
    , ticks_( 0 )
{
    setSpacing( 5 );
    setMargin( 5 );
    QHBox* box = new QHBox( this );
    new QLabel( tr( "Incoming: " ), box );
    networkTotalReceived_ = new QLabel( box );
    networkReceived_ = new StatisticsWidget( this );
    box = new QHBox( this );
    new QLabel( tr( "Outgoing: " ), box );
    networkTotalSent_ = new QLabel( box );
    networkSent_ = new StatisticsWidget( this );

    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ProfilingPanel destructor
// Created: SBO 2007-01-04
// -----------------------------------------------------------------------------
ProfilingPanel::~ProfilingPanel()
{
    controllers_.Remove( *this );
}

namespace
{
    QString ToUSI( unsigned long bytes )
    {
        static const char* units[] = { "B", "KiB", "MiB", "GiB" };
        unsigned int i = 0;
        while( bytes >> 10 > 0 && i < 4 )
        {
            bytes /= 1024;
            ++i;
        }
        std::stringstream ss;
        ss << bytes << ' ' << units[i];
        return ss.str().c_str();
    }
}

// -----------------------------------------------------------------------------
// Name: ProfilingPanel::NotifyUpdated
// Created: SBO 2007-01-04
// -----------------------------------------------------------------------------
void ProfilingPanel::NotifyUpdated( const Simulation::sEndTick& endTick )
{
    ++ticks_;
    unsigned long received = network_.GetReceivedAmount();
    networkTotalReceived_->setText( ToUSI( received ) + " - " + ToUSI( received / ticks_ ) + "/tick" );
    networkReceived_->AddValue( ticks_, received - previousTotalReceived_ );
    previousTotalReceived_ = received;

    unsigned long sent = network_.GetSentAmount();
    networkTotalSent_->setText( ToUSI( sent ) + " - " + ToUSI( sent / ticks_ ) + "/tick" );
    networkSent_->AddValue( ticks_, sent - previousTotalSent_ );
    previousTotalSent_ = sent;
}
