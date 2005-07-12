/****************************************************************************
** LMP_MainWindow meta object code from reading C++ file 'LMP_MainWindow.h'
**
** Created: mar. 12. juil. 13:30:42 2005
**      by: The Qt MOC ($Id: $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *LMP_MainWindow::className() const
{
    return "LMP_MainWindow";
}

QMetaObject *LMP_MainWindow::metaObj = 0;
static QMetaObjectCleanUp cleanUp_LMP_MainWindow( "LMP_MainWindow", &LMP_MainWindow::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString LMP_MainWindow::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "LMP_MainWindow", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString LMP_MainWindow::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "LMP_MainWindow", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* LMP_MainWindow::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QMainWindow::staticMetaObject();
    static const QUMethod slot_0 = {"SlotPushButtonReconnect", 0, 0 };
    static const QUMethod slot_1 = {"SlotPushButtonRequestListMission", 0, 0 };
    static const QUMethod slot_2 = {"SlotPushButtonRequestListSim", 0, 0 };
    static const QUMethod slot_3 = {"SlotPushButtonRequestListCheckpoint", 0, 0 };
    static const QUMethod slot_4 = {"SlotPushButtonLaunchSelectedMission", 0, 0 };
    static const QUMethod slot_5 = {"SlotPushButtonLaunchSelectedCheckpoint", 0, 0 };
    static const QUMethod slot_6 = {"SlotPushButtonStopSimulation", 0, 0 };
    static const QUMethod slot_7 = {"SlotPushButtonStopAllSimulation", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "SlotPushButtonReconnect()", &slot_0, QMetaData::Private },
	{ "SlotPushButtonRequestListMission()", &slot_1, QMetaData::Private },
	{ "SlotPushButtonRequestListSim()", &slot_2, QMetaData::Private },
	{ "SlotPushButtonRequestListCheckpoint()", &slot_3, QMetaData::Private },
	{ "SlotPushButtonLaunchSelectedMission()", &slot_4, QMetaData::Private },
	{ "SlotPushButtonLaunchSelectedCheckpoint()", &slot_5, QMetaData::Private },
	{ "SlotPushButtonStopSimulation()", &slot_6, QMetaData::Private },
	{ "SlotPushButtonStopAllSimulation()", &slot_7, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"LMP_MainWindow", parentObject,
	slot_tbl, 8,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_LMP_MainWindow.setMetaObject( metaObj );
    return metaObj;
}

void* LMP_MainWindow::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "LMP_MainWindow" ) )
	return this;
    return QMainWindow::qt_cast( clname );
}

bool LMP_MainWindow::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: SlotPushButtonReconnect(); break;
    case 1: SlotPushButtonRequestListMission(); break;
    case 2: SlotPushButtonRequestListSim(); break;
    case 3: SlotPushButtonRequestListCheckpoint(); break;
    case 4: SlotPushButtonLaunchSelectedMission(); break;
    case 5: SlotPushButtonLaunchSelectedCheckpoint(); break;
    case 6: SlotPushButtonStopSimulation(); break;
    case 7: SlotPushButtonStopAllSimulation(); break;
    default:
	return QMainWindow::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool LMP_MainWindow::qt_emit( int _id, QUObject* _o )
{
    return QMainWindow::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool LMP_MainWindow::qt_property( int id, int f, QVariant* v)
{
    return QMainWindow::qt_property( id, f, v);
}

bool LMP_MainWindow::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
