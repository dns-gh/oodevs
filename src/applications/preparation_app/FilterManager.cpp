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
#include "clients_kernel/Tools.h"
#include "clients_gui/RichWidget.h"
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: FilterManager constructor
// Created: ABR 2013-08-20
// -----------------------------------------------------------------------------
FilterManager::FilterManager( const std::string& id, const std::string& name, const tools::ExerciseConfig& config, QWidget& parent )
    : config_     ( config )
    , id_         ( id )
    , name_       ( name )
    , parent_     ( parent )
{
    // NOTHING
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
// Name: FilterManager::Load
// Created: ABR 2013-08-20
// -----------------------------------------------------------------------------
void FilterManager::Load( xml::xistream& xis, gui::RichWidget< QListWidget >& list, QStackedWidget& stack )
{
    xis >> xml::start( "filters" )
            >> xml::list( "filter", *this, &FilterManager::ReadFilter, list, stack )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: FilterManager::GetName
// Created: ABR 2011-06-20
// -----------------------------------------------------------------------------
const std::string FilterManager::GetName() const
{
    return name_;
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
void FilterManager::ReadFilter( xml::xistream& xis, gui::RichWidget< QListWidget >& list, QStackedWidget& stack )
{
    gui::SubObjectName subObject( "FilterManager" );
    Filter_ABC* filter;
    if( xis.has_attribute( "command" ) )
    {
        filter = new FilterCommand( xis, config_ );
        filter->connect( filter, SIGNAL( ForceSaveAndAddActionPlanning( const tools::Path& ) ), &parent_, SLOT( OnForceSaveAndAddActionPlanning( const tools::Path& ) ) );
    }
    else
    {
        if( !xis.has_attribute( "target" ) || !xis.has_attribute( "xsl" ) || !xis.has_attribute( "output" ) )
            throw MASA_EXCEPTION( tools::translate( "FilterManager", "Bad filter attribute, either 'command' attribute or 'target', 'xsl' and 'output' attributes." ).toStdString() );
        filter = new FilterXsl( xis, config_ );
    }
    AddFilter( *filter, list, stack );
}

// -----------------------------------------------------------------------------
// Name: FilterManager::AddFilter
// Created: ABR 2011-06-20
// -----------------------------------------------------------------------------
void FilterManager::AddFilter( Filter_ABC& filter, gui::RichWidget< QListWidget >& list, QStackedWidget& stack )
{
    list.addItem( filter.GetName().c_str() );
    stack.addWidget( filter.CreateParametersWidget( filter.GetName().c_str(), &stack ) );
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

// -----------------------------------------------------------------------------
// Name: FilterManager::Update
// Created: ABR 2012-05-30
// -----------------------------------------------------------------------------
void FilterManager::Update()
{
    for( IT_Filters it = filters_.begin(); it != filters_.end(); ++it )
        it->Update();
}
