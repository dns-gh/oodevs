/****************************************************************************
** MOS_ControllerToolbar meta object code from reading C++ file 'MOS_ControllerToolbar.h'
**
** Created: mar. 12. juil. 12:42:03 2005
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

const char *MOS_ControllerToolbar::className() const
{
    return "MOS_ControllerToolbar";
}

QMetaObject *MOS_ControllerToolbar::metaObj = 0;
static QMetaObjectCleanUp cleanUp_MOS_ControllerToolbar( "MOS_ControllerToolbar", &MOS_ControllerToolbar::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString MOS_ControllerToolbar::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_ControllerToolbar", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString MOS_ControllerToolbar::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_ControllerToolbar", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* MOS_ControllerToolbar::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QToolBar::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "nValue", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_0 = {"OnTeamChanged", 1, param_slot_0 };
    static const QUParameter param_slot_1[] = {
	{ "team", &static_QUType_ptr, "MOS_Team", QUParameter::InOut }
    };
    static const QUMethod slot_1 = {"OnTeamCreated", 1, param_slot_1 };
    static const QUMethod slot_2 = {"LaunchScipio", 0, 0 };
    static const QUMethod slot_3 = {"LaunchScipioDbg", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "OnTeamChanged(int)", &slot_0, QMetaData::Private },
	{ "OnTeamCreated(MOS_Team&)", &slot_1, QMetaData::Private },
	{ "LaunchScipio()", &slot_2, QMetaData::Private },
	{ "LaunchScipioDbg()", &slot_3, QMetaData::Private }
    };
    static const QUMethod signal_0 = {"TeamChanged", 0, 0 };
    static const QMetaData signal_tbl[] = {
	{ "TeamChanged()", &signal_0, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"MOS_ControllerToolbar", parentObject,
	slot_tbl, 4,
	signal_tbl, 1,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_MOS_ControllerToolbar.setMetaObject( metaObj );
    return metaObj;
}

void* MOS_ControllerToolbar::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "MOS_ControllerToolbar" ) )
	return this;
    return QToolBar::qt_cast( clname );
}

// SIGNAL TeamChanged
void MOS_ControllerToolbar::TeamChanged()
{
    activate_signal( staticMetaObject()->signalOffset() + 0 );
}

bool MOS_ControllerToolbar::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: OnTeamChanged((int)static_QUType_int.get(_o+1)); break;
    case 1: OnTeamCreated((MOS_Team&)*((MOS_Team*)static_QUType_ptr.get(_o+1))); break;
    case 2: LaunchScipio(); break;
    case 3: LaunchScipioDbg(); break;
    default:
	return QToolBar::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool MOS_ControllerToolbar::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: TeamChanged(); break;
    default:
	return QToolBar::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool MOS_ControllerToolbar::qt_property( int id, int f, QVariant* v)
{
    return QToolBar::qt_property( id, f, v);
}

bool MOS_ControllerToolbar::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
