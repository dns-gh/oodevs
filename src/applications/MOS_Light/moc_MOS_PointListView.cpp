/****************************************************************************
** MOS_PointListView meta object code from reading C++ file 'MOS_PointListView.h'
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

const char *MOS_PointListView::className() const
{
    return "MOS_PointListView";
}

QMetaObject *MOS_PointListView::metaObj = 0;
static QMetaObjectCleanUp cleanUp_MOS_PointListView( "MOS_PointListView", &MOS_PointListView::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString MOS_PointListView::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_PointListView", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString MOS_PointListView::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_PointListView", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* MOS_PointListView::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QListView::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "pItem", &static_QUType_ptr, "QListViewItem", QUParameter::In }
    };
    static const QUMethod slot_0 = {"SlotItemPressed", 1, param_slot_0 };
    static const QUParameter param_slot_1[] = {
	{ "pItem", &static_QUType_ptr, "QListViewItem", QUParameter::In },
	{ "point", &static_QUType_varptr, "\x0e", QUParameter::In }
    };
    static const QUMethod slot_1 = {"SlotContextMenuRequested", 2, param_slot_1 };
    static const QMetaData slot_tbl[] = {
	{ "SlotItemPressed(QListViewItem*)", &slot_0, QMetaData::Protected },
	{ "SlotContextMenuRequested(QListViewItem*,const QPoint&)", &slot_1, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"MOS_PointListView", parentObject,
	slot_tbl, 2,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_MOS_PointListView.setMetaObject( metaObj );
    return metaObj;
}

void* MOS_PointListView::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "MOS_PointListView" ) )
	return this;
    if ( !qstrcmp( clname, "MOS_GLMouse_ABC" ) )
	return (MOS_GLMouse_ABC*)this;
    if ( !qstrcmp( clname, "MOS_MissionTool_ABC" ) )
	return (MOS_MissionTool_ABC*)this;
    return QListView::qt_cast( clname );
}

bool MOS_PointListView::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: SlotItemPressed((QListViewItem*)static_QUType_ptr.get(_o+1)); break;
    case 1: SlotContextMenuRequested((QListViewItem*)static_QUType_ptr.get(_o+1),(const QPoint&)*((const QPoint*)static_QUType_ptr.get(_o+2))); break;
    default:
	return QListView::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool MOS_PointListView::qt_emit( int _id, QUObject* _o )
{
    return QListView::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool MOS_PointListView::qt_property( int id, int f, QVariant* v)
{
    return QListView::qt_property( id, f, v);
}

bool MOS_PointListView::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
