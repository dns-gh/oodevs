// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "FilterManager.h"

#include "FilterCommand.h"
#include "FilterXsl.h"
#include "preparation/Tools.h"
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: FilterManager constructor
// Created: ABR 2011-06-20
// -----------------------------------------------------------------------------
FilterManager::FilterManager( xml::xistream& xis, const tools::ExerciseConfig& config, Q3ListBox& list, Q3WidgetStack& stack, QWidget& parent )
    : description_( xis )
    , config_     ( config )
    , id_         ( xis.attribute< std::string >( "id" ) )
    , parent_     ( parent )
{
    assert( !id_.empty() );
    xis >> xml::start( "filters" )
            >> xml::list( "filter", *this, &FilterManager::ReadFilter, list, stack )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: FilterManager destructor
// Created: ABR 2011-06-20
// -----------------------------------------------------------------------------
FilterManager::~FilterManager()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FilterManager::GetName
// Created: ABR 2011-06-20
// -----------------------------------------------------------------------------
const std::string FilterManager::GetName() const
{
    std::string name = description_.GetName();
    return ( name.empty() ) ? id_ : name;
}

// -----------------------------------------------------------------------------
// Name: FilterManager::GetId
// Created: ABR 2011-06-24
// -----------------------------------------------------------------------------
const std::string FilterManager::GetId() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: FilterManager::ReadFilter
// Created: ABR 2011-06-20
// -----------------------------------------------------------------------------
void FilterManager::ReadFilter( xml::xistream& xis, Q3ListBox& list, Q3WidgetStack& stack )
{
    Filter_ABC* filter;
    if( xis.has_attribute( "command" ) )
    {
        filter = new FilterCommand( xis, config_ );
        filter->connect( filter, SIGNAL( ForceSaveAndAddActionPlanning( const std::string& ) ), &parent_, SLOT( OnForceSaveAndAddActionPlanning( const std::string& ) ) );
    }
    else
    {
        if( !xis.has_attribute( "target" ) || !xis.has_attribute( "xsl" ) || !xis.has_attribute( "output" ) )
            throw std::runtime_error( tools::translate( "FilterManager", "Bad filter attribute, either 'command' attribute or 'target', 'xsl' and 'output' attributes." ).ascii() );
        filter = new FilterXsl( xis, config_ );
    }
    AddFilter( *filter, list, stack );
}

// -----------------------------------------------------------------------------
// Name: FilterManager::AddFilter
// Created: ABR 2011-06-20
// -----------------------------------------------------------------------------
void FilterManager::AddFilter( Filter_ABC& filter, Q3ListBox& list, Q3WidgetStack& stack )
{
    list.insertItem( filter.GetName().c_str() );
    stack.addWidget( filter.CreateParametersWidget( &stack ), static_cast< int >( filters_.size() ) );
    filters_.push_back( &filter );
}

// -----------------------------------------------------------------------------
// Name: FilterManager::GetFilter
// Created: ABR 2011-06-21
// -----------------------------------------------------------------------------
Filter_ABC& FilterManager::GetFilter( unsigned int index )
{
    assert( index >= 0 && index < filters_.size() );
    return filters_.at( index );
}

// -----------------------------------------------------------------------------
// Name: FilterManager::GetFilter
// Created: ABR 2011-06-21
// -----------------------------------------------------------------------------
const Filter_ABC& FilterManager::GetFilter( unsigned int index ) const
{
    assert( index >= 0 && index < filters_.size() );
    return filters_.at( index );
}
