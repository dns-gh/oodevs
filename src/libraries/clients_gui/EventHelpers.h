// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __EventHelpers_h_
#define __EventHelpers_h_

namespace timeline
{
    struct Event;
}

namespace gui
{
    namespace event_helpers
    {
        bool StringToBool( const std::string& str );
        std::string BoolToString( bool b );
        void ReadJsonPayload( const timeline::Event& event, std::map< std::string, std::string >& values );
        void WriteJsonPayload( const std::map< std::string, std::string >& values, timeline::Event& event );
        
        struct ReplayPayload
        {
            ReplayPayload( bool begin, bool end, bool enabled );
            bool begin;
            bool end;
            bool enabled;
        };
        ReplayPayload ReadReplayPayload( const timeline::Event& event );
        void WriteReplayPayload( const ReplayPayload& payload, timeline::Event& event );

        static const std::string resetDrawingsKey( "reset_drawings" );
        static const std::string drawingsPathKey( "drawings_path" );
        static const std::string configurationPathKey( "configuration_path" );
        static const std::string replayBeginKey( "begin" );
        static const std::string replayEndKey( "end" );
        static const std::string replayActivationKey( "enabled" );
    } //! namespace event_helpers

} //! namespace gui

#endif // __EventHelpers_h_
