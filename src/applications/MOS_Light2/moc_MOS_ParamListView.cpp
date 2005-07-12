/****************************************************************************
** MOS_ParamListView meta object code from reading C++ file 'MOS_ParamListView.h'
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

const char *MOS_ParamListView::className() const
{
    return "MOS_ParamListView";
}

QMetaObject *MOS_ParamListView::metaObj = 0;
static QMetaObjectCleanUp cleanUp_MOS_ParamListView( "MOS_ParamListView", &MOS_ParamListView::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString MOS_ParamListView::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_ParamListView", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString MOS_ParamListView::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_ParamListView", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* MOS_ParamListView::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QListView::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "pItem", &static_QUType_ptr, "QListViewItem", QUParameter::In },
	{ "pos", &static_QUType_varptr, "\x0e", QUParameter::In }
    };
    static const QUMethod slot_0 = {"OnRequestPopup", 2, param_slot_0 };
    static const QUMethod slot_1 = {"OnDeleteSelectedItem", 0, 0 };
    static const QUMethod slot_2 = {"OnClearList", 0, 0 };
    static const QUMethod slot_3 = {"TurnHeaderBlack", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "OnRequestPopup(QListViewItem*,const QPoint&)", &slot_0, QMetaData::Private },
	{ "OnDeleteSelectedItem()", &slot_1, QMetaData::Private },
	{ "OnClearList()", &slot_2, QMetaData::Private },
	{ "TurnHeaderBlack()", &slot_3, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"MOS_ParamListView", parentObject,
	slot_tbl, 4,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_MOS_ParamListView.setMetaObject( metaObj );
    return metaObj;
}

void* MOS_ParamListView::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "MOS_ParamListView" ) )
	return this;
    return QListView::qt_cast( clname );
}

bool MOS_ParamListView::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: OnRequestPopup((QListViewItem*)static_QUType_ptr.get(_o+1),(const QPoint&)*((const QPoint*)static_QUType_ptr.get(_o+2))); break;
    case 1: OnDeleteSelectedItem(); break;
    case 2: OnClearList(); break;
    case 3: TurnHeaderBlack(); break;
    default:
	return QListView::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool MOS_ParamListView::qt_emit( int _id, QUObject* _o )
{
    return QListView::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool MOS_ParamListView::qt_property( int id, int f, QVariant* v)
{
    return QListView::qt_property( id, f, v);
}

bool MOS_ParamListView::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
