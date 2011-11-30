// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "PropertiesTableDisplayer.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/CoordinateSystems.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/Moveable_ABC.h"
#include "clients_gui/Tools.h"
#include "preparation/Dotation.h"
#include "preparation/DotationsItem.h"

// -----------------------------------------------------------------------------
// Name: PropertiesTableDisplayer constructor
// Created: SBO 2006-10-27
// -----------------------------------------------------------------------------
PropertiesTableDisplayer::PropertiesTableDisplayer( const kernel::CoordinateConverter_ABC& converter )
    : converter_( converter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PropertiesTableDisplayer destructor
// Created: SBO 2006-10-27
// -----------------------------------------------------------------------------
PropertiesTableDisplayer::~PropertiesTableDisplayer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PropertiesTableDisplayer::Call
// Created: SBO 2006-11-10
// -----------------------------------------------------------------------------
void PropertiesTableDisplayer::Call( const Enum_PopulationAttitude& value )
{
    AddToDisplay( value.ToString() );
}

// -----------------------------------------------------------------------------
// Name: PropertiesTableDisplayer::Call
// Created: SBO 2006-11-10
// -----------------------------------------------------------------------------
void PropertiesTableDisplayer::Call( const Enum_DemolitionTargetType& value )
{
    AddToDisplay( value.ToString() );
}

// -----------------------------------------------------------------------------
// Name: PropertiesTableDisplayer::Call
// Created: ABR 2011-11-29
// -----------------------------------------------------------------------------
void PropertiesTableDisplayer::Call( const Enum_NbcState & value )
{
    AddToDisplay( value.ToString() );
}

// -----------------------------------------------------------------------------
// Name: PropertiesTableDisplayer::Call
// Created: SBO 2006-11-10
// -----------------------------------------------------------------------------
void PropertiesTableDisplayer::Call( const DotationsItem& /*value*/ )
{
    AddToDisplay( tools::translate( "PropertiesTableDisplayer", "<Edit Property Group...>" ) );
}

// -----------------------------------------------------------------------------
// Name: PropertiesTableDisplayer::Call
// Created: SBO 2008-03-25
// -----------------------------------------------------------------------------
void PropertiesTableDisplayer::Call( const geometry::Point2f& value )
{
    try
    {
        if( value.IsZero() )
            AddToDisplay( tools::translate( "PropertiesTableDisplayer", "not set" ) );
        else
            AddToDisplay( converter_.ConvertToMgrs( value ) );
    }
    catch( ... )
    {
        AddToDisplay( tools::translate( "PropertiesTableDisplayer", "invalid" ) );
    }
}

// -----------------------------------------------------------------------------
// Name: PropertiesTableDisplayer::Call
// Created: SBO 2008-03-25
// -----------------------------------------------------------------------------
void PropertiesTableDisplayer::Call( const kernel::Moveable_ABC& value )
{
    try
    {
        AddToDisplay( converter_.GetStringPosition( value.GetPosition() ).c_str() );
    }
    catch( ... )
    {
        AddToDisplay( tools::translate( "PropertiesTableDisplayer", "invalid" ) );
    }
}

// -----------------------------------------------------------------------------
// Name: PropertiesTableDisplayer::Call
// Created: JSR 2011-03-08
// -----------------------------------------------------------------------------
void PropertiesTableDisplayer::Call( const PopulationRepartition& /*value*/ )
{
    AddToDisplay( tools::translate( "PropertiesTableDisplayer", "<Edit repartition...>" ) );
}
