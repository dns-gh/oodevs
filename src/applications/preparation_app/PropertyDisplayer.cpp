// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "PropertyDisplayer.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/CoordinateSystems.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/InfrastructureType.h"
#include "clients_kernel/Moveable_ABC.h"
#include "clients_kernel/RoofShapeType.h"
#include "clients_kernel/UrbanColor_ABC.h"
#include "clients_kernel/UrbanTemplateType.h"
#include "clients_gui/Tools.h"
#include "preparation/Dotation.h"
#include "preparation/DotationsItem.h"

// -----------------------------------------------------------------------------
// Name: PropertyDisplayer constructor
// Created: SBO 2006-10-27
// -----------------------------------------------------------------------------
PropertyDisplayer::PropertyDisplayer( const kernel::CoordinateConverter_ABC& converter )
    : converter_( converter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PropertyDisplayer destructor
// Created: SBO 2006-10-27
// -----------------------------------------------------------------------------
PropertyDisplayer::~PropertyDisplayer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PropertyDisplayer::Call
// Created: SBO 2006-11-10
// -----------------------------------------------------------------------------
void PropertyDisplayer::Call( const Enum_PopulationAttitude& value )
{
    AddToDisplay( value.ToString() );
}

// -----------------------------------------------------------------------------
// Name: PropertyDisplayer::Call
// Created: SBO 2006-11-10
// -----------------------------------------------------------------------------
void PropertyDisplayer::Call( const Enum_DemolitionTargetType& value )
{
    AddToDisplay( value.ToString() );
}

// -----------------------------------------------------------------------------
// Name: PropertyDisplayer::Call
// Created: ABR 2011-11-29
// -----------------------------------------------------------------------------
void PropertyDisplayer::Call( const Enum_NbcState & value )
{
    AddToDisplay( value.ToString() );
}

// -----------------------------------------------------------------------------
// Name: PropertyDisplayer::Call
// Created: SBO 2006-11-10
// -----------------------------------------------------------------------------
void PropertyDisplayer::Call( const DotationsItem& /*value*/ )
{
    AddToDisplay( tools::translate( "PropertyDisplayer", "<Edit Property Group...>" ) );
}

// -----------------------------------------------------------------------------
// Name: PropertyDisplayer::Call
// Created: SBO 2008-03-25
// -----------------------------------------------------------------------------
void PropertyDisplayer::Call( const geometry::Point2f& value )
{
    try
    {
        if( value.IsZero() )
            AddToDisplay( tools::translate( "PropertyDisplayer", "not set" ) );
        else
            AddToDisplay( converter_.ConvertToMgrs( value ) );
    }
    catch( ... )
    {
        AddToDisplay( tools::translate( "PropertyDisplayer", "invalid" ) );
    }
}

// -----------------------------------------------------------------------------
// Name: PropertyDisplayer::Call
// Created: SBO 2008-03-25
// -----------------------------------------------------------------------------
void PropertyDisplayer::Call( const kernel::Moveable_ABC& value )
{
    try
    {
        AddToDisplay( converter_.GetStringPosition( value.GetPosition() ).c_str() );
    }
    catch( ... )
    {
        AddToDisplay( tools::translate( "PropertyDisplayer", "invalid" ) );
    }
}

// -----------------------------------------------------------------------------
// Name: PropertyDisplayer::Call
// Created: JSR 2011-03-08
// -----------------------------------------------------------------------------
void PropertyDisplayer::Call( const PopulationRepartition& /*value*/ )
{
    AddToDisplay( tools::translate( "PropertyDisplayer", "<Edit repartition...>" ) );
}

// -----------------------------------------------------------------------------
// Name: PropertyDisplayer::Call
// Created: ABR 2012-05-30
// -----------------------------------------------------------------------------
void PropertyDisplayer::Call( const kernel::InfrastructureType& value )
{
    AddToDisplay( value.GetName() );
}

// -----------------------------------------------------------------------------
// Name: PropertyDisplayer::Call
// Created: JSR 2012-06-20
// -----------------------------------------------------------------------------
void PropertyDisplayer::Call( const kernel::UrbanTemplateType& value )
{
    AddToDisplay( value.GetName() );
}

// -----------------------------------------------------------------------------
// Name: PropertyDisplayer::Call
// Created: ABR 2012-06-04
// -----------------------------------------------------------------------------
void PropertyDisplayer::Call( const kernel::UrbanBlockColor& value )
{
    AddToDisplay( QString( "<color red=\"%1\" green=\"%2\" blue=\"%3\" />" ).arg( value.red_ ).arg( value.green_ ).arg( value.blue_ ) );
}

// -----------------------------------------------------------------------------
// Name: PropertyDisplayer::Call
// Created: ABR 2012-06-04
// -----------------------------------------------------------------------------
void PropertyDisplayer::Call( const kernel::CriticalIntelligence& value )
{
    AddToDisplay( value() );
}
