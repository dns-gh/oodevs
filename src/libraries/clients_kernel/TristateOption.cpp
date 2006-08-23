// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "TristateOption.h"
#include "Tools.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: TristateOption::Statics
// Created: AGE 2006-08-23
// -----------------------------------------------------------------------------
TristateOption TristateOption::Off() {
    return TristateOption( OffName() );
}

TristateOption TristateOption::On() {
    return TristateOption( OnName() );
}

TristateOption TristateOption::Auto() {
    return TristateOption( AutoName() );
}

QString TristateOption::OffName() {
    static const QString name = tools::translate( "Tristate", "Off" );
    return name;
}
    
QString TristateOption::OnName() {
    static const QString name = tools::translate( "Tristate", "On" );
    return name;
}

QString TristateOption::AutoName() {
    static const QString name = tools::translate( "Tristate", "Auto" );
    return name;
}

// -----------------------------------------------------------------------------
// Name: TristateOption constructor
// Created: AGE 2006-03-30
// -----------------------------------------------------------------------------
TristateOption::TristateOption( const QString& state )
    : state_( '0' )
{
    if( state == OnName() )
        state_ = '1';
    else if( state == AutoName() )
        state_ = 'a';
}
            
// -----------------------------------------------------------------------------
// Name: TristateOption destructor
// Created: AGE 2006-03-30
// -----------------------------------------------------------------------------
TristateOption::~TristateOption()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TristateOption::operator QString
// Created: AGE 2006-04-19
// -----------------------------------------------------------------------------
TristateOption::operator QString() const
{
    return state_ == 'a' ? AutoName() : ( state_ == '1' ? OnName() : OffName() );
}

// -----------------------------------------------------------------------------
// Name: TristateOption::IsSet
// Created: AGE 2006-03-30
// -----------------------------------------------------------------------------
bool TristateOption::IsSet( bool automaticCondition ) const
{
    return state_ == '1' || ( automaticCondition && state_ == 'a' );
}
