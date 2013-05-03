// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************
#ifndef SERVER_H__
#define SERVER_H__

#include <timeline/api.h>
#include <QProcess>

namespace tools
{
namespace ipc
{
    class Device;
}
}

namespace timeline
{
    class Embedded_ABC;
}

namespace timeline
{
class Server : public Server_ABC
{
    Q_OBJECT
public:
             Server( const Configuration& cfg );
    virtual ~Server();

    /// Server_ABC methods
    virtual void Reload();
    virtual bool CreateEvent( const Event& event );

private:
    void StartProcess();

private slots:
    void OnError( QProcess::ProcessError error );

private:
    const Configuration cfg_;
    const std::string uuid_;
    std::auto_ptr< tools::ipc::Device > device_;
    std::auto_ptr< Embedded_ABC > embedded_;
};
}

#endif//SERVER_H__
