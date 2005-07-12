/****************************************************************************
** MT_GLDrawer meta object code from reading C++ file 'MT_GLDrawer.h'
**
** Created: mar. 12. juil. 12:42:03 2005
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

const char *MT_GLDrawer::className() const
{
    return "MT_GLDrawer";
}

QMetaObject *MT_GLDrawer::metaObj = 0;
static QMetaObjectCleanUp cleanUp_MT_GLDrawer( "MT_GLDrawer", &MT_GLDrawer::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString MT_GLDrawer::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MT_GLDrawer", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString MT_GLDrawer::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MT_GLDrawer", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* MT_GLDrawer::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QObject::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "bOn", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_0 = {"SetActive", 1, param_slot_0 };
    static const QUMethod slot_1 = {"NewLayer", 0, 0 };
    static const QUMethod slot_2 = {"DeleteCurrentLayer", 0, 0 };
    static const QUParameter param_slot_3[] = {
	{ "pLayer", &static_QUType_ptr, "MT_GLDLayer", QUParameter::In }
    };
    static const QUMethod slot_3 = {"SetCurrentLayer", 1, param_slot_3 };
    static const QUMethod slot_4 = {"OnComboBoxChange", 0, 0 };
    static const QUParameter param_slot_5[] = {
	{ "layer", &static_QUType_ptr, "MT_GLDLayer", QUParameter::InOut }
    };
    static const QUMethod slot_5 = {"UpdateLayerInCombo", 1, param_slot_5 };
    static const QUParameter param_slot_6[] = {
	{ "nAction", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_6 = {"OnShapeAction", 1, param_slot_6 };
    static const QUMethod slot_7 = {"CancelCreation", 0, 0 };
    static const QUMethod slot_8 = {"ValidateCreation", 0, 0 };
    static const QUParameter param_slot_9[] = {
	{ "nLayer", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_9 = {"MoveSelectionToLayer", 1, param_slot_9 };
    static const QUParameter param_slot_10[] = {
	{ "bVisible", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_10 = {"SetCurrentLayerVisible", 1, param_slot_10 };
    static const QUMethod slot_11 = {"GroupSelection", 0, 0 };
    static const QUMethod slot_12 = {"DisgroupSelection", 0, 0 };
    static const QUMethod slot_13 = {"Load", 0, 0 };
    static const QUMethod slot_14 = {"Save", 0, 0 };
    static const QUMethod slot_15 = {"SaveAll", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "SetActive(bool)", &slot_0, QMetaData::Public },
	{ "NewLayer()", &slot_1, QMetaData::Public },
	{ "DeleteCurrentLayer()", &slot_2, QMetaData::Public },
	{ "SetCurrentLayer(MT_GLDLayer*)", &slot_3, QMetaData::Public },
	{ "OnComboBoxChange()", &slot_4, QMetaData::Public },
	{ "UpdateLayerInCombo(MT_GLDLayer&)", &slot_5, QMetaData::Public },
	{ "OnShapeAction(int)", &slot_6, QMetaData::Public },
	{ "CancelCreation()", &slot_7, QMetaData::Public },
	{ "ValidateCreation()", &slot_8, QMetaData::Public },
	{ "MoveSelectionToLayer(int)", &slot_9, QMetaData::Public },
	{ "SetCurrentLayerVisible(bool)", &slot_10, QMetaData::Public },
	{ "GroupSelection()", &slot_11, QMetaData::Public },
	{ "DisgroupSelection()", &slot_12, QMetaData::Public },
	{ "Load()", &slot_13, QMetaData::Public },
	{ "Save()", &slot_14, QMetaData::Public },
	{ "SaveAll()", &slot_15, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"MT_GLDrawer", parentObject,
	slot_tbl, 16,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_MT_GLDrawer.setMetaObject( metaObj );
    return metaObj;
}

void* MT_GLDrawer::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "MT_GLDrawer" ) )
	return this;
    return QObject::qt_cast( clname );
}

bool MT_GLDrawer::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: SetActive((bool)static_QUType_bool.get(_o+1)); break;
    case 1: NewLayer(); break;
    case 2: DeleteCurrentLayer(); break;
    case 3: SetCurrentLayer((MT_GLDLayer*)static_QUType_ptr.get(_o+1)); break;
    case 4: OnComboBoxChange(); break;
    case 5: UpdateLayerInCombo((MT_GLDLayer&)*((MT_GLDLayer*)static_QUType_ptr.get(_o+1))); break;
    case 6: OnShapeAction((int)static_QUType_int.get(_o+1)); break;
    case 7: CancelCreation(); break;
    case 8: ValidateCreation(); break;
    case 9: MoveSelectionToLayer((int)static_QUType_int.get(_o+1)); break;
    case 10: SetCurrentLayerVisible((bool)static_QUType_bool.get(_o+1)); break;
    case 11: GroupSelection(); break;
    case 12: DisgroupSelection(); break;
    case 13: Load(); break;
    case 14: Save(); break;
    case 15: SaveAll(); break;
    default:
	return QObject::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool MT_GLDrawer::qt_emit( int _id, QUObject* _o )
{
    return QObject::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool MT_GLDrawer::qt_property( int id, int f, QVariant* v)
{
    return QObject::qt_property( id, f, v);
}

bool MT_GLDrawer::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
