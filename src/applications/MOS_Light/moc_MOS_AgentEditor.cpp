/****************************************************************************
** MOS_AgentEditor meta object code from reading C++ file 'MOS_AgentEditor.h'
**
** Created: jeu. 21. juil. 13:57:07 2005
**      by: The Qt MOC ($Id: $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "MOS_AgentEditor.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *MOS_AgentEditor::className() const
{
    return "MOS_AgentEditor";
}

QMetaObject *MOS_AgentEditor::metaObj = 0;
static QMetaObjectCleanUp cleanUp_MOS_AgentEditor( "MOS_AgentEditor", &MOS_AgentEditor::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString MOS_AgentEditor::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_AgentEditor", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString MOS_AgentEditor::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_AgentEditor", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* MOS_AgentEditor::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QWidget::staticMetaObject();
    static const QUMethod slot_0 = {"SlotChangeCheckPointFrequency", 0, 0 };
    static const QUMethod slot_1 = {"SlotCheckPointSaveNow", 0, 0 };
    static const QUMethod slot_2 = {"SlotChangeDiplomacy", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "SlotChangeCheckPointFrequency()", &slot_0, QMetaData::Public },
	{ "SlotCheckPointSaveNow()", &slot_1, QMetaData::Public },
	{ "SlotChangeDiplomacy()", &slot_2, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"MOS_AgentEditor", parentObject,
	slot_tbl, 3,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_MOS_AgentEditor.setMetaObject( metaObj );
    return metaObj;
}

void* MOS_AgentEditor::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "MOS_AgentEditor" ) )
	return this;
    return QWidget::qt_cast( clname );
}

bool MOS_AgentEditor::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: SlotChangeCheckPointFrequency(); break;
    case 1: SlotCheckPointSaveNow(); break;
    case 2: SlotChangeDiplomacy(); break;
    default:
	return QWidget::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool MOS_AgentEditor::qt_emit( int _id, QUObject* _o )
{
    return QWidget::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool MOS_AgentEditor::qt_property( int id, int f, QVariant* v)
{
    return QWidget::qt_property( id, f, v);
}

bool MOS_AgentEditor::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
