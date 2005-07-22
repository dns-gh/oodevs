/****************************************************************************
** MOS_ParamLocationList meta object code from reading C++ file 'MOS_ParamLocationList.h'
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

const char *MOS_ParamLocationList::className() const
{
    return "MOS_ParamLocationList";
}

QMetaObject *MOS_ParamLocationList::metaObj = 0;
static QMetaObjectCleanUp cleanUp_MOS_ParamLocationList( "MOS_ParamLocationList", &MOS_ParamLocationList::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString MOS_ParamLocationList::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_ParamLocationList", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString MOS_ParamLocationList::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_ParamLocationList", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* MOS_ParamLocationList::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = MOS_ParamListView::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "nLocationType", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_0 = {"StartTracing", 1, param_slot_0 };
    static const QUMethod slot_1 = {"TracingDone", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "StartTracing(int)", &slot_0, QMetaData::Private },
	{ "TracingDone()", &slot_1, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"MOS_ParamLocationList", parentObject,
	slot_tbl, 2,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_MOS_ParamLocationList.setMetaObject( metaObj );
    return metaObj;
}

void* MOS_ParamLocationList::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "MOS_ParamLocationList" ) )
	return this;
    if ( !qstrcmp( clname, "MOS_Param_ABC" ) )
	return (MOS_Param_ABC*)this;
    return MOS_ParamListView::qt_cast( clname );
}

bool MOS_ParamLocationList::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: StartTracing((int)static_QUType_int.get(_o+1)); break;
    case 1: TracingDone(); break;
    default:
	return MOS_ParamListView::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool MOS_ParamLocationList::qt_emit( int _id, QUObject* _o )
{
    return MOS_ParamListView::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool MOS_ParamLocationList::qt_property( int id, int f, QVariant* v)
{
    return MOS_ParamListView::qt_property( id, f, v);
}

bool MOS_ParamLocationList::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
