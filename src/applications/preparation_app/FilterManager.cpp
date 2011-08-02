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

namespace
{
    std::string GetCurrentLanguage()
    {
        QSettings settings;
        settings.setPath( "MASA Group", tools::translate( "Application", "SWORD" ) );
        return settings.readEntry( "/Common/Language", QTextCodec::locale() ).ascii();
    }
}

// -----------------------------------------------------------------------------
// Name: FilterManager constructor
// Created: ABR 2011-06-20
// -----------------------------------------------------------------------------
FilterManager::FilterManager( xml::xistream& xis, const tools::ExerciseConfig& config, Q3ListBox& list, Q3WidgetStack& stack )
    : config_         ( config )
    , id_             ( xis.attribute< std::string >( "id" ) )
    , currentLanguage_( GetCurrentLanguage() )
{
    assert( !id_.empty() );
    xis >> xml::start( "descriptions" )
            >> xml::list( "description", *this, &FilterManager::ReadDescription )
        >> xml::end
        >> xml::start( "filters" )
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
const std::string& FilterManager::GetName() const
{
    T_Descriptions::const_iterator it = descriptions_.find( currentLanguage_ );
    if( it != descriptions_.end() )
        return it->second;
    else
    {
        it = descriptions_.find( "en" );
        if( it != descriptions_.end() )
            return it->second;
    }
    return id_;
}

// -----------------------------------------------------------------------------
// Name: FilterManager::GetId
// Created: ABR 2011-06-24
// -----------------------------------------------------------------------------
const std::string& FilterManager::GetId() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: FilterManager::ReadDescription
// Created: ABR 2011-06-20
// -----------------------------------------------------------------------------
void FilterManager::ReadDescription( xml::xistream& xis )
{
    assert( xis.has_attribute( "xml:lang" ) && xis.has_attribute( "name" ) );
    const std::string lang = xis.attribute< std::string >( "xml:lang" );
    const std::string name = xis.attribute< std::string >( "name" );
    assert( !name.empty() && !lang.empty() );
    descriptions_[ lang ] = name;
}

// -----------------------------------------------------------------------------
// Name: FilterManager::ReadFilter
// Created: ABR 2011-06-20
// -----------------------------------------------------------------------------
void FilterManager::ReadFilter( xml::xistream& xis, Q3ListBox& list, Q3WidgetStack& stack )
{
    Filter_ABC* filter;
    if( xis.has_attribute( "command" ) )
        filter = new FilterCommand( xis, config_ );
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
    stack.addWidget( filter.CreateParametersWidget( &stack ), filters_.size() );
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
