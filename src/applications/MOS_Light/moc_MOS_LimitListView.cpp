/****************************************************************************
** MOS_LimitListView meta object code from reading C++ file 'MOS_LimitListView.h'
**
** Created: jeu. 21. juil. 13:57:41 2005
**      by: The Qt MOC ($Id: $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "MOS_LimitListView.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *MOS_LimitListView::className() const
{
    return "MOS_LimitListView";
}

QMetaObject *MOS_LimitListView::metaObj = 0;
static QMetaObjectCleanUp cleanUp_MOS_LimitListView( "MOS_LimitListView", &MOS_LimitListView::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString MOS_LimitListView::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_LimitListView", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString MOS_LimitListView::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_LimitListView", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* MOS_LimitListView::staticMetaObject()
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
	{ "pItem", &static_QUType_ptr, "QListViewItem", QUParameter::In }
    };
    static const QUMethod slot_1 = {"SlotItemPressed", 1, param_slot_1 };
    static const QUParameter param_slot_2[] = {
	{ "pItem", &static_QUType_ptr, "QListViewItem", QUParameter::In }
    };
    static const QUMethod slot_2 = {"SlotItemDoubleClicked", 1, param_slot_2 };
    static const QUParameter param_slot_3[] = {
	{ 0, &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_3 = {"SlotSetShowAll", 1, param_slot_3 };
    static const QUParameter param_slot_4[] = {
	{ "pItem", &static_QUType_ptr, "QListViewItem", QUParameter::In },
	{ "nCol", &static_QUType_int, 0, QUParameter::In },
	{ "sText", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_4 = {"SlotItemRenamed", 3, param_slot_4 };
    static const QUMethod slot_5 = {"EnterLimitEditor", 0, 0 };
    static const QUMethod slot_6 = {"EnterLimitSelector", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "SlotContextMenuRequested(QListViewItem*,const QPoint&)", &slot_0, QMetaData::Protected },
	{ "SlotItemPressed(QListViewItem*)", &slot_1, QMetaData::Protected },
	{ "SlotItemDoubleClicked(QListViewItem*)", &slot_2, QMetaData::Protected },
	{ "SlotSetShowAll(bool)", &slot_3, QMetaData::Protected },
	{ "SlotItemRenamed(QListViewItem*,int,const QString&)", &slot_4, QMetaData::Protected },
	{ "EnterLimitEditor()", &slot_5, QMetaData::Protected },
	{ "EnterLimitSelector()", &slot_6, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"MOS_LimitListView", parentObject,
	slot_tbl, 7,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_MOS_LimitListView.setMetaObject( metaObj );
    return metaObj;
}

void* MOS_LimitListView::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "MOS_LimitListView" ) )
	return this;
    if ( !qstrcmp( clname, "MOS_GLMouse_ABC" ) )
	return (MOS_GLMouse_ABC*)this;
    return QListView::qt_cast( clname );
}

bool MOS_LimitListView::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: SlotContextMenuRequested((QListViewItem*)static_QUType_ptr.get(_o+1),(const QPoint&)*((const QPoint*)static_QUType_ptr.get(_o+2))); break;
    case 1: SlotItemPressed((QListViewItem*)static_QUType_ptr.get(_o+1)); break;
    case 2: SlotItemDoubleClicked((QListViewItem*)static_QUType_ptr.get(_o+1)); break;
    case 3: SlotSetShowAll((bool)static_QUType_bool.get(_o+1)); break;
    case 4: SlotItemRenamed((QListViewItem*)static_QUType_ptr.get(_o+1),(int)static_QUType_int.get(_o+2),(const QString&)static_QUType_QString.get(_o+3)); break;
    case 5: EnterLimitEditor(); break;
    case 6: EnterLimitSelector(); break;
    default:
	return QListView::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool MOS_LimitListView::qt_emit( int _id, QUObject* _o )
{
    return QListView::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool MOS_LimitListView::qt_property( int id, int f, QVariant* v)
{
    return QListView::qt_property( id, f, v);
}

bool MOS_LimitListView::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
