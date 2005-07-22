/****************************************************************************
** MOS_AgentCreationMapFilter meta object code from reading C++ file 'MOS_AgentCreationMapFilter.h'
**
** Created: jeu. 21. juil. 15:28:03 2005
**      by: The Qt MOC ($Id: $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "MOS_AgentCreationMapFilter.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *MOS_AgentCreationMapFilter::className() const
{
    return "MOS_AgentCreationMapFilter";
}

QMetaObject *MOS_AgentCreationMapFilter::metaObj = 0;
static QMetaObjectCleanUp cleanUp_MOS_AgentCreationMapFilter( "MOS_AgentCreationMapFilter", &MOS_AgentCreationMapFilter::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString MOS_AgentCreationMapFilter::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_AgentCreationMapFilter", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString MOS_AgentCreationMapFilter::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_AgentCreationMapFilter", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* MOS_AgentCreationMapFilter::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QObject::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "selectedElement", &static_QUType_ptr, "MOS_SelectedElement", QUParameter::InOut }
    };
    static const QUMethod slot_0 = {"SetSelectedElement", 1, param_slot_0 };
    static const QMetaData slot_tbl[] = {
	{ "SetSelectedElement(MOS_SelectedElement&)", &slot_0, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"MOS_AgentCreationMapFilter", parentObject,
	slot_tbl, 1,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_MOS_AgentCreationMapFilter.setMetaObject( metaObj );
    return metaObj;
}

void* MOS_AgentCreationMapFilter::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "MOS_AgentCreationMapFilter" ) )
	return this;
    if ( !qstrcmp( clname, "MOS_MapEventFilter_ABC" ) )
	return (MOS_MapEventFilter_ABC*)this;
    return QObject::qt_cast( clname );
}

bool MOS_AgentCreationMapFilter::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: SetSelectedElement((MOS_SelectedElement&)*((MOS_SelectedElement*)static_QUType_ptr.get(_o+1))); break;
    default:
	return QObject::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool MOS_AgentCreationMapFilter::qt_emit( int _id, QUObject* _o )
{
    return QObject::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool MOS_AgentCreationMapFilter::qt_property( int id, int f, QVariant* v)
{
    return QObject::qt_property( id, f, v);
}

bool MOS_AgentCreationMapFilter::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
