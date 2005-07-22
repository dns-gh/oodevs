/****************************************************************************
** MOS_SIMControlToolbar meta object code from reading C++ file 'MOS_SIMControlToolbar.h'
**
** Created: ven. 22. juil. 13:35:02 2005
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

const char *MOS_SIMControlToolbar::className() const
{
    return "MOS_SIMControlToolbar";
}

QMetaObject *MOS_SIMControlToolbar::metaObj = 0;
static QMetaObjectCleanUp cleanUp_MOS_SIMControlToolbar( "MOS_SIMControlToolbar", &MOS_SIMControlToolbar::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString MOS_SIMControlToolbar::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_SIMControlToolbar", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString MOS_SIMControlToolbar::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_SIMControlToolbar", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* MOS_SIMControlToolbar::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QToolBar::staticMetaObject();
    static const QUMethod slot_0 = {"SlotConnectDisconnect", 0, 0 };
    static const QUMethod slot_1 = {"SlotPlayPause", 0, 0 };
    static const QUMethod slot_2 = {"SlotSpeedChange", 0, 0 };
    static const QUMethod slot_3 = {"SlotOnSpinBoxChange", 0, 0 };
    static const QUParameter param_slot_4[] = {
	{ "bConnected", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_4 = {"SlotOnConnexionStatusChanged", 1, param_slot_4 };
    static const QUParameter param_slot_5[] = {
	{ "bPaused", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_5 = {"SlotOnPauseStatusChanged", 1, param_slot_5 };
    static const QUParameter param_slot_6[] = {
	{ "nSpeed", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_6 = {"SlotOnSpeedChanged", 1, param_slot_6 };
    static const QMetaData slot_tbl[] = {
	{ "SlotConnectDisconnect()", &slot_0, QMetaData::Protected },
	{ "SlotPlayPause()", &slot_1, QMetaData::Protected },
	{ "SlotSpeedChange()", &slot_2, QMetaData::Protected },
	{ "SlotOnSpinBoxChange()", &slot_3, QMetaData::Protected },
	{ "SlotOnConnexionStatusChanged(bool)", &slot_4, QMetaData::Protected },
	{ "SlotOnPauseStatusChanged(bool)", &slot_5, QMetaData::Protected },
	{ "SlotOnSpeedChanged(int)", &slot_6, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"MOS_SIMControlToolbar", parentObject,
	slot_tbl, 7,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_MOS_SIMControlToolbar.setMetaObject( metaObj );
    return metaObj;
}

void* MOS_SIMControlToolbar::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "MOS_SIMControlToolbar" ) )
	return this;
    return QToolBar::qt_cast( clname );
}

bool MOS_SIMControlToolbar::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: SlotConnectDisconnect(); break;
    case 1: SlotPlayPause(); break;
    case 2: SlotSpeedChange(); break;
    case 3: SlotOnSpinBoxChange(); break;
    case 4: SlotOnConnexionStatusChanged((bool)static_QUType_bool.get(_o+1)); break;
    case 5: SlotOnPauseStatusChanged((bool)static_QUType_bool.get(_o+1)); break;
    case 6: SlotOnSpeedChanged((int)static_QUType_int.get(_o+1)); break;
    default:
	return QToolBar::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool MOS_SIMControlToolbar::qt_emit( int _id, QUObject* _o )
{
    return QToolBar::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool MOS_SIMControlToolbar::qt_property( int id, int f, QVariant* v)
{
    return QToolBar::qt_property( id, f, v);
}

bool MOS_SIMControlToolbar::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
