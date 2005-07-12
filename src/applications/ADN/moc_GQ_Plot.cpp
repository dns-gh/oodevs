/****************************************************************************
** GQ_Plot meta object code from reading C++ file 'GQ_Plot.h'
**
** Created: jeu. 7. juil. 13:46:09 2005
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

const char *GQ_Plot::className() const
{
    return "GQ_Plot";
}

QMetaObject *GQ_Plot::metaObj = 0;
static QMetaObjectCleanUp cleanUp_GQ_Plot( "GQ_Plot", &GQ_Plot::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString GQ_Plot::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "GQ_Plot", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString GQ_Plot::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "GQ_Plot", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* GQ_Plot::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QWidget::staticMetaObject();
    static const QUMethod slot_0 = {"TouchFrame", 0, 0 };
    static const QUMethod slot_1 = {"TouchData", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "TouchFrame()", &slot_0, QMetaData::Public },
	{ "TouchData()", &slot_1, QMetaData::Public }
    };
    static const QUMethod signal_0 = {"FrameTouched", 0, 0 };
    static const QUMethod signal_1 = {"DataTouched", 0, 0 };
    static const QUParameter param_signal_2[] = {
	{ "selection", &static_QUType_ptr, "T_DataItemVector", QUParameter::In }
    };
    static const QUMethod signal_2 = {"SelectionChanged", 1, param_signal_2 };
    static const QMetaData signal_tbl[] = {
	{ "FrameTouched()", &signal_0, QMetaData::Public },
	{ "DataTouched()", &signal_1, QMetaData::Public },
	{ "SelectionChanged(const T_DataItemVector&)", &signal_2, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"GQ_Plot", parentObject,
	slot_tbl, 2,
	signal_tbl, 3,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_GQ_Plot.setMetaObject( metaObj );
    return metaObj;
}

void* GQ_Plot::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "GQ_Plot" ) )
	return this;
    return QWidget::qt_cast( clname );
}

// SIGNAL FrameTouched
void GQ_Plot::FrameTouched()
{
    activate_signal( staticMetaObject()->signalOffset() + 0 );
}

// SIGNAL DataTouched
void GQ_Plot::DataTouched()
{
    activate_signal( staticMetaObject()->signalOffset() + 1 );
}

#include <qobjectdefs.h>
#include <qsignalslotimp.h>

// SIGNAL SelectionChanged
void GQ_Plot::SelectionChanged( const T_DataItemVector& t0 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 2 );
    if ( !clist )
	return;
    QUObject o[2];
    static_QUType_ptr.set(o+1,&t0);
    activate_signal( clist, o );
}

bool GQ_Plot::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: TouchFrame(); break;
    case 1: TouchData(); break;
    default:
	return QWidget::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool GQ_Plot::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: FrameTouched(); break;
    case 1: DataTouched(); break;
    case 2: SelectionChanged((const T_DataItemVector&)*((const T_DataItemVector*)static_QUType_ptr.get(_o+1))); break;
    default:
	return QWidget::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool GQ_Plot::qt_property( int id, int f, QVariant* v)
{
    return QWidget::qt_property( id, f, v);
}

bool GQ_Plot::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
