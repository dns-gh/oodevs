// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************
#ifndef EMBEDDED_ABC_H__
#define EMBEDDED_ABC_H__

#include <timeline/api.h>
#include <functional>
#include <QProcess>

/// enable this line to enable embedded core
/// this mode is only available on 32-bit
//#define USE_EMBEDDED_CORE

namespace tools
{
namespace ipc
{
    class Device;
}
}

namespace timeline
{
namespace controls
{
    typedef std::function< void( const std::string& ) > T_Logger;
}
}

namespace timeline
{
class Embedded_ABC : public QObject
                   , public boost::noncopyable
{
    Q_OBJECT;
public:
    /// constructor/destructor
             Embedded_ABC() {}
    virtual ~Embedded_ABC() {}

    /// Default factory
    static std::auto_ptr< Embedded_ABC > Factory( tools::ipc::Device& device, const controls::T_Logger& log, bool external );

    /// Public methods
    virtual void Start( const Configuration& cfg, const std::string& uuid ) = 0;

signals:
    void Error( QProcess::ProcessError error );
};
}

#endif//SERVER_H__
