// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************
#ifndef TIMELINE_UI_API_H__
#define TIMELINE_UI_API_H__

#include <string>
#include <memory>
#include <boost/noncopyable.hpp>

class QWidget;

namespace timeline
{
    struct Context_ABC : public boost::noncopyable
    {
                 Context_ABC() {}
        virtual ~Context_ABC() {}
    };

    struct Configuration
    {
        QWidget*    widget;
        std::string target;
    };

    std::auto_ptr< Context_ABC > MakeContext( const Configuration& cfg );
}

#endif//TIMELINE_UI_H__
