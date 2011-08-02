// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "FourStateOption.h"
#include "Tools.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: FourStateOption::Statics
// Created: AGE 2006-08-23
// -----------------------------------------------------------------------------
FourStateOption FourStateOption::Off() {
    return FourStateOption( OffName() );
}

FourStateOption FourStateOption::On() {
    return FourStateOption( OnName() );
}

FourStateOption FourStateOption::Selected() {
    return  FourStateOption( SelectedName() );
}

FourStateOption FourStateOption::SuperiorSelected() {
    return FourStateOption( SuperiorSelectedName() );
}

QString FourStateOption::OffName() {
    static const QString name = tools::translate( "Fourstate", "Off" );
    return name;
}

QString FourStateOption::OnName() {
    static const QString name = tools::translate( "Fourstate", "On" );
    return name;
}

QString FourStateOption::SelectedName() {
    static const QString name = tools::translate( "Fourstate", "Selected" );
    return name;
}

QString FourStateOption::SuperiorSelectedName() {
    static const QString name = tools::translate( "Fourstate", "Superior Selected" );
    return name;
}

// -----------------------------------------------------------------------------
// Name: FourStateOption constructor
// Created: AGE 2007-05-31
// -----------------------------------------------------------------------------
FourStateOption::FourStateOption( const QString& state /* = OffName()*/ )
    : state_( '0' )
{
    if( state == OnName() || state == "1" )
        state_ = '1';
    else if( state == SelectedName() || state == "s" )
        state_ = 's';
    else if( state == SuperiorSelectedName() || state == "+" )
        state_ = '+';
}

// -----------------------------------------------------------------------------
// Name: FourStateOption destructor
// Created: AGE 2007-05-31
// -----------------------------------------------------------------------------
FourStateOption::~FourStateOption()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FourStateOption::IsSet
// Created: AGE 2007-05-31
// -----------------------------------------------------------------------------
bool FourStateOption::IsSet( bool selected, bool superior ) const
{
    return state_ == '1'
        || ( state_ == 's' && selected )
        || ( state_ == '+' && ( selected || superior ) );
}

// -----------------------------------------------------------------------------
// Name: FourStateOption::operator QString
// Created: AGE 2007-05-31
// -----------------------------------------------------------------------------
FourStateOption::operator QString() const
{
    if( state_ == '+' )
        return SuperiorSelectedName();
    if( state_ == 's' )
        return SelectedName();
    if( state_ == '1' )
        return OnName();
    return OffName();
}
