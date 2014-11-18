// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "EventHelpers.h"
#include "Event.h"
#include <timeline/api.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/assign.hpp>

#ifdef _MSC_VER
#   pragma warning( push )
#   pragma warning( disable : 4100 4512 )
#endif
#include <boost/property_tree/json_parser.hpp>
#ifdef _MSC_VER
#pragma warning( pop )
#endif

namespace bpt = boost::property_tree;

bool gui::event_helpers::StringToBool( const std::string& str )
{
    bool b;
    std::istringstream stream( str );
    stream >> std::boolalpha >> b; // convert localized "true"/"false" to true/false
    return b;
}

std::string gui::event_helpers::BoolToString( bool b )
{
    std::ostringstream os;
    os << std::boolalpha << b;
    return os.str();
}

void gui::event_helpers::ReadJsonPayload( const timeline::Event& event, std::map< std::string, std::string >& values )
{
    const std::string& payload = event.action.payload;
    if( payload.empty() )
        return;
    bpt::ptree rpy;
    std::istringstream input( payload );
    bpt::read_json( input, rpy );
    for( auto it = values.begin(); it != values.end(); ++it )
        it->second = rpy.get< std::string >( it->first, it->second );
}

void gui::event_helpers::WriteJsonPayload( const std::map< std::string, std::string >& values, timeline::Event& event )
{
    bpt::ptree rpy;
    for( auto it = values.begin(); it != values.end(); ++it )
        rpy.put( it->first, it->second );
    std::ostringstream output;
    bpt::write_json( output, rpy );
    event.action.payload = output.str();
}

std::pair< bool, bool > gui::event_helpers::GetReplayBoundariesActivation( const gui::Event& event )
{
    std::map< std::string, std::string > jsonPayload = boost::assign::map_list_of
        ( replayBeginKey, gui::event_helpers::BoolToString( true ) )
        ( replayEndKey, gui::event_helpers::BoolToString( true ) );
    if( event.GetType() == eEventTypes_Replay )
        gui::event_helpers::ReadJsonPayload( event.GetEvent(), jsonPayload );
    return std::make_pair( gui::event_helpers::StringToBool( jsonPayload[ replayBeginKey ] ),
                           gui::event_helpers::StringToBool( jsonPayload[ replayEndKey ] ) );
}
