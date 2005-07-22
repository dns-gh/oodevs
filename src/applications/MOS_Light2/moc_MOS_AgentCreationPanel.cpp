/****************************************************************************
** MOS_AgentCreationPanel meta object code from reading C++ file 'MOS_AgentCreationPanel.h'
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

const char *MOS_AgentCreationPanel::className() const
{
    return "MOS_AgentCreationPanel";
}

QMetaObject *MOS_AgentCreationPanel::metaObj = 0;
static QMetaObjectCleanUp cleanUp_MOS_AgentCreationPanel( "MOS_AgentCreationPanel", &MOS_AgentCreationPanel::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString MOS_AgentCreationPanel::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_AgentCreationPanel", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString MOS_AgentCreationPanel::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_AgentCreationPanel", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* MOS_AgentCreationPanel::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QWidgetStack::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "selectedElement", &static_QUType_ptr, "MOS_SelectedElement", QUParameter::InOut }
    };
    static const QUMethod slot_0 = {"SetSelectedElement", 1, param_slot_0 };
    static const QUParameter param_slot_1[] = {
	{ "popupMenu", &static_QUType_ptr, "QPopupMenu", QUParameter::InOut },
	{ "context", &static_QUType_ptr, "MOS_ActionContext", QUParameter::In }
    };
    static const QUMethod slot_1 = {"FillRemotePopupMenu", 2, param_slot_1 };
    static const QUMethod slot_2 = {"CreateGtia", 0, 0 };
    static const QUMethod slot_3 = {"ToggleAutomate", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "SetSelectedElement(MOS_SelectedElement&)", &slot_0, QMetaData::Private },
	{ "FillRemotePopupMenu(QPopupMenu&,const MOS_ActionContext&)", &slot_1, QMetaData::Private },
	{ "CreateGtia()", &slot_2, QMetaData::Private },
	{ "ToggleAutomate()", &slot_3, QMetaData::Private }
    };
    static const QUParameter param_signal_0[] = {
	{ "selectedElement", &static_QUType_ptr, "MOS_SelectedElement", QUParameter::InOut }
    };
    static const QUMethod signal_0 = {"ElementSelected", 1, param_signal_0 };
    static const QMetaData signal_tbl[] = {
	{ "ElementSelected(MOS_SelectedElement&)", &signal_0, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"MOS_AgentCreationPanel", parentObject,
	slot_tbl, 4,
	signal_tbl, 1,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_MOS_AgentCreationPanel.setMetaObject( metaObj );
    return metaObj;
}

void* MOS_AgentCreationPanel::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "MOS_AgentCreationPanel" ) )
	return this;
    return QWidgetStack::qt_cast( clname );
}

#include <qobjectdefs.h>
#include <qsignalslotimp.h>

// SIGNAL ElementSelected
void MOS_AgentCreationPanel::ElementSelected( MOS_SelectedElement& t0 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 0 );
    if ( !clist )
	return;
    QUObject o[2];
    static_QUType_ptr.set(o+1,&t0);
    activate_signal( clist, o );
}

bool MOS_AgentCreationPanel::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: SetSelectedElement((MOS_SelectedElement&)*((MOS_SelectedElement*)static_QUType_ptr.get(_o+1))); break;
    case 1: FillRemotePopupMenu((QPopupMenu&)*((QPopupMenu*)static_QUType_ptr.get(_o+1)),(const MOS_ActionContext&)*((const MOS_ActionContext*)static_QUType_ptr.get(_o+2))); break;
    case 2: CreateGtia(); break;
    case 3: ToggleAutomate(); break;
    default:
	return QWidgetStack::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool MOS_AgentCreationPanel::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: ElementSelected((MOS_SelectedElement&)*((MOS_SelectedElement*)static_QUType_ptr.get(_o+1))); break;
    default:
	return QWidgetStack::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool MOS_AgentCreationPanel::qt_property( int id, int f, QVariant* v)
{
    return QWidgetStack::qt_property( id, f, v);
}

bool MOS_AgentCreationPanel::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
