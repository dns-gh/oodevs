// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************
#ifndef TIMELINE_CORE_API_H__
#define TIMELINE_CORE_API_H__

#include <boost/noncopyable.hpp>
#include <functional>
#include <string>
#include <memory>

namespace timeline
{
namespace core
{
    struct Client_ABC : public boost::noncopyable
    {
                 Client_ABC() {}
        virtual ~Client_ABC() {}

        /// Public methods
        virtual int Run() = 0;
    };

    struct Configuration
    {
        Configuration()
            : wid( 0 )
            , debug_port( 0 )
            , log_events( false )
        {}

        int wid;
        std::string uuid;
        std::string url;
        std::string log;
        int debug_port;
        bool log_events;
    };

    bool SpawnClient();
    std::auto_ptr< Client_ABC > MakeClient( const Configuration& cfg,
           const std::function< void( const std::string& )>& logger );
}
}

#endif//TIMELINE_CORE_API_H__
