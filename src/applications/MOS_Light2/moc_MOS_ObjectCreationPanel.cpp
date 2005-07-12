/****************************************************************************
** MOS_ObjectCreationPanel meta object code from reading C++ file 'MOS_ObjectCreationPanel.h'
**
** Created: mar. 12. juil. 12:42:19 2005
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

const char *MOS_ObjectCreationPanel::className() const
{
    return "MOS_ObjectCreationPanel";
}

QMetaObject *MOS_ObjectCreationPanel::metaObj = 0;
static QMetaObjectCleanUp cleanUp_MOS_ObjectCreationPanel( "MOS_ObjectCreationPanel", &MOS_ObjectCreationPanel::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString MOS_ObjectCreationPanel::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_ObjectCreationPanel", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString MOS_ObjectCreationPanel::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_ObjectCreationPanel", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* MOS_ObjectCreationPanel::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QWidget::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "popupMenu", &static_QUType_ptr, "QPopupMenu", QUParameter::InOut },
	{ "context", &static_QUType_ptr, "MOS_ActionContext", QUParameter::In }
    };
    static const QUMethod slot_0 = {"FillRemotePopupMenu", 2, param_slot_0 };
    static const QUMethod slot_1 = {"OnObjectChanged", 0, 0 };
    static const QUMethod slot_2 = {"OnOk", 0, 0 };
    static const QUParameter param_slot_3[] = {
	{ "selectedElement", &static_QUType_ptr, "MOS_SelectedElement", QUParameter::InOut }
    };
    static const QUMethod slot_3 = {"SetSelectedElement", 1, param_slot_3 };
    static const QUParameter param_slot_4[] = {
	{ "team", &static_QUType_ptr, "MOS_Team", QUParameter::InOut }
    };
    static const QUMethod slot_4 = {"OnTeamCreated", 1, param_slot_4 };
    static const QUParameter param_slot_5[] = {
	{ "bConnected", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_5 = {"OnConnexionStatusChanged", 1, param_slot_5 };
    static const QMetaData slot_tbl[] = {
	{ "FillRemotePopupMenu(QPopupMenu&,const MOS_ActionContext&)", &slot_0, QMetaData::Private },
	{ "OnObjectChanged()", &slot_1, QMetaData::Private },
	{ "OnOk()", &slot_2, QMetaData::Private },
	{ "SetSelectedElement(MOS_SelectedElement&)", &slot_3, QMetaData::Private },
	{ "OnTeamCreated(MOS_Team&)", &slot_4, QMetaData::Private },
	{ "OnConnexionStatusChanged(bool)", &slot_5, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"MOS_ObjectCreationPanel", parentObject,
	slot_tbl, 6,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_MOS_ObjectCreationPanel.setMetaObject( metaObj );
    return metaObj;
}

void* MOS_ObjectCreationPanel::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "MOS_ObjectCreationPanel" ) )
	return this;
    if ( !qstrcmp( clname, "MOS_MapEventFilter_ABC" ) )
	return (MOS_MapEventFilter_ABC*)this;
    return QWidget::qt_cast( clname );
}

bool MOS_ObjectCreationPanel::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: FillRemotePopupMenu((QPopupMenu&)*((QPopupMenu*)static_QUType_ptr.get(_o+1)),(const MOS_ActionContext&)*((const MOS_ActionContext*)static_QUType_ptr.get(_o+2))); break;
    case 1: OnObjectChanged(); break;
    case 2: OnOk(); break;
    case 3: SetSelectedElement((MOS_SelectedElement&)*((MOS_SelectedElement*)static_QUType_ptr.get(_o+1))); break;
    case 4: OnTeamCreated((MOS_Team&)*((MOS_Team*)static_QUType_ptr.get(_o+1))); break;
    case 5: OnConnexionStatusChanged((bool)static_QUType_bool.get(_o+1)); break;
    default:
	return QWidget::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool MOS_ObjectCreationPanel::qt_emit( int _id, QUObject* _o )
{
    return QWidget::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool MOS_ObjectCreationPanel::qt_property( int id, int f, QVariant* v)
{
    return QWidget::qt_property( id, f, v);
}

bool MOS_ObjectCreationPanel::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
