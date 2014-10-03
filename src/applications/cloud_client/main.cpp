// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifdef _MSC_VER
#pragma warning( push, 0 )
#endif
#include <QtCore>
#include <QtGui>
#ifdef _MSC_VER
#pragma warning( pop )
#endif

#include "client/Head.h"

#include "runtime/CrashHandler.h"
#include "runtime/FileSystem.h"
#include "runtime/Pool.h"
#include "runtime/Factory.h"

#include "cpplog/cpplog.hpp"

using namespace runtime;
using namespace gui;

namespace
{
const QString g_Company = "MASA Group";
const QString g_Name = "Sword Client";

struct QtLog : public cpplog::BaseLogger
{
    QtLog() : cpplog::BaseLogger()
    {
        // NOTHING
    }
    virtual bool sendLogMessage( cpplog::LogData* data )
    {
        qDebug() << data->stream.str().c_str();
        return true;
    }
};
}

int main( int argc, char* argv[] )
{
    runtime::CrashHandlerInit();
    QT_REQUIRE_VERSION( argc, argv, "4.7.0" );

    QApplication app( argc, argv );
    QCoreApplication::setOrganizationName( g_Company );
    QCoreApplication::setApplicationName( g_Name );
    app.connect( &app, SIGNAL( lastWindowClosed() ), &app, SLOT( quit() ) );
    try
    {
        QtLog log;
        Factory factory( log );
        FileSystem fs( log );
        Pool pool( 8, 256 );
        Head head( factory.GetRuntime(), fs, pool );
        return app.exec();
    }
    catch( const std::exception& err )
    {
        QMessageBox::critical( NULL, "Critical failure", err.what() );
        return -1;
    }
    catch( ... )
    {
        QMessageBox::critical( NULL, "Critical failure", "Unexpected exception" );
        return -1;
    }
}
