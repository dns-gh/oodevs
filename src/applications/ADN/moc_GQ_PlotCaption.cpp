/****************************************************************************
** GQ_PlotCaption meta object code from reading C++ file 'GQ_PlotCaption.h'
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

const char *GQ_PlotCaption::className() const
{
    return "GQ_PlotCaption";
}

QMetaObject *GQ_PlotCaption::metaObj = 0;
static QMetaObjectCleanUp cleanUp_GQ_PlotCaption( "GQ_PlotCaption", &GQ_PlotCaption::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString GQ_PlotCaption::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "GQ_PlotCaption", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString GQ_PlotCaption::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "GQ_PlotCaption", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* GQ_PlotCaption::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QHBox::staticMetaObject();
    static const QUMethod slot_0 = {"UpdateCaption", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "UpdateCaption()", &slot_0, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"GQ_PlotCaption", parentObject,
	slot_tbl, 1,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_GQ_PlotCaption.setMetaObject( metaObj );
    return metaObj;
}

void* GQ_PlotCaption::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "GQ_PlotCaption" ) )
	return this;
    return QHBox::qt_cast( clname );
}

bool GQ_PlotCaption::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: UpdateCaption(); break;
    default:
	return QHBox::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool GQ_PlotCaption::qt_emit( int _id, QUObject* _o )
{
    return QHBox::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool GQ_PlotCaption::qt_property( int id, int f, QVariant* v)
{
    return QHBox::qt_property( id, f, v);
}

bool GQ_PlotCaption::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
