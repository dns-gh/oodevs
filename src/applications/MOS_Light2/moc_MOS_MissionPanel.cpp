/****************************************************************************
** MOS_MissionPanel meta object code from reading C++ file 'MOS_MissionPanel.h'
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

const char *MOS_MissionPanel::className() const
{
    return "MOS_MissionPanel";
}

QMetaObject *MOS_MissionPanel::metaObj = 0;
static QMetaObjectCleanUp cleanUp_MOS_MissionPanel( "MOS_MissionPanel", &MOS_MissionPanel::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString MOS_MissionPanel::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_MissionPanel", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString MOS_MissionPanel::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_MissionPanel", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* MOS_MissionPanel::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QDockWindow::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "popupMenu", &static_QUType_ptr, "QPopupMenu", QUParameter::InOut },
	{ "context", &static_QUType_ptr, "MOS_ActionContext", QUParameter::In }
    };
    static const QUMethod slot_0 = {"FillRemotePopupMenu", 2, param_slot_0 };
    static const QUMethod slot_1 = {"ToggleAutomate", 0, 0 };
    static const QUParameter param_slot_2[] = {
	{ "nMissionId", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_2 = {"ActivateUnitMission", 1, param_slot_2 };
    static const QUParameter param_slot_3[] = {
	{ "nMissionId", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_3 = {"ActivateAutomataMission", 1, param_slot_3 };
    static const QUParameter param_slot_4[] = {
	{ "nOrderId", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_4 = {"ActivateFragmentaryOrder", 1, param_slot_4 };
    static const QUMethod slot_5 = {"MagicMove", 0, 0 };
    static const QUMethod slot_6 = {"MagicMoveDone", 0, 0 };
    static const QUParameter param_slot_7[] = {
	{ "nId", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_7 = {"MagicRestore", 1, param_slot_7 };
    static const QUMethod slot_8 = {"MagicDestroyComponent", 0, 0 };
    static const QUMethod slot_9 = {"MagicSurrender", 0, 0 };
    static const QUMethod slot_10 = {"MagicChangeDiplomatie", 0, 0 };
    static const QUMethod slot_11 = {"MagicChangeLogisticLinks", 0, 0 };
    static const QUMethod slot_12 = {"MagicPushFlux", 0, 0 };
    static const QUMethod slot_13 = {"MagicChangeQuotas", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "FillRemotePopupMenu(QPopupMenu&,const MOS_ActionContext&)", &slot_0, QMetaData::Public },
	{ "ToggleAutomate()", &slot_1, QMetaData::Private },
	{ "ActivateUnitMission(int)", &slot_2, QMetaData::Private },
	{ "ActivateAutomataMission(int)", &slot_3, QMetaData::Private },
	{ "ActivateFragmentaryOrder(int)", &slot_4, QMetaData::Private },
	{ "MagicMove()", &slot_5, QMetaData::Private },
	{ "MagicMoveDone()", &slot_6, QMetaData::Private },
	{ "MagicRestore(int)", &slot_7, QMetaData::Private },
	{ "MagicDestroyComponent()", &slot_8, QMetaData::Private },
	{ "MagicSurrender()", &slot_9, QMetaData::Private },
	{ "MagicChangeDiplomatie()", &slot_10, QMetaData::Private },
	{ "MagicChangeLogisticLinks()", &slot_11, QMetaData::Private },
	{ "MagicPushFlux()", &slot_12, QMetaData::Private },
	{ "MagicChangeQuotas()", &slot_13, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"MOS_MissionPanel", parentObject,
	slot_tbl, 14,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_MOS_MissionPanel.setMetaObject( metaObj );
    return metaObj;
}

void* MOS_MissionPanel::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "MOS_MissionPanel" ) )
	return this;
    return QDockWindow::qt_cast( clname );
}

bool MOS_MissionPanel::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: FillRemotePopupMenu((QPopupMenu&)*((QPopupMenu*)static_QUType_ptr.get(_o+1)),(const MOS_ActionContext&)*((const MOS_ActionContext*)static_QUType_ptr.get(_o+2))); break;
    case 1: ToggleAutomate(); break;
    case 2: ActivateUnitMission((int)static_QUType_int.get(_o+1)); break;
    case 3: ActivateAutomataMission((int)static_QUType_int.get(_o+1)); break;
    case 4: ActivateFragmentaryOrder((int)static_QUType_int.get(_o+1)); break;
    case 5: MagicMove(); break;
    case 6: MagicMoveDone(); break;
    case 7: MagicRestore((int)static_QUType_int.get(_o+1)); break;
    case 8: MagicDestroyComponent(); break;
    case 9: MagicSurrender(); break;
    case 10: MagicChangeDiplomatie(); break;
    case 11: MagicChangeLogisticLinks(); break;
    case 12: MagicPushFlux(); break;
    case 13: MagicChangeQuotas(); break;
    default:
	return QDockWindow::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool MOS_MissionPanel::qt_emit( int _id, QUObject* _o )
{
    return QDockWindow::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool MOS_MissionPanel::qt_property( int id, int f, QVariant* v)
{
    return QDockWindow::qt_property( id, f, v);
}

bool MOS_MissionPanel::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
