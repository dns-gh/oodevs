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
#include "clients_kernel/Karma.h"
#include "clients_kernel/DotationType.h"
#include "clients_gui/Tools.h"

// -----------------------------------------------------------------------------
// Name: PropertiesTableDisplayer constructor
// Created: SBO 2006-10-27
// -----------------------------------------------------------------------------
PropertiesTableDisplayer::PropertiesTableDisplayer()
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
// Created: SBO 2006-10-27
// -----------------------------------------------------------------------------
void PropertiesTableDisplayer::Call( const kernel::Karma& value )
{
    AddToDisplay( value.GetName() ); // $$$$ SBO 2007-03-08: move to BaseDisplayer
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
