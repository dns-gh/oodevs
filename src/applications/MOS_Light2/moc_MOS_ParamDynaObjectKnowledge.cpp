/****************************************************************************
** MOS_ParamDynaObjectKnowledge meta object code from reading C++ file 'MOS_ParamDynaObjectKnowledge.h'
**
** Created: mar. 12. juil. 12:42:02 2005
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

const char *MOS_ParamDynaObjectKnowledge::className() const
{
    return "MOS_ParamDynaObjectKnowledge";
}

QMetaObject *MOS_ParamDynaObjectKnowledge::metaObj = 0;
static QMetaObjectCleanUp cleanUp_MOS_ParamDynaObjectKnowledge( "MOS_ParamDynaObjectKnowledge", &MOS_ParamDynaObjectKnowledge::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString MOS_ParamDynaObjectKnowledge::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_ParamDynaObjectKnowledge", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString MOS_ParamDynaObjectKnowledge::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_ParamDynaObjectKnowledge", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* MOS_ParamDynaObjectKnowledge::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QHBox::staticMetaObject();
    static const QUMethod slot_0 = {"AcceptPopupMenuKnowledge", 0, 0 };
    static const QUParameter param_slot_1[] = {
	{ "team", &static_QUType_ptr, "MOS_Team", QUParameter::InOut },
	{ "knowledge", &static_QUType_ptr, "MOS_DynaObjectKnowledge", QUParameter::InOut }
    };
    static const QUMethod slot_1 = {"OnObjectKnowledgeDeleted", 2, param_slot_1 };
    static const QMetaData slot_tbl[] = {
	{ "AcceptPopupMenuKnowledge()", &slot_0, QMetaData::Private },
	{ "OnObjectKnowledgeDeleted(MOS_Team&,MOS_DynaObjectKnowledge&)", &slot_1, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"MOS_ParamDynaObjectKnowledge", parentObject,
	slot_tbl, 2,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_MOS_ParamDynaObjectKnowledge.setMetaObject( metaObj );
    return metaObj;
}

void* MOS_ParamDynaObjectKnowledge::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "MOS_ParamDynaObjectKnowledge" ) )
	return this;
    if ( !qstrcmp( clname, "MOS_Param_ABC" ) )
	return (MOS_Param_ABC*)this;
    return QHBox::qt_cast( clname );
}

bool MOS_ParamDynaObjectKnowledge::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: AcceptPopupMenuKnowledge(); break;
    case 1: OnObjectKnowledgeDeleted((MOS_Team&)*((MOS_Team*)static_QUType_ptr.get(_o+1)),(MOS_DynaObjectKnowledge&)*((MOS_DynaObjectKnowledge*)static_QUType_ptr.get(_o+2))); break;
    default:
	return QHBox::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool MOS_ParamDynaObjectKnowledge::qt_emit( int _id, QUObject* _o )
{
    return QHBox::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool MOS_ParamDynaObjectKnowledge::qt_property( int id, int f, QVariant* v)
{
    return QHBox::qt_property( id, f, v);
}

bool MOS_ParamDynaObjectKnowledge::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
