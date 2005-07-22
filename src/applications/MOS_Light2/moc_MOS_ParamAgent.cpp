/****************************************************************************
** MOS_ParamAgent meta object code from reading C++ file 'MOS_ParamAgent.h'
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

const char *MOS_ParamAgent::className() const
{
    return "MOS_ParamAgent";
}

QMetaObject *MOS_ParamAgent::metaObj = 0;
static QMetaObjectCleanUp cleanUp_MOS_ParamAgent( "MOS_ParamAgent", &MOS_ParamAgent::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString MOS_ParamAgent::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_ParamAgent", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString MOS_ParamAgent::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_ParamAgent", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* MOS_ParamAgent::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QHBox::staticMetaObject();
    static const QUMethod slot_0 = {"AcceptPopupMenuAgent", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "AcceptPopupMenuAgent()", &slot_0, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"MOS_ParamAgent", parentObject,
	slot_tbl, 1,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_MOS_ParamAgent.setMetaObject( metaObj );
    return metaObj;
}

void* MOS_ParamAgent::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "MOS_ParamAgent" ) )
	return this;
    if ( !qstrcmp( clname, "MOS_Param_ABC" ) )
	return (MOS_Param_ABC*)this;
    return QHBox::qt_cast( clname );
}

bool MOS_ParamAgent::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: AcceptPopupMenuAgent(); break;
    default:
	return QHBox::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool MOS_ParamAgent::qt_emit( int _id, QUObject* _o )
{
    return QHBox::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool MOS_ParamAgent::qt_property( int id, int f, QVariant* v)
{
    return QHBox::qt_property( id, f, v);
}

bool MOS_ParamAgent::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
