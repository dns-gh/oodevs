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
#include "preparation/DotationsItem.h"
#include "preparation/Dotation.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_gui/Tools.h"

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
void PropertiesTableDisplayer::Call( const Enum_ObstacleType& value )
{
    AddToDisplay( value.ToString() );
}

// -----------------------------------------------------------------------------
// Name: PropertiesTableDisplayer::Call
// Created: SBO 2006-11-10
// -----------------------------------------------------------------------------
void PropertiesTableDisplayer::Call( const DotationsItem& value )
{
    AddToDisplay( tools::translate( "PropertiesTableDisplayer", "<Edit Property Group...>" ) );
//    kernel::Iterator< const Dotation& > it = value.CreateIterator();
//    QString result;
//    while( it.HasMoreElements() )
//    {
//        const Dotation& dotation = it.NextElement();
//        result.append( dotation.type_->GetCategory() ).append( " " );
//    }
//    AddToDisplay( result );
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
