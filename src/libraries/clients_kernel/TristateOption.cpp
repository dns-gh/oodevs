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

using namespace kernel;

const TristateOption TristateOption::off_( "Off" );
const TristateOption TristateOption::on_( "On" );
const TristateOption TristateOption::auto_( "Auto" );

// -----------------------------------------------------------------------------
// Name: TristateOption constructor
// Created: AGE 2006-03-30
// -----------------------------------------------------------------------------
TristateOption::TristateOption( const std::string& state )
    : state_( '0' )
{
    if( state == "On" )
        state_ = '1';
    else if( state == "Auto" )
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
// Name: TristateOption::operator const char*
// Created: AGE 2006-04-19
// -----------------------------------------------------------------------------
TristateOption::operator const char*() const
{
    return state_ == 'a' ? "Auto" : ( state_ == '1' ? "On" : "Off" );
}

// -----------------------------------------------------------------------------
// Name: TristateOption::IsSet
// Created: AGE 2006-03-30
// -----------------------------------------------------------------------------
bool TristateOption::IsSet( bool automaticCondition ) const
{
    return state_ == '1' || ( automaticCondition && state_ == 'a' );
}
