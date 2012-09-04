#include <QtCore>
#include <QtGui>

#include "ui_main.h"
#include "cpplog/cpplog.hpp"
#include "runtime/CrashHandler.h"
#include "runtime/Factory.h"
#include "runtime/Runtime_ABC.h"

#include <boost/filesystem/path.hpp>

#define APP_COMPANY "MASA Group"
#define APP_NAME "Cloud Client"

namespace
{
void RegisterProtocolHandler( const QString& protocol, const QString& cmd )
{
    QSettings opt( "HKEY_CLASSES_ROOT\\" + protocol, QSettings::NativeFormat );
    opt.setValue( ".", "URL:" + protocol + " Protocol" );
    opt.setValue( "URL Protocol", "" );
    QSettings open( "HKEY_CLASSES_ROOT\\" + protocol + "\\shell\\open\\command", QSettings::NativeFormat );
    open.setValue( ".", cmd );
}
}

struct QtLog : public cpplog::BaseLogger
{
    virtual bool sendLogMessage( cpplog::LogData* data )
    {
        qDebug() << data->stream.str().c_str();
        return true;
    }
};

struct Main : public QMainWindow
{
    explicit Main( const runtime::Runtime_ABC& runtime )
        : QMainWindow( 0 )
        , runtime_   ( runtime )
    {
        QCoreApplication::setOrganizationName( APP_COMPANY );
        QCoreApplication::setApplicationName( APP_NAME );
        ui_.setupUi( this );
        QString app = QString::fromStdWString( runtime_.GetModuleFilename().wstring() );
        QString cmd = QString( "\"%1\" \"%2\"" ).arg( app ).arg( "%1" );
        RegisterProtocolHandler( "sword", cmd );
        ui_.statusbar->showMessage( QCoreApplication::arguments().join( " " ) );
    }

    virtual ~Main()
    {
        // NOTHING
    }

    const runtime::Runtime_ABC& runtime_;
    Ui::Main ui_;
};

int main( int argc, char* argv[] )
{
    runtime::CrashHandlerInit();
    QT_REQUIRE_VERSION( argc, argv, "4.7.0" );

    QApplication app( argc, argv );
    app.connect( &app, SIGNAL( lastWindowClosed() ), &app, SLOT( quit() ) );
    try
    {
        QtLog log;
        runtime::Factory factory( log );
        Main gui( factory.GetRuntime() );
        gui.show();
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
