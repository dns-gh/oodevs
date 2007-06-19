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
ProfilingPanel::ProfilingPanel( QWidget* parent, kernel::Controllers& controllers, const Network& network, const Simulation& simulation )
    : QTabWidget( parent )
    , controllers_( controllers )
    , network_( network )
    , simulation_( simulation )
    , previousTotalReceived_( 0 )
    , previousTotalSent_( 0 )
    , ticks_( 0 )
{
    setMargin( 5 );
    {
        QVBox* vBox = new QVBox( this );
        QHBox* box = new QHBox( vBox );
        new QLabel( tr( "Incoming: " ), box );
        networkTotalReceived_ = new QLabel( box );
        networkReceived_ = new StatisticsWidget( vBox );

        box = new QHBox( vBox );
        new QLabel( tr( "Outgoing: " ), box );
        networkTotalSent_ = new QLabel( box );
        networkSent_ = new StatisticsWidget( vBox );
        addTab( vBox, tr( "Network" ) );
    }

    {
        QVBox* vBox = new QVBox( this );
        QHBox* box = new QHBox( vBox );
        new QLabel( tr( "Memory: " ), box );
        memoryUsage_ = new QLabel( box );
        memory_ = new StatisticsWidget( vBox );
        memory_->SetYAxisCaption( tr( "Usage (MiB)" ) );

        box = new QHBox( vBox );
        new QLabel( tr( "Virtual memory: " ), box );
        virtualMemoryUsage_ = new QLabel( box );
        virtualMemory_ = new StatisticsWidget( vBox );
        virtualMemory_->SetYAxisCaption( tr( "Usage (MiB)" ) );
        addTab( vBox, tr( "Memory" ) );
    }

    {
        QVBox* vBox = new QVBox( this );
        QHBox* box = new QHBox( vBox );
        new QLabel( tr( "Shorts: " ), box );
        shortPathfindsCount_ = new QLabel( box );
        shortPathfinds_ = new StatisticsWidget( vBox );
        shortPathfinds_->SetYAxisCaption( tr( "Count" ) );

        box = new QHBox( vBox );
        new QLabel( tr( "Longs: " ), box );
        longPathfindsCount_ = new QLabel( box );
        longPathfinds_ = new StatisticsWidget( vBox );
        longPathfinds_->SetYAxisCaption( tr( "Count" ) );
        addTab( vBox, tr( "Pathfinds" ) );
    }

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

    unsigned long memory = simulation_.GetMemory();
    memory_->AddValue( ticks_, memory / 1048576. );
    memoryUsage_->setText( ToUSI( memory ) );

    unsigned long vm = simulation_.GetVirtualMemory();
    virtualMemory_->AddValue( ticks_, vm / 1048576. );
    virtualMemoryUsage_->setText( ToUSI( vm ) );

    unsigned long shortPathfinds = simulation_.GetShortPathfinds();
    shortPathfinds_->AddValue( ticks_, shortPathfinds );
    shortPathfindsCount_->setText( QString::number( shortPathfinds ) );

    unsigned long longPathfinds = simulation_.GetLongPathfinds();
    longPathfinds_->AddValue( ticks_, longPathfinds );
    longPathfindsCount_->setText( QString::number( longPathfinds ) );
}
