/****************************************************************************
** MOS_AgentCargoPanel meta object code from reading C++ file 'MOS_AgentCargoPanel.h'
**
** Created: mar. 22. mars 18:32:10 2005
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

const char *MOS_AgentCargoPanel::className() const
{
    return "MOS_AgentCargoPanel";
}

QMetaObject *MOS_AgentCargoPanel::metaObj = 0;
static QMetaObjectCleanUp cleanUp_MOS_AgentCargoPanel( "MOS_AgentCargoPanel", &MOS_AgentCargoPanel::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString MOS_AgentCargoPanel::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_AgentCargoPanel", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString MOS_AgentCargoPanel::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_AgentCargoPanel", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* MOS_AgentCargoPanel::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QVBox::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "agent", &static_QUType_ptr, "MOS_Agent", QUParameter::InOut }
    };
    static const QUMethod slot_0 = {"OnAgentUpdated", 1, param_slot_0 };
    static const QMetaData slot_tbl[] = {
	{ "OnAgentUpdated(MOS_Agent&)", &slot_0, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"MOS_AgentCargoPanel", parentObject,
	slot_tbl, 1,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_MOS_AgentCargoPanel.setMetaObject( metaObj );
    return metaObj;
}

void* MOS_AgentCargoPanel::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "MOS_AgentCargoPanel" ) )
	return this;
    return QVBox::qt_cast( clname );
}

bool MOS_AgentCargoPanel::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: OnAgentUpdated((MOS_Agent&)*((MOS_Agent*)static_QUType_ptr.get(_o+1))); break;
    default:
	return QVBox::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool MOS_AgentCargoPanel::qt_emit( int _id, QUObject* _o )
{
    return QVBox::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool MOS_AgentCargoPanel::qt_property( int id, int f, QVariant* v)
{
    return QVBox::qt_property( id, f, v);
}

bool MOS_AgentCargoPanel::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
