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

#include <string>
#include <memory>
#include <boost/noncopyable.hpp>

namespace timeline
{
namespace core
{
    struct Client_ABC : public boost::noncopyable
    {
                 Client_ABC() {}
        virtual ~Client_ABC() {}

        virtual int Run() = 0;
    };

    struct Configuration
    {
        int wid;
        std::string uuid;
        std::string url;
    };

    std::auto_ptr< Client_ABC > MakeClient( const Configuration& cfg );
}
}

#endif//TIMELINE_CORE_API_H__
