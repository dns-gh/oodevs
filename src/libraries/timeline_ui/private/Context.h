// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************
#ifndef CONTEXT_H__
#define CONTEXT_H__

#include <timeline_ui/api.h>
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
namespace ui
{
class Context : public QObject
              , public Context_ABC
{
    Q_OBJECT

public:
             Context( const Configuration& cfg );
    virtual ~Context();

private:
    void StartProcess();

private slots:
    void OnError( QProcess::ProcessError error );

private:
    const Configuration cfg_;
    const std::string uuid_;
    std::auto_ptr< tools::ipc::Device > device_;
    std::auto_ptr< QProcess > core_;
};
}
}

#endif//CONTEXT_H__
