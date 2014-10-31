// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "EventMarkerPresenter.h"
#include "moc_EventMarkerPresenter.cpp"
#include "Event.h"
#include "EventAction.h"
#include "EventMarkerViewState.h"
#include "EventView_ABC.h"
#include "clients_kernel/TimelineHelpers.h"
#include <boost/lexical_cast.hpp>
#include <boost/property_tree/ptree.hpp>
#include <timeline/api.h>

#ifdef _MSC_VER
#   pragma warning( push )
#   pragma warning( disable : 4100 4512 )
#endif
#include <boost/property_tree/json_parser.hpp>
#ifdef _MSC_VER
#pragma warning( pop )
#endif

using namespace gui;
namespace bpt = boost::property_tree;

// -----------------------------------------------------------------------------
// Name: EventMarkerPresenter constructor
// Created: JSR 2014-10-24
// -----------------------------------------------------------------------------
EventMarkerPresenter::EventMarkerPresenter( EventView_ABC< EventMarkerViewState >& view, const std::string& uuid )
    : EventSubPresenter_ABC< EventMarkerViewState >( eEventTypes_Marker, view )
    , uuid_( uuid )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EventMarkerPresenter destructor
// Created: JSR 2014-10-24
// -----------------------------------------------------------------------------
EventMarkerPresenter::~EventMarkerPresenter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EventMarkerPresenter::OnLabelChanged
// Created: JSR 2014-10-24
// -----------------------------------------------------------------------------
void EventMarkerPresenter::OnLabelChanged( const QString& label )
{
    state_->label_ = label.toStdString();
    BuildView();
}

// -----------------------------------------------------------------------------
// Name: EventMarkerPresenter::OnDescriptionChanged
// Created: JSR 2014-10-24
// -----------------------------------------------------------------------------
void EventMarkerPresenter::OnDescriptionChanged( const QString& description )
{
    state_->description_ = description.toStdString();
    BuildView();
}

// -----------------------------------------------------------------------------
// Name: EventMarkerPresenter::OnResetDrawingsChanged
// Created: JSR 2014-10-24
// -----------------------------------------------------------------------------
void EventMarkerPresenter::OnResetDrawingsChanged( bool resetDrawings )
{
    state_->resetDrawings_ = resetDrawings;
    BuildView();
}

// -----------------------------------------------------------------------------
// Name: EventMarkerPresenter::OnDrawingsPathChanged
// Created: JSR 2014-10-24
// -----------------------------------------------------------------------------
void EventMarkerPresenter::OnDrawingsPathChanged( const tools::Path& path )
{
    state_->drawingsPath_ = path;
    BuildView();
}

// -----------------------------------------------------------------------------
// Name: EventMarkerPresenter::OnConfigurationPathChanged
// Created: JSR 2014-10-24
// -----------------------------------------------------------------------------
void EventMarkerPresenter::OnConfigurationPathChanged( const tools::Path& path )
{
    state_->configurationPath_ = path;
    BuildView();
}

// -----------------------------------------------------------------------------
// Name: EventMarkerPresenter::ShouldEnableTrigger
// Created: JSR 2014-10-24
// -----------------------------------------------------------------------------
bool EventMarkerPresenter::ShouldEnableTrigger() const
{
    return true; // TODO
}

// -----------------------------------------------------------------------------
// Name: EventMarkerPresenter::Trigger
// Created: JSR 2014-10-24
// -----------------------------------------------------------------------------
void EventMarkerPresenter::Trigger( const Event& /*event*/ )
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

// -----------------------------------------------------------------------------
// Name: EventMarkerPresenter::Clear
// Created: JSR 2014-10-24
// -----------------------------------------------------------------------------
void EventMarkerPresenter::Clear()
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

namespace
{
    const std::string resetDrawingsKey( "reset_drawings" );
    const std::string drawingsPathKey( "drawings_path" );
    const std::string configurationPathKey( "configuration_path" );
}

// -----------------------------------------------------------------------------
// Name: EventMarkerPresenter::FillFrom
// Created: JSR 2014-10-24
// -----------------------------------------------------------------------------
void EventMarkerPresenter::FillFrom( const Event& event )
{
    const timeline::Event& timelineEvent = event.GetEvent();
    state_->Purge();
    state_->label_ = timelineEvent.name;
    state_->description_ = timelineEvent.info;
    const std::string& payload = timelineEvent.action.payload;
    if( !payload.empty() )
    {
        bpt::ptree rpy;
        try
        {
            std::istringstream input( payload );
            bpt::read_json( input, rpy );
            std::istringstream stream( rpy.get< std::string >( resetDrawingsKey ) );
            stream >> std::boolalpha >> state_->resetDrawings_; // convert localized "true"/"false" to true/false
            state_->drawingsPath_ = tools::Path::FromUTF8( rpy.get< std::string >( drawingsPathKey ) );
            state_->configurationPath_ = tools::Path::FromUTF8( rpy.get< std::string >( configurationPathKey ) );
        }
        catch( ... )
        {
        }
    }
}

// -----------------------------------------------------------------------------
// Name: EventMarkerPresenter::CommitTo
// Created: JSR 2014-10-24
// -----------------------------------------------------------------------------
void EventMarkerPresenter::CommitTo( timeline::Event& event ) const
{
    event.name = state_->label_;
    event.info = state_->description_;
    event.action.apply = false;
    bpt::ptree rpy;
    rpy.put( resetDrawingsKey, state_->resetDrawings_ );
    rpy.put( drawingsPathKey, state_->drawingsPath_.ToUTF8() );
    rpy.put( configurationPathKey, state_->configurationPath_.ToUTF8() );
    std::ostringstream output;
    bpt::write_json( output, rpy );
    event.action.payload = output.str();
    event.action.apply = true;
    event.action.target = "marker://" + uuid_;
}
