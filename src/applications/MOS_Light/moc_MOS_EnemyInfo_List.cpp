/****************************************************************************
** MOS_EnemyInfo_List meta object code from reading C++ file 'MOS_EnemyInfo_List.h'
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

const char *MOS_EnemyInfo_List::className() const
{
    return "MOS_EnemyInfo_List";
}

QMetaObject *MOS_EnemyInfo_List::metaObj = 0;
static QMetaObjectCleanUp cleanUp_MOS_EnemyInfo_List( "MOS_EnemyInfo_List", &MOS_EnemyInfo_List::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString MOS_EnemyInfo_List::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_EnemyInfo_List", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString MOS_EnemyInfo_List::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_EnemyInfo_List", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* MOS_EnemyInfo_List::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QListView::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "pItem", &static_QUType_ptr, "QListViewItem", QUParameter::In },
	{ "point", &static_QUType_varptr, "\x0e", QUParameter::In }
    };
    static const QUMethod slot_0 = {"SlotContextMenuRequested", 2, param_slot_0 };
    static const QUParameter param_slot_1[] = {
	{ "bNewVal", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_1 = {"SlotDrawRensEni", 1, param_slot_1 };
    static const QMetaData slot_tbl[] = {
	{ "SlotContextMenuRequested(QListViewItem*,const QPoint&)", &slot_0, QMetaData::Protected },
	{ "SlotDrawRensEni(bool)", &slot_1, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"MOS_EnemyInfo_List", parentObject,
	slot_tbl, 2,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_MOS_EnemyInfo_List.setMetaObject( metaObj );
    return metaObj;
}

void* MOS_EnemyInfo_List::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "MOS_EnemyInfo_List" ) )
	return this;
    return QListView::qt_cast( clname );
}

bool MOS_EnemyInfo_List::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: SlotContextMenuRequested((QListViewItem*)static_QUType_ptr.get(_o+1),(const QPoint&)*((const QPoint*)static_QUType_ptr.get(_o+2))); break;
    case 1: SlotDrawRensEni((bool)static_QUType_bool.get(_o+1)); break;
    default:
	return QListView::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool MOS_EnemyInfo_List::qt_emit( int _id, QUObject* _o )
{
    return QListView::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool MOS_EnemyInfo_List::qt_property( int id, int f, QVariant* v)
{
    return QListView::qt_property( id, f, v);
}

bool MOS_EnemyInfo_List::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
