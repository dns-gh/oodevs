// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************
#ifndef TIMELINE_API_H__
#define TIMELINE_API_H__

#include <string>
#include <memory>
#include <boost/noncopyable.hpp>
#include <tools/Path.h>

class QWidget;

namespace timeline
{
class Server_ABC : public boost::noncopyable
{
public:
             Server_ABC() {}
    virtual ~Server_ABC() {}
};

struct Configuration
{
    tools::Path rundir;
    tools::Path binary;
    QWidget*    widget;
    std::string url;
    bool        external;   ///< use external process
    int         debug_port; ///< optional remove debug port
};

bool SpawnServer();
std::auto_ptr< Server_ABC > MakeServer( const Configuration& cfg );
}

#endif//TIMELINE_API_H__
