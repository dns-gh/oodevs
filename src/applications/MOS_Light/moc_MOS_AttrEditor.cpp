/****************************************************************************
** MOS_AttrEditor meta object code from reading C++ file 'MOS_AttrEditor.h'
**
** Created: jeu. 21. juil. 13:57:07 2005
**      by: The Qt MOC ($Id: $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "MOS_AttrEditor.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *MOS_AttrEditor::className() const
{
    return "MOS_AttrEditor";
}

QMetaObject *MOS_AttrEditor::metaObj = 0;
static QMetaObjectCleanUp cleanUp_MOS_AttrEditor( "MOS_AttrEditor", &MOS_AttrEditor::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString MOS_AttrEditor::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_AttrEditor", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString MOS_AttrEditor::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_AttrEditor", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* MOS_AttrEditor::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QFrame::staticMetaObject();
    static const QUMethod slot_0 = {"SlotAccept", 0, 0 };
    static const QUMethod slot_1 = {"SlotReject", 0, 0 };
    static const QUMethod slot_2 = {"SlotClearItineraire", 0, 0 };
    static const QUMethod slot_3 = {"SlotOrder", 0, 0 };
    static const QUParameter param_slot_4[] = {
	{ "bValue", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_4 = {"SlotDrawKnowledge", 1, param_slot_4 };
    static const QUMethod slot_5 = {"SlotChangeAutomateMode", 0, 0 };
    static const QUMethod slot_6 = {"SlotMagicActionRecompletementTotal", 0, 0 };
    static const QUMethod slot_7 = {"SlotMagicActionRecompletementPersonnel", 0, 0 };
    static const QUMethod slot_8 = {"SlotMagicActionRecompletementEquipement", 0, 0 };
    static const QUMethod slot_9 = {"SlotMagicActionRecompletementRessources", 0, 0 };
    static const QUMethod slot_10 = {"SlotMagicActionDestroyComposante", 0, 0 };
    static const QUMethod slot_11 = {"SlotMagicActionChangeHumanFactors", 0, 0 };
    static const QUMethod slot_12 = {"SlotMagicActionChangeLogisticLinks", 0, 0 };
    static const QUMethod slot_13 = {"SlotMagicActionDestroyAllComposantes", 0, 0 };
    static const QUMethod slot_14 = {"SlotMagicActionLogisticChangeQuotas", 0, 0 };
    static const QUMethod slot_15 = {"SlotMagicActionLogisticPushFlow", 0, 0 };
    static const QUMethod slot_16 = {"SlotMagicActionSurrender", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "SlotAccept()", &slot_0, QMetaData::Protected },
	{ "SlotReject()", &slot_1, QMetaData::Protected },
	{ "SlotClearItineraire()", &slot_2, QMetaData::Protected },
	{ "SlotOrder()", &slot_3, QMetaData::Protected },
	{ "SlotDrawKnowledge(bool)", &slot_4, QMetaData::Protected },
	{ "SlotChangeAutomateMode()", &slot_5, QMetaData::Protected },
	{ "SlotMagicActionRecompletementTotal()", &slot_6, QMetaData::Protected },
	{ "SlotMagicActionRecompletementPersonnel()", &slot_7, QMetaData::Protected },
	{ "SlotMagicActionRecompletementEquipement()", &slot_8, QMetaData::Protected },
	{ "SlotMagicActionRecompletementRessources()", &slot_9, QMetaData::Protected },
	{ "SlotMagicActionDestroyComposante()", &slot_10, QMetaData::Protected },
	{ "SlotMagicActionChangeHumanFactors()", &slot_11, QMetaData::Protected },
	{ "SlotMagicActionChangeLogisticLinks()", &slot_12, QMetaData::Protected },
	{ "SlotMagicActionDestroyAllComposantes()", &slot_13, QMetaData::Protected },
	{ "SlotMagicActionLogisticChangeQuotas()", &slot_14, QMetaData::Protected },
	{ "SlotMagicActionLogisticPushFlow()", &slot_15, QMetaData::Protected },
	{ "SlotMagicActionSurrender()", &slot_16, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"MOS_AttrEditor", parentObject,
	slot_tbl, 17,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_MOS_AttrEditor.setMetaObject( metaObj );
    return metaObj;
}

void* MOS_AttrEditor::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "MOS_AttrEditor" ) )
	return this;
    return QFrame::qt_cast( clname );
}

bool MOS_AttrEditor::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: SlotAccept(); break;
    case 1: SlotReject(); break;
    case 2: SlotClearItineraire(); break;
    case 3: SlotOrder(); break;
    case 4: SlotDrawKnowledge((bool)static_QUType_bool.get(_o+1)); break;
    case 5: SlotChangeAutomateMode(); break;
    case 6: SlotMagicActionRecompletementTotal(); break;
    case 7: SlotMagicActionRecompletementPersonnel(); break;
    case 8: SlotMagicActionRecompletementEquipement(); break;
    case 9: SlotMagicActionRecompletementRessources(); break;
    case 10: SlotMagicActionDestroyComposante(); break;
    case 11: SlotMagicActionChangeHumanFactors(); break;
    case 12: SlotMagicActionChangeLogisticLinks(); break;
    case 13: SlotMagicActionDestroyAllComposantes(); break;
    case 14: SlotMagicActionLogisticChangeQuotas(); break;
    case 15: SlotMagicActionLogisticPushFlow(); break;
    case 16: SlotMagicActionSurrender(); break;
    default:
	return QFrame::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool MOS_AttrEditor::qt_emit( int _id, QUObject* _o )
{
    return QFrame::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool MOS_AttrEditor::qt_property( int id, int f, QVariant* v)
{
    return QFrame::qt_property( id, f, v);
}

bool MOS_AttrEditor::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
