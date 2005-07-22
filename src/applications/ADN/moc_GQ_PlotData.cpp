/****************************************************************************
** GQ_PlotData meta object code from reading C++ file 'GQ_PlotData.h'
**
** Created: jeu. 21. juil. 11:15:55 2005
**      by: The Qt MOC ($Id: $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "GQ_PlotData.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *GQ_PlotData::className() const
{
    return "GQ_PlotData";
}

QMetaObject *GQ_PlotData::metaObj = 0;
static QMetaObjectCleanUp cleanUp_GQ_PlotData( "GQ_PlotData", &GQ_PlotData::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString GQ_PlotData::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "GQ_PlotData", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString GQ_PlotData::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "GQ_PlotData", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* GQ_PlotData::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QObject::staticMetaObject();
    static const QUMethod slot_0 = {"TouchRange", 0, 0 };
    static const QUMethod slot_1 = {"TouchData", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "TouchRange()", &slot_0, QMetaData::Public },
	{ "TouchData()", &slot_1, QMetaData::Public }
    };
    static const QUMethod signal_0 = {"RangeTouched", 0, 0 };
    static const QUMethod signal_1 = {"DataTouched", 0, 0 };
    static const QMetaData signal_tbl[] = {
	{ "RangeTouched()", &signal_0, QMetaData::Public },
	{ "DataTouched()", &signal_1, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"GQ_PlotData", parentObject,
	slot_tbl, 2,
	signal_tbl, 2,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_GQ_PlotData.setMetaObject( metaObj );
    return metaObj;
}

void* GQ_PlotData::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "GQ_PlotData" ) )
	return this;
    return QObject::qt_cast( clname );
}

// SIGNAL RangeTouched
void GQ_PlotData::RangeTouched()
{
    activate_signal( staticMetaObject()->signalOffset() + 0 );
}

// SIGNAL DataTouched
void GQ_PlotData::DataTouched()
{
    activate_signal( staticMetaObject()->signalOffset() + 1 );
}

bool GQ_PlotData::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: TouchRange(); break;
    case 1: TouchData(); break;
    default:
	return QObject::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool GQ_PlotData::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: RangeTouched(); break;
    case 1: DataTouched(); break;
    default:
	return QObject::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool GQ_PlotData::qt_property( int id, int f, QVariant* v)
{
    return QObject::qt_property( id, f, v);
}

bool GQ_PlotData::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
