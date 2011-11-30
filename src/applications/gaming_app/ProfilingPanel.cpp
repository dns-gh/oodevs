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
#include "clients_kernel/Tools.h"
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
    , previousTotalBytesReceived_( 0 )
    , previousTotalBytesSent_( 0 )
    , previousTotalMsgsReceived_( 0 )
    , previousTotalMsgsSent_( 0 )
    , ticks_( 0 )
{
    setMargin( 5 );
    {
        Q3VBox* vBox = new Q3VBox( this );
        Q3HBox* box = new Q3HBox( vBox );
        new QLabel( tools::translate( "ProfilingPanel", "Incoming bytes: " ), box );
        networkTotalBytesReceived_ = new QLabel( box );
        networkBytesReceived_ = new StatisticsWidget( vBox );

        box = new Q3HBox( vBox );
        new QLabel( tools::translate( "ProfilingPanel", "Incoming messages: " ), box );
        networkTotalMsgsReceived_ = new QLabel( box );
        networkMsgsReceived_ = new StatisticsWidget( vBox );
        networkMsgsReceived_->SetYAxisCaption( tools::translate( "ProfilingPanel", "Nb messages" ) );

        box = new Q3HBox( vBox );
        new QLabel( tools::translate( "ProfilingPanel", "Outgoing bytes: " ), box );
        networkTotalBytesSent_ = new QLabel( box );
        networkBytesSent_ = new StatisticsWidget( vBox );

        box = new Q3HBox( vBox );
        new QLabel( tools::translate( "ProfilingPanel", "Outgoing messages: " ), box );
        networkTotalMsgsSent_ = new QLabel( box );
        networkMsgsSent_ = new StatisticsWidget( vBox );
        networkMsgsSent_->SetYAxisCaption( tools::translate( "ProfilingPanel", "Nb messages" ) );

        addTab( vBox, tools::translate( "ProfilingPanel", "Network" ) );
    }

    {
        Q3VBox* vBox = new Q3VBox( this );
        Q3HBox* box = new Q3HBox( vBox );
        new QLabel( tools::translate( "ProfilingPanel", "Memory: " ), box );
        memoryUsage_ = new QLabel( box );
        memory_ = new StatisticsWidget( vBox );
        memory_->SetYAxisCaption( tools::translate( "ProfilingPanel", "Usage (MiB)" ) );

        box = new Q3HBox( vBox );
        new QLabel( tools::translate( "ProfilingPanel", "Virtual memory: " ), box );
        virtualMemoryUsage_ = new QLabel( box );
        virtualMemory_ = new StatisticsWidget( vBox );
        virtualMemory_->SetYAxisCaption( tools::translate( "ProfilingPanel", "Usage (MiB)" ) );
        addTab( vBox, tools::translate( "ProfilingPanel", "Memory" ) );
    }

    {
        Q3VBox* vBox = new Q3VBox( this );
        Q3HBox* box = new Q3HBox( vBox );
        new QLabel( tools::translate( "ProfilingPanel", "Shorts: " ), box );
        shortPathfindsCount_ = new QLabel( box );
        shortPathfinds_ = new StatisticsWidget( vBox );
        shortPathfinds_->SetYAxisCaption( tools::translate( "ProfilingPanel", "Count" ) );

        box = new Q3HBox( vBox );
        new QLabel( tools::translate( "ProfilingPanel", "Longs: " ), box );
        longPathfindsCount_ = new QLabel( box );
        longPathfinds_ = new StatisticsWidget( vBox );
        longPathfinds_->SetYAxisCaption( tools::translate( "ProfilingPanel", "Count" ) );
        addTab( vBox, tools::translate( "ProfilingPanel", "Pathfinds" ) );
    }

    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ProfilingPanel destructor
// Created: SBO 2007-01-04
// -----------------------------------------------------------------------------
ProfilingPanel::~ProfilingPanel()
{
    controllers_.Unregister( *this );
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
void ProfilingPanel::NotifyUpdated( const Simulation::sEndTick& )
{
    ++ticks_;
    {
        unsigned long bytesReceived = network_.GetReceivedAmount();
        networkTotalBytesReceived_->setText( ToUSI( bytesReceived ) + " - " + ToUSI( bytesReceived / ticks_ ) + "/tick" );
        networkBytesReceived_->AddValue( ticks_, bytesReceived - previousTotalBytesReceived_ );
        previousTotalBytesReceived_ = bytesReceived;
    }

    {
        unsigned long bytesSent = network_.GetSentAmount();
        networkTotalBytesSent_->setText( ToUSI( bytesSent ) + " - " + ToUSI( bytesSent / ticks_ ) + "/tick" );
        networkBytesSent_->AddValue( ticks_, bytesSent - previousTotalBytesSent_ );
        previousTotalBytesSent_ = bytesSent;
    }

    {
        unsigned long msgsReceived = network_.GetNbMessagesReceived();
        networkTotalMsgsReceived_->setText( QString::number( msgsReceived ) + " messages - " + QString::number( msgsReceived - previousTotalMsgsReceived_ ) + " for last tick" );
        networkMsgsReceived_->AddValue( ticks_, msgsReceived - previousTotalMsgsReceived_ );
        previousTotalMsgsReceived_ = msgsReceived;
    }

    {
        unsigned long msgsSent = network_.GetNbMessagesSent();
        networkTotalMsgsSent_->setText( QString::number( msgsSent ) + " messages - " + QString::number( msgsSent - previousTotalMsgsSent_ ) + " for last tick" );
        networkMsgsSent_->AddValue( ticks_, msgsSent - previousTotalMsgsSent_ );
        previousTotalMsgsSent_ = msgsSent;
    }

    {
        unsigned long memory = simulation_.GetMemory();
        memory_->AddValue( ticks_, unsigned long( memory / 1048576. ) );
        memoryUsage_->setText( ToUSI( memory ) );
    }

    {
        unsigned long vm = simulation_.GetVirtualMemory();
        virtualMemory_->AddValue( ticks_, unsigned long( vm / 1048576. ) );
        virtualMemoryUsage_->setText( ToUSI( vm ) );
    }

    {
        unsigned long shortPathfinds = simulation_.GetShortPathfinds();
        shortPathfinds_->AddValue( ticks_, shortPathfinds );
        shortPathfindsCount_->setText( QString::number( shortPathfinds ) );
    }

    {
        unsigned long longPathfinds = simulation_.GetLongPathfinds();
        longPathfinds_->AddValue( ticks_, longPathfinds );
        longPathfindsCount_->setText( QString::number( longPathfinds ) );
    }
}
