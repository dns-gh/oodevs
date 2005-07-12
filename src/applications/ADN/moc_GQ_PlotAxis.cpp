/****************************************************************************
** GQ_PlotAxis meta object code from reading C++ file 'GQ_PlotAxis.h'
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

const char *GQ_PlotAxis::className() const
{
    return "GQ_PlotAxis";
}

QMetaObject *GQ_PlotAxis::metaObj = 0;
static QMetaObjectCleanUp cleanUp_GQ_PlotAxis( "GQ_PlotAxis", &GQ_PlotAxis::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString GQ_PlotAxis::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "GQ_PlotAxis", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString GQ_PlotAxis::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "GQ_PlotAxis", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* GQ_PlotAxis::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QObject::staticMetaObject();
    static const QUMethod signal_0 = {"Touched", 0, 0 };
    static const QMetaData signal_tbl[] = {
	{ "Touched()", &signal_0, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"GQ_PlotAxis", parentObject,
	0, 0,
	signal_tbl, 1,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_GQ_PlotAxis.setMetaObject( metaObj );
    return metaObj;
}

void* GQ_PlotAxis::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "GQ_PlotAxis" ) )
	return this;
    return QObject::qt_cast( clname );
}

// SIGNAL Touched
void GQ_PlotAxis::Touched()
{
    activate_signal( staticMetaObject()->signalOffset() + 0 );
}

bool GQ_PlotAxis::qt_invoke( int _id, QUObject* _o )
{
    return QObject::qt_invoke(_id,_o);
}

bool GQ_PlotAxis::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: Touched(); break;
    default:
	return QObject::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool GQ_PlotAxis::qt_property( int id, int f, QVariant* v)
{
    return QObject::qt_property( id, f, v);
}

bool GQ_PlotAxis::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
