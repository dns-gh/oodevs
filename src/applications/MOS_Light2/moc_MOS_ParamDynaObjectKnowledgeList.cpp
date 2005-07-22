/****************************************************************************
** MOS_ParamDynaObjectKnowledgeList meta object code from reading C++ file 'MOS_ParamDynaObjectKnowledgeList.h'
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

const char *MOS_ParamDynaObjectKnowledgeList::className() const
{
    return "MOS_ParamDynaObjectKnowledgeList";
}

QMetaObject *MOS_ParamDynaObjectKnowledgeList::metaObj = 0;
static QMetaObjectCleanUp cleanUp_MOS_ParamDynaObjectKnowledgeList( "MOS_ParamDynaObjectKnowledgeList", &MOS_ParamDynaObjectKnowledgeList::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString MOS_ParamDynaObjectKnowledgeList::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_ParamDynaObjectKnowledgeList", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString MOS_ParamDynaObjectKnowledgeList::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_ParamDynaObjectKnowledgeList", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* MOS_ParamDynaObjectKnowledgeList::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = MOS_ParamListView::staticMetaObject();
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
	"MOS_ParamDynaObjectKnowledgeList", parentObject,
	slot_tbl, 2,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_MOS_ParamDynaObjectKnowledgeList.setMetaObject( metaObj );
    return metaObj;
}

void* MOS_ParamDynaObjectKnowledgeList::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "MOS_ParamDynaObjectKnowledgeList" ) )
	return this;
    if ( !qstrcmp( clname, "MOS_Param_ABC" ) )
	return (MOS_Param_ABC*)this;
    return MOS_ParamListView::qt_cast( clname );
}

bool MOS_ParamDynaObjectKnowledgeList::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: AcceptPopupMenuKnowledge(); break;
    case 1: OnObjectKnowledgeDeleted((MOS_Team&)*((MOS_Team*)static_QUType_ptr.get(_o+1)),(MOS_DynaObjectKnowledge&)*((MOS_DynaObjectKnowledge*)static_QUType_ptr.get(_o+2))); break;
    default:
	return MOS_ParamListView::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool MOS_ParamDynaObjectKnowledgeList::qt_emit( int _id, QUObject* _o )
{
    return MOS_ParamListView::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool MOS_ParamDynaObjectKnowledgeList::qt_property( int id, int f, QVariant* v)
{
    return MOS_ParamListView::qt_property( id, f, v);
}

bool MOS_ParamDynaObjectKnowledgeList::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
