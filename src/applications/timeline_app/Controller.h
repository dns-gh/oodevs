// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************
#ifndef CONTROLLER_H__
#define CONTROLLER_H__

#include <timeline/api.h>
#include <memory>

#ifdef _MSC_VER
#pragma warning( push, 0 )
#endif
#include <QtGui>
#ifdef _MSC_VER
#pragma warning( pop )
#endif

namespace Ui
{
    class Main;
}

namespace timeline
{
class Controller : public QObject
{
    Q_OBJECT
public:
             Controller( const Configuration& cfg );
    virtual ~Controller();

public slots:
    void OnCreateEvent();

private:
    QMainWindow main_;
    std::auto_ptr< Ui::Main > ui_;
    std::auto_ptr< Server_ABC > ctx_;
};
}

#endif//CONTROLLER_H__
