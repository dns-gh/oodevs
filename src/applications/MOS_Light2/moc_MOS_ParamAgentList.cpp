/****************************************************************************
** MOS_ParamAgentList meta object code from reading C++ file 'MOS_ParamAgentList.h'
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

const char *MOS_ParamAgentList::className() const
{
    return "MOS_ParamAgentList";
}

QMetaObject *MOS_ParamAgentList::metaObj = 0;
static QMetaObjectCleanUp cleanUp_MOS_ParamAgentList( "MOS_ParamAgentList", &MOS_ParamAgentList::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString MOS_ParamAgentList::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_ParamAgentList", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString MOS_ParamAgentList::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_ParamAgentList", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* MOS_ParamAgentList::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = MOS_ParamListView::staticMetaObject();
    static const QUMethod slot_0 = {"AcceptPopupMenuAgent", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "AcceptPopupMenuAgent()", &slot_0, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"MOS_ParamAgentList", parentObject,
	slot_tbl, 1,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_MOS_ParamAgentList.setMetaObject( metaObj );
    return metaObj;
}

void* MOS_ParamAgentList::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "MOS_ParamAgentList" ) )
	return this;
    if ( !qstrcmp( clname, "MOS_Param_ABC" ) )
	return (MOS_Param_ABC*)this;
    return MOS_ParamListView::qt_cast( clname );
}

bool MOS_ParamAgentList::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: AcceptPopupMenuAgent(); break;
    default:
	return MOS_ParamListView::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool MOS_ParamAgentList::qt_emit( int _id, QUObject* _o )
{
    return MOS_ParamListView::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool MOS_ParamAgentList::qt_property( int id, int f, QVariant* v)
{
    return MOS_ParamListView::qt_property( id, f, v);
}

bool MOS_ParamAgentList::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
