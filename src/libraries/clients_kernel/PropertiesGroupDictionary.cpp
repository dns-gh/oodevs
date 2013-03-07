// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "PropertiesGroupDictionary.h"
#include "UrbanObject_ABC.h"
#include "PropertiesGroup.h"
#include "Displayer_ABC.h"
#include <boost/foreach.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: PropertiesGroupDictionary constructor
// Created: LGY 2013-01-03
// -----------------------------------------------------------------------------
PropertiesGroupDictionary::PropertiesGroupDictionary( kernel::Controller& controller, Displayer_ABC& comparator )
    : kernel::PropertiesDictionary( controller )
    , comparator_( comparator )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PropertiesGroupDictionary destructor
// Created: LGY 2013-01-03
// -----------------------------------------------------------------------------
PropertiesGroupDictionary::~PropertiesGroupDictionary()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PropertiesGroupDictionary::Fill
// Created: LGY 2013-01-03
// -----------------------------------------------------------------------------
void PropertiesGroupDictionary::Fill( const std::vector< const kernel::UrbanObject_ABC* >& urbanBlocks )
{
    Clear();

    BOOST_FOREACH( const kernel::UrbanObject_ABC* block, urbanBlocks )
        if( const kernel::PropertiesDictionary* dictionary = block->Retrieve< kernel::PropertiesDictionary >() )
            BOOST_FOREACH( const T_Properties::value_type& property, dictionary->GetProperties() )
            {
                IT_Properties propertyGroup = properties_.find( property.first );
                if( propertyGroup == properties_.end() )
                    properties_[ property.first ] = new kernel::PropertiesGroup( comparator_ , *block );
                 properties_[ property.first ]->AddSubProperty( property.second );
            }
}

// -----------------------------------------------------------------------------
// Name: PropertiesGroupDictionary::Clear
// Created: LGY 2013-01-17
// -----------------------------------------------------------------------------
void PropertiesGroupDictionary::Clear()
{
    for( IT_Properties it = properties_.begin(); it != properties_.end(); ++it )
        delete it->second;
    properties_.clear();
}
