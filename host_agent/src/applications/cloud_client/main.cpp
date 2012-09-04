#include <QtCore>
#include <QtGui>

#include "ui_main.h"

#define APP_COMPANY "MASA Group"
#define APP_NAME "Cloud Client"

struct Main : public QMainWindow
{
    explicit Main( QMainWindow* parent = 0 )
        : QMainWindow( parent )
    {
        QCoreApplication::setOrganizationName( APP_COMPANY );
        QCoreApplication::setApplicationName( APP_NAME );
        ui_.setupUi( this );
    }

    virtual ~Main()
    {
        // NOTHING
    }

    Ui::Main ui_;
};

int main( int argc, char* argv[] )
{
    QT_REQUIRE_VERSION( argc, argv, "4.7.0" );

    QApplication app( argc, argv );
    app.connect( &app, SIGNAL( lastWindowClosed() ), &app, SLOT( quit() ) );
    try
    {
        Main gui;
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
