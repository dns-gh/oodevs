/****************************************************************************
** ADN_ListView meta object code from reading C++ file 'ADN_ListView.h'
**
** Created: jeu. 21. juil. 11:15:29 2005
**      by: The Qt MOC ($Id: $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "ADN_ListView.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *ADN_ListView::className() const
{
    return "ADN_ListView";
}

QMetaObject *ADN_ListView::metaObj = 0;
static QMetaObjectCleanUp cleanUp_ADN_ListView( "ADN_ListView", &ADN_ListView::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString ADN_ListView::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ADN_ListView", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString ADN_ListView::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ADN_ListView", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* ADN_ListView::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QListView::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "pData", &static_QUType_ptr, "void", QUParameter::In }
    };
    static const QUMethod slot_0 = {"SetCurrentItem", 1, param_slot_0 };
    static const QUMethod slot_1 = {"ContextMenuNew", 0, 0 };
    static const QUMethod slot_2 = {"ContextMenuDelete", 0, 0 };
    static const QUParameter param_slot_3[] = {
	{ "pItem", &static_QUType_ptr, "QListViewItem", QUParameter::In }
    };
    static const QUMethod slot_3 = {"SetCurrentItem", 1, param_slot_3 };
    static const QUParameter param_slot_4[] = {
	{ "pItem", &static_QUType_ptr, "QListViewItem", QUParameter::In },
	{ "pt", &static_QUType_varptr, "\x0e", QUParameter::In },
	{ "nCol", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_4 = {"OnContextMenuRequested", 3, param_slot_4 };
    static const QUMethod slot_5 = {"UpdateEnableState", 0, 0 };
    static const QUParameter param_slot_6[] = {
	{ "pItem", &static_QUType_ptr, "QListViewItem", QUParameter::In }
    };
    static const QUMethod slot_6 = {"OnOnItem", 1, param_slot_6 };
    static const QMetaData slot_tbl[] = {
	{ "SetCurrentItem(void*)", &slot_0, QMetaData::Public },
	{ "ContextMenuNew()", &slot_1, QMetaData::Private },
	{ "ContextMenuDelete()", &slot_2, QMetaData::Private },
	{ "SetCurrentItem(QListViewItem*)", &slot_3, QMetaData::Private },
	{ "OnContextMenuRequested(QListViewItem*,const QPoint&,int)", &slot_4, QMetaData::Private },
	{ "UpdateEnableState()", &slot_5, QMetaData::Private },
	{ "OnOnItem(QListViewItem*)", &slot_6, QMetaData::Private }
    };
    static const QUParameter param_signal_0[] = {
	{ "pData", &static_QUType_ptr, "void", QUParameter::In }
    };
    static const QUMethod signal_0 = {"ItemSelected", 1, param_signal_0 };
    static const QMetaData signal_tbl[] = {
	{ "ItemSelected(void*)", &signal_0, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"ADN_ListView", parentObject,
	slot_tbl, 7,
	signal_tbl, 1,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_ADN_ListView.setMetaObject( metaObj );
    return metaObj;
}

void* ADN_ListView::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "ADN_ListView" ) )
	return this;
    if ( !qstrcmp( clname, "ADN_Gfx_ABC" ) )
	return (ADN_Gfx_ABC*)this;
    return QListView::qt_cast( clname );
}

#include <qobjectdefs.h>
#include <qsignalslotimp.h>

// SIGNAL ItemSelected
void ADN_ListView::ItemSelected( void* t0 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 0 );
    if ( !clist )
	return;
    QUObject o[2];
    static_QUType_ptr.set(o+1,t0);
    activate_signal( clist, o );
}

bool ADN_ListView::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: SetCurrentItem((void*)static_QUType_ptr.get(_o+1)); break;
    case 1: ContextMenuNew(); break;
    case 2: ContextMenuDelete(); break;
    case 3: SetCurrentItem((QListViewItem*)static_QUType_ptr.get(_o+1)); break;
    case 4: OnContextMenuRequested((QListViewItem*)static_QUType_ptr.get(_o+1),(const QPoint&)*((const QPoint*)static_QUType_ptr.get(_o+2)),(int)static_QUType_int.get(_o+3)); break;
    case 5: UpdateEnableState(); break;
    case 6: OnOnItem((QListViewItem*)static_QUType_ptr.get(_o+1)); break;
    default:
	return QListView::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool ADN_ListView::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: ItemSelected((void*)static_QUType_ptr.get(_o+1)); break;
    default:
	return QListView::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool ADN_ListView::qt_property( int id, int f, QVariant* v)
{
    return QListView::qt_property( id, f, v);
}

bool ADN_ListView::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
