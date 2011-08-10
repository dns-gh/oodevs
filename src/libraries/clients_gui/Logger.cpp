// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

/* TRANSLATOR gui::Logger */

#include "clients_gui_pch.h"
#include "Logger.h"
#include "moc_Logger.cpp"
#include "ValuedListItem.h"
#include "ItemFactory_ABC.h"
#include "clients_kernel\Time_ABC.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: Logger constructor
// Created: APE 2004-06-02
// -----------------------------------------------------------------------------
Logger::Logger( QWidget* pParent, ItemFactory_ABC& factory, const kernel::Time_ABC& simulation, const std::string& filename )
    : Q3ListView  ( pParent )
    , factory_   ( factory )
    , simulation_( simulation )
    , log_       ( filename.c_str(), std::ios::out | std::ios::app )
{
    setBackgroundColor( Qt::white );
    setMinimumSize( 40, 40 );
    setShowSortIndicator( true );
    setSorting( -1 );
    setRootIsDecorated( true );
    addColumn( tr( "Real time" ) );
    addColumn( tr( "Simulation time" ) );
    addColumn( tr( "Message" ) );
    setResizeMode( Q3ListView::LastColumn );
    setAllColumnsShowFocus ( true );

    popupMenu_.insertItem( tr( "Clear list" ), this, SLOT( clear() ) );

    connect( this, SIGNAL( contextMenuRequested( Q3ListViewItem*, const QPoint&, int ) ), this, SLOT( OnRequestPopup( Q3ListViewItem*, const QPoint& ) ) );
}

namespace
{
    void MakeHeader( std::ostream& s, const kernel::Time_ABC& simulation )
    {
        s << "[" << QTime::currentTime().toString() << "] [" << simulation.GetTimeAsString() << "] ";
    }
}

// -----------------------------------------------------------------------------
// Name: Logger destructor
// Created: APE 2004-06-02
// -----------------------------------------------------------------------------
Logger::~Logger()
{
    MakeHeader( log_, simulation_ );
    log_ << "----------------------------------------------------------------" << std::endl;
}

// -----------------------------------------------------------------------------
// Name: Logger::Info
// Created: AGE 2008-05-16
// -----------------------------------------------------------------------------
Logger::LogElement Logger::Info()
{
    MakeHeader( log_, simulation_ );
    return StartLog( Qt::black, false );
}

// -----------------------------------------------------------------------------
// Name: Logger::Warning
// Created: AGE 2008-05-16
// -----------------------------------------------------------------------------
Logger::LogElement Logger::Warning()
{
    MakeHeader( log_, simulation_ );
    log_ << "Warning - ";
    return StartLog( Qt::darkRed, true );
}

// -----------------------------------------------------------------------------
// Name: Logger::Error
// Created: AGE 2008-05-16
// -----------------------------------------------------------------------------
Logger::LogElement Logger::Error()
{
    MakeHeader( log_, simulation_ );
    log_ << "Error - ";
    return StartLog( Qt::red, true );
}

// -----------------------------------------------------------------------------
// Name: Logger::StartLog
// Created: AGE 2008-05-16
// -----------------------------------------------------------------------------
Logger::LogElement Logger::StartLog( const QColor& color, bool popup )
{
    ValuedListItem* pItem = factory_.CreateItem( this );
    pItem->setText( 0, QTime::currentTime().toString() );
    pItem->setText( 1, simulation_.GetTimeAsString() );
    pItem->SetFontColor( color );
    std::stringstream* output = new std::stringstream();
    items_[ output ] = T_Item( pItem, popup );
    return LogElement( *this, *output );
}

// -----------------------------------------------------------------------------
// Name: Logger::End
// Created: AGE 2008-05-16
// -----------------------------------------------------------------------------
void Logger::End( std::stringstream& output )
{
    log_ << output.str() << std::endl;
    T_Item& item = items_[ &output ];
    if( item.first )
        item.first->setText( 2, output.str().c_str() );
    items_.erase( &output );
    delete &output;
}

// -----------------------------------------------------------------------------
// Name: Logger::OnRequestPopup
// Created: APE 2004-06-02
// -----------------------------------------------------------------------------
void Logger::OnRequestPopup( Q3ListViewItem* /*pItem*/, const QPoint& pos )
{
    popupMenu_.popup( pos );
}
