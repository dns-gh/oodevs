/****************************************************************************
** MOS_AgentListView meta object code from reading C++ file 'MOS_AgentListView.h'
**
** Created: lun. 11. juil. 18:50:10 2005
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

const char *MOS_AgentListView::className() const
{
    return "MOS_AgentListView";
}

QMetaObject *MOS_AgentListView::metaObj = 0;
static QMetaObjectCleanUp cleanUp_MOS_AgentListView( "MOS_AgentListView", &MOS_AgentListView::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString MOS_AgentListView::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_AgentListView", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString MOS_AgentListView::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_AgentListView", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* MOS_AgentListView::staticMetaObject()
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
	{ "bFollow", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_2 = {"SlotSetFollow", 1, param_slot_2 };
    static const QUParameter param_slot_3[] = {
	{ "bItineraire", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_3 = {"SlotSetItineraire", 1, param_slot_3 };
    static const QUParameter param_slot_4[] = {
	{ "bAllItineraire", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_4 = {"SlotSetAllItineraire", 1, param_slot_4 };
    static const QUParameter param_slot_5[] = {
	{ "bOptiDraw", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_5 = {"SlotSetOptiDraw", 1, param_slot_5 };
    static const QUParameter param_slot_6[] = {
	{ "bDrawAltitude", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_6 = {"SlotSetDrawAltitude", 1, param_slot_6 };
    static const QUParameter param_slot_7[] = {
	{ "bAllLimit", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_7 = {"SlotSetDrawLimit", 1, param_slot_7 };
    static const QUParameter param_slot_8[] = {
	{ "bAllLima", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_8 = {"SlotSetDrawLima", 1, param_slot_8 };
    static const QUParameter param_slot_9[] = {
	{ "bAllVision", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_9 = {"SlotSetAllVision", 1, param_slot_9 };
    static const QUParameter param_slot_10[] = {
	{ "bAllCone", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_10 = {"SlotSetAllCone", 1, param_slot_10 };
    static const QUParameter param_slot_11[] = {
	{ "bValue", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_11 = {"SlotSetPointEdition", 1, param_slot_11 };
    static const QUParameter param_slot_12[] = {
	{ "bDrawOldPath", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_12 = {"SlotSetDrawOldPath", 1, param_slot_12 };
    static const QUParameter param_slot_13[] = {
	{ "bDrawText", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_13 = {"SlotSetDrawText", 1, param_slot_13 };
    static const QUParameter param_slot_14[] = {
	{ "bScrollTrace", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_14 = {"SlotSetScrollTrace", 1, param_slot_14 };
    static const QUParameter param_slot_15[] = {
	{ "bEnabled", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_15 = {"SlotEnableProfiling", 1, param_slot_15 };
    static const QMetaData slot_tbl[] = {
	{ "SlotContextMenuRequested(QListViewItem*,const QPoint&)", &slot_0, QMetaData::Protected },
	{ "SlotItemPressed(QListViewItem*)", &slot_1, QMetaData::Protected },
	{ "SlotSetFollow(bool)", &slot_2, QMetaData::Protected },
	{ "SlotSetItineraire(bool)", &slot_3, QMetaData::Protected },
	{ "SlotSetAllItineraire(bool)", &slot_4, QMetaData::Protected },
	{ "SlotSetOptiDraw(bool)", &slot_5, QMetaData::Protected },
	{ "SlotSetDrawAltitude(bool)", &slot_6, QMetaData::Protected },
	{ "SlotSetDrawLimit(bool)", &slot_7, QMetaData::Protected },
	{ "SlotSetDrawLima(bool)", &slot_8, QMetaData::Protected },
	{ "SlotSetAllVision(bool)", &slot_9, QMetaData::Protected },
	{ "SlotSetAllCone(bool)", &slot_10, QMetaData::Protected },
	{ "SlotSetPointEdition(bool)", &slot_11, QMetaData::Protected },
	{ "SlotSetDrawOldPath(bool)", &slot_12, QMetaData::Protected },
	{ "SlotSetDrawText(bool)", &slot_13, QMetaData::Protected },
	{ "SlotSetScrollTrace(bool)", &slot_14, QMetaData::Protected },
	{ "SlotEnableProfiling(bool)", &slot_15, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"MOS_AgentListView", parentObject,
	slot_tbl, 16,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_MOS_AgentListView.setMetaObject( metaObj );
    return metaObj;
}

void* MOS_AgentListView::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "MOS_AgentListView" ) )
	return this;
    if ( !qstrcmp( clname, "MOS_GLMouse_ABC" ) )
	return (MOS_GLMouse_ABC*)this;
    return QListView::qt_cast( clname );
}

bool MOS_AgentListView::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: SlotContextMenuRequested((QListViewItem*)static_QUType_ptr.get(_o+1),(const QPoint&)*((const QPoint*)static_QUType_ptr.get(_o+2))); break;
    case 1: SlotItemPressed((QListViewItem*)static_QUType_ptr.get(_o+1)); break;
    case 2: SlotSetFollow((bool)static_QUType_bool.get(_o+1)); break;
    case 3: SlotSetItineraire((bool)static_QUType_bool.get(_o+1)); break;
    case 4: SlotSetAllItineraire((bool)static_QUType_bool.get(_o+1)); break;
    case 5: SlotSetOptiDraw((bool)static_QUType_bool.get(_o+1)); break;
    case 6: SlotSetDrawAltitude((bool)static_QUType_bool.get(_o+1)); break;
    case 7: SlotSetDrawLimit((bool)static_QUType_bool.get(_o+1)); break;
    case 8: SlotSetDrawLima((bool)static_QUType_bool.get(_o+1)); break;
    case 9: SlotSetAllVision((bool)static_QUType_bool.get(_o+1)); break;
    case 10: SlotSetAllCone((bool)static_QUType_bool.get(_o+1)); break;
    case 11: SlotSetPointEdition((bool)static_QUType_bool.get(_o+1)); break;
    case 12: SlotSetDrawOldPath((bool)static_QUType_bool.get(_o+1)); break;
    case 13: SlotSetDrawText((bool)static_QUType_bool.get(_o+1)); break;
    case 14: SlotSetScrollTrace((bool)static_QUType_bool.get(_o+1)); break;
    case 15: SlotEnableProfiling((bool)static_QUType_bool.get(_o+1)); break;
    default:
	return QListView::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool MOS_AgentListView::qt_emit( int _id, QUObject* _o )
{
    return QListView::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool MOS_AgentListView::qt_property( int id, int f, QVariant* v)
{
    return QListView::qt_property( id, f, v);
}

bool MOS_AgentListView::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
