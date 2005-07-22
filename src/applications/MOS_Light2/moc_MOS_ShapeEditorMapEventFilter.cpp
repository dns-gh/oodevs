/****************************************************************************
** MOS_ShapeEditorMapEventFilter meta object code from reading C++ file 'MOS_ShapeEditorMapEventFilter.h'
**
** Created: ven. 22. juil. 13:35:03 2005
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

const char *MOS_ShapeEditorMapEventFilter::className() const
{
    return "MOS_ShapeEditorMapEventFilter";
}

QMetaObject *MOS_ShapeEditorMapEventFilter::metaObj = 0;
static QMetaObjectCleanUp cleanUp_MOS_ShapeEditorMapEventFilter( "MOS_ShapeEditorMapEventFilter", &MOS_ShapeEditorMapEventFilter::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString MOS_ShapeEditorMapEventFilter::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_ShapeEditorMapEventFilter", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString MOS_ShapeEditorMapEventFilter::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_ShapeEditorMapEventFilter", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* MOS_ShapeEditorMapEventFilter::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QObject::staticMetaObject();
    static const QUMethod signal_0 = {"Done", 0, 0 };
    static const QMetaData signal_tbl[] = {
	{ "Done()", &signal_0, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"MOS_ShapeEditorMapEventFilter", parentObject,
	0, 0,
	signal_tbl, 1,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_MOS_ShapeEditorMapEventFilter.setMetaObject( metaObj );
    return metaObj;
}

void* MOS_ShapeEditorMapEventFilter::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "MOS_ShapeEditorMapEventFilter" ) )
	return this;
    if ( !qstrcmp( clname, "MOS_MapEventFilter_ABC" ) )
	return (MOS_MapEventFilter_ABC*)this;
    return QObject::qt_cast( clname );
}

// SIGNAL Done
void MOS_ShapeEditorMapEventFilter::Done()
{
    activate_signal( staticMetaObject()->signalOffset() + 0 );
}

bool MOS_ShapeEditorMapEventFilter::qt_invoke( int _id, QUObject* _o )
{
    return QObject::qt_invoke(_id,_o);
}

bool MOS_ShapeEditorMapEventFilter::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: Done(); break;
    default:
	return QObject::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool MOS_ShapeEditorMapEventFilter::qt_property( int id, int f, QVariant* v)
{
    return QObject::qt_property( id, f, v);
}

bool MOS_ShapeEditorMapEventFilter::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
