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

void UnregisterProtocolHandler( const QString& protocol )
{
    QSettings opt( "HKEY_CLASSES_ROOT\\" + protocol, QSettings::NativeFormat );
    opt.remove( "" );
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

enum Command
{
    CMD_REGISTER,
    CMD_UNREGISTER,
    CMD_RUN,
    CMD_DISPLAY,
    CMD_COUNT,
};

struct Main : public QMainWindow
{
    explicit Main( const runtime::Runtime_ABC& runtime )
        : QMainWindow( 0 )
        , runtime_   ( runtime )
        , cmd_       ( CMD_DISPLAY )
        , done_      ( false )
    {
        QCoreApplication::setOrganizationName( APP_COMPANY );
        QCoreApplication::setApplicationName( APP_NAME );
        ui_.setupUi( this );
        ParseArguments();
        ProcessCommand();
    }

    virtual ~Main()
    {
        // NOTHING
    }

    void ParseArguments()
    {
        const QStringList list = QCoreApplication::arguments();
        for( QStringList::const_iterator it = list.begin() + 1; it != list.end(); ++it )
            if( *it == "--root" && ++it != list.end() )
            {
                root_ = *it;
            }
            else if( *it == "--register" )
            {
                cmd_ = CMD_REGISTER;
            }
            else if( *it == "--unregister" )
            {
                cmd_ = CMD_UNREGISTER;
            }
            else if( *it == "--run" && ++it != list.end() )
            {
                cmd_ = CMD_RUN;
                url_ = *it;
            }
            else
            {
                throw std::runtime_error( "unknown parameter " + it->toStdString() );
            }
    }

    void ProcessCommand()
    {
        switch( cmd_ )
        {
            case CMD_REGISTER:
                Register();
                break;

            case CMD_UNREGISTER:
                Unregister();
                break;

            case CMD_RUN:
                throw std::runtime_error( "run not implemented" );
                break;

            case CMD_DISPLAY:
                break;
        }
    }

    void Register()
    {
        QString app = QString::fromStdWString( runtime_.GetModuleFilename().wstring() );
        QString cmd = QString( "\"%1\" \"%2\"" ).arg( app ).arg( "%1" );
        RegisterProtocolHandler( "sword", cmd );
        done_ = true;
    }

    void Unregister()
    {
        UnregisterProtocolHandler( "sword" );
        done_ = true;
    }

    bool IsDone() const
    {
        return done_;
    }

    const runtime::Runtime_ABC& runtime_;
    Ui::Main ui_;
    QFileInfo root_;
    Command cmd_;
    QString url_;
    bool done_;
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
        if( gui.IsDone() )
            return 0;
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
