/****************************************************************************
** MOS_EventToolButton meta object code from reading C++ file 'MOS_EventToolButton.h'
**
** Created: jeu. 21. juil. 15:28:03 2005
**      by: The Qt MOC ($Id: $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "MOS_EventToolButton.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *MOS_EventToolButton::className() const
{
    return "MOS_EventToolButton";
}

QMetaObject *MOS_EventToolButton::metaObj = 0;
static QMetaObjectCleanUp cleanUp_MOS_EventToolButton( "MOS_EventToolButton", &MOS_EventToolButton::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString MOS_EventToolButton::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_EventToolButton", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString MOS_EventToolButton::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_EventToolButton", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* MOS_EventToolButton::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QToolButton::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "nEventId", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_0 = {"OnNewEvent", 1, param_slot_0 };
    static const QUParameter param_slot_1[] = {
	{ "nEventId", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_1 = {"OnEventViewed", 1, param_slot_1 };
    static const QUMethod slot_2 = {"ClearEvents", 0, 0 };
    static const QUMethod slot_3 = {"OnClicked", 0, 0 };
    static const QUMethod slot_4 = {"OnTimer", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "OnNewEvent(int)", &slot_0, QMetaData::Public },
	{ "OnEventViewed(int)", &slot_1, QMetaData::Public },
	{ "ClearEvents()", &slot_2, QMetaData::Public },
	{ "OnClicked()", &slot_3, QMetaData::Private },
	{ "OnTimer()", &slot_4, QMetaData::Private }
    };
    static const QUParameter param_signal_0[] = {
	{ "nEventId", &static_QUType_int, 0, QUParameter::In },
	{ "bCenterOnEvent", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod signal_0 = {"FocusOnEvent", 2, param_signal_0 };
    static const QMetaData signal_tbl[] = {
	{ "FocusOnEvent(int,bool)", &signal_0, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"MOS_EventToolButton", parentObject,
	slot_tbl, 5,
	signal_tbl, 1,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_MOS_EventToolButton.setMetaObject( metaObj );
    return metaObj;
}

void* MOS_EventToolButton::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "MOS_EventToolButton" ) )
	return this;
    return QToolButton::qt_cast( clname );
}

#include <qobjectdefs.h>
#include <qsignalslotimp.h>

// SIGNAL FocusOnEvent
void MOS_EventToolButton::FocusOnEvent( int t0, bool t1 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 0 );
    if ( !clist )
	return;
    QUObject o[3];
    static_QUType_int.set(o+1,t0);
    static_QUType_bool.set(o+2,t1);
    activate_signal( clist, o );
}

bool MOS_EventToolButton::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: OnNewEvent((int)static_QUType_int.get(_o+1)); break;
    case 1: OnEventViewed((int)static_QUType_int.get(_o+1)); break;
    case 2: ClearEvents(); break;
    case 3: OnClicked(); break;
    case 4: OnTimer(); break;
    default:
	return QToolButton::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool MOS_EventToolButton::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: FocusOnEvent((int)static_QUType_int.get(_o+1),(bool)static_QUType_bool.get(_o+2)); break;
    default:
	return QToolButton::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool MOS_EventToolButton::qt_property( int id, int f, QVariant* v)
{
    return QToolButton::qt_property( id, f, v);
}

bool MOS_EventToolButton::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
