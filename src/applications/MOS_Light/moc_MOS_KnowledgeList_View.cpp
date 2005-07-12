/****************************************************************************
** MOS_KnowledgeList_View meta object code from reading C++ file 'MOS_KnowledgeList_View.h'
**
** Created: lun. 11. juil. 18:50:11 2005
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

const char *MOS_KnowledgeList_View::className() const
{
    return "MOS_KnowledgeList_View";
}

QMetaObject *MOS_KnowledgeList_View::metaObj = 0;
static QMetaObjectCleanUp cleanUp_MOS_KnowledgeList_View( "MOS_KnowledgeList_View", &MOS_KnowledgeList_View::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString MOS_KnowledgeList_View::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_KnowledgeList_View", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString MOS_KnowledgeList_View::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_KnowledgeList_View", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* MOS_KnowledgeList_View::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QListView::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "pItem", &static_QUType_ptr, "QListViewItem", QUParameter::In }
    };
    static const QUMethod slot_0 = {"OnClick", 1, param_slot_0 };
    static const QUParameter param_slot_1[] = {
	{ "pItem", &static_QUType_ptr, "QListViewItem", QUParameter::In }
    };
    static const QUMethod slot_1 = {"SlotDoubleClicked", 1, param_slot_1 };
    static const QMetaData slot_tbl[] = {
	{ "OnClick(QListViewItem*)", &slot_0, QMetaData::Private },
	{ "SlotDoubleClicked(QListViewItem*)", &slot_1, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"MOS_KnowledgeList_View", parentObject,
	slot_tbl, 2,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_MOS_KnowledgeList_View.setMetaObject( metaObj );
    return metaObj;
}

void* MOS_KnowledgeList_View::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "MOS_KnowledgeList_View" ) )
	return this;
    return QListView::qt_cast( clname );
}

bool MOS_KnowledgeList_View::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: OnClick((QListViewItem*)static_QUType_ptr.get(_o+1)); break;
    case 1: SlotDoubleClicked((QListViewItem*)static_QUType_ptr.get(_o+1)); break;
    default:
	return QListView::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool MOS_KnowledgeList_View::qt_emit( int _id, QUObject* _o )
{
    return QListView::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool MOS_KnowledgeList_View::qt_property( int id, int f, QVariant* v)
{
    return QListView::qt_property( id, f, v);
}

bool MOS_KnowledgeList_View::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
