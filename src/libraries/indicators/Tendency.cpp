// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "indicators_pch.h"
#include "Tendency.h"
#include "clients_kernel/Displayer_ABC.h"
#include <tools/Path.h>
#include "tools/GeneralConfig.h"
#pragma warning( push, 0 )
#include <QtGui/qpixmap.h>
#pragma warning( pop )

using namespace indicators;

// -----------------------------------------------------------------------------
// Name: Tendency constructor
// Created: SBO 2009-05-05
// -----------------------------------------------------------------------------
Tendency::Tendency( unsigned int interval /* = 10*/ )
    : interval_( interval )
{
    values_.push_back( 0 );
}

// -----------------------------------------------------------------------------
// Name: Tendency destructor
// Created: SBO 2009-05-05
// -----------------------------------------------------------------------------
Tendency::~Tendency()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Tendency::AddValue
// Created: SBO 2010-07-30
// -----------------------------------------------------------------------------
void Tendency::AddValue( double value )
{
    values_.push_back( value );
    if( values_.size() > interval_ )
        values_.pop_front();
}

namespace
{
    QPixmap ComputePixmap( double value )
    {
        const tools::Path image = tools::Path( value > 0 ? "raise" : ( value < 0 ? "fall" : "stable" ) );
        return QPixmap( QString::fromWCharArray( tools::GeneralConfig::BuildResourceChildFile( tools::Path( "images/gauges/tendency" ) / image + ".png" ).ToUnicode().c_str() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: Tendency::Display
// Created: SBO 2009-05-05
// -----------------------------------------------------------------------------
void Tendency::Display( kernel::Displayer_ABC& displayer ) const
{
    displayer.Display( ComputePixmap( ( values_.back() - values_.front() ) / 2. ) );
}

// -----------------------------------------------------------------------------
// Name: Tendency::Display
// Created: JSR 2012-10-25
// -----------------------------------------------------------------------------
void Tendency::Display( QTreeWidgetItem* item, int col ) const
{
    item->setData( col, Qt::DecorationRole, ComputePixmap( ( values_.back() - values_.front() ) / 2. ) );
}
