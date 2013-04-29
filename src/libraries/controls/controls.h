// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************
#ifndef CONTROLS_H__
#define CONTROLS_H__

#include <boost/noncopyable.hpp>
#include <cstdint>
#include <vector>

namespace timeline
{
namespace controls
{
    size_t ResizeClient( void* dst, size_t size );
    size_t QuitClient  ( void* dst, size_t size );

    struct Handler_ABC : public boost::noncopyable
    {
                 Handler_ABC() {}
        virtual ~Handler_ABC() {}

        virtual void OnResizeClient() = 0;
        virtual void OnQuitClient()   = 0;
    };

    void ParseClient( Handler_ABC& handler, const void* src, size_t size );
}
}

#endif//CONTROLS_H__
