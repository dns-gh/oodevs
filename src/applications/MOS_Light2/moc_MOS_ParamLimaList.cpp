/****************************************************************************
** MOS_ParamLimaList meta object code from reading C++ file 'MOS_ParamLimaList.h'
**
** Created: jeu. 21. juil. 15:28:03 2005
**      by: The Qt MOC ($Id: $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "MOS_ParamLimaList.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *MOS_ParamLimaList::className() const
{
    return "MOS_ParamLimaList";
}

QMetaObject *MOS_ParamLimaList::metaObj = 0;
static QMetaObjectCleanUp cleanUp_MOS_ParamLimaList( "MOS_ParamLimaList", &MOS_ParamLimaList::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString MOS_ParamLimaList::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_ParamLimaList", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString MOS_ParamLimaList::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_ParamLimaList", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* MOS_ParamLimaList::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = MOS_ParamListView::staticMetaObject();
    static const QUMethod slot_0 = {"AcceptPopupMenuLima", 0, 0 };
    static const QUParameter param_slot_1[] = {
	{ "line", &static_QUType_ptr, "MOS_TacticalLine_ABC", QUParameter::InOut }
    };
    static const QUMethod slot_1 = {"OnTacticalLineDeleted", 1, param_slot_1 };
    static const QMetaData slot_tbl[] = {
	{ "AcceptPopupMenuLima()", &slot_0, QMetaData::Private },
	{ "OnTacticalLineDeleted(MOS_TacticalLine_ABC&)", &slot_1, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"MOS_ParamLimaList", parentObject,
	slot_tbl, 2,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_MOS_ParamLimaList.setMetaObject( metaObj );
    return metaObj;
}

void* MOS_ParamLimaList::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "MOS_ParamLimaList" ) )
	return this;
    if ( !qstrcmp( clname, "MOS_Param_ABC" ) )
	return (MOS_Param_ABC*)this;
    return MOS_ParamListView::qt_cast( clname );
}

bool MOS_ParamLimaList::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: AcceptPopupMenuLima(); break;
    case 1: OnTacticalLineDeleted((MOS_TacticalLine_ABC&)*((MOS_TacticalLine_ABC*)static_QUType_ptr.get(_o+1))); break;
    default:
	return MOS_ParamListView::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool MOS_ParamLimaList::qt_emit( int _id, QUObject* _o )
{
    return MOS_ParamListView::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool MOS_ParamLimaList::qt_property( int id, int f, QVariant* v)
{
    return MOS_ParamListView::qt_property( id, f, v);
}

bool MOS_ParamLimaList::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
