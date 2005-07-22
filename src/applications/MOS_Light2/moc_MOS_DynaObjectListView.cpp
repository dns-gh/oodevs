/****************************************************************************
** MOS_DynaObjectListView meta object code from reading C++ file 'MOS_DynaObjectListView.h'
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

const char *MOS_DynaObjectListView::className() const
{
    return "MOS_DynaObjectListView";
}

QMetaObject *MOS_DynaObjectListView::metaObj = 0;
static QMetaObjectCleanUp cleanUp_MOS_DynaObjectListView( "MOS_DynaObjectListView", &MOS_DynaObjectListView::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString MOS_DynaObjectListView::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_DynaObjectListView", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString MOS_DynaObjectListView::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_DynaObjectListView", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* MOS_DynaObjectListView::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QListView::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "object", &static_QUType_ptr, "MOS_DynaObject", QUParameter::InOut }
    };
    static const QUMethod slot_0 = {"AddObject", 1, param_slot_0 };
    static const QUParameter param_slot_1[] = {
	{ "object", &static_QUType_ptr, "MOS_DynaObject", QUParameter::InOut }
    };
    static const QUMethod slot_1 = {"RemoveObject", 1, param_slot_1 };
    static const QUParameter param_slot_2[] = {
	{ "selectedElement", &static_QUType_ptr, "MOS_SelectedElement", QUParameter::InOut }
    };
    static const QUMethod slot_2 = {"SetSelectedElement", 1, param_slot_2 };
    static const QUParameter param_slot_3[] = {
	{ "pItem", &static_QUType_ptr, "QListViewItem", QUParameter::In },
	{ "pos", &static_QUType_varptr, "\x0e", QUParameter::In },
	{ "nCol", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_3 = {"OnRequestPopup", 3, param_slot_3 };
    static const QUMethod slot_4 = {"OnRequestCenter", 0, 0 };
    static const QUParameter param_slot_5[] = {
	{ "pItem", &static_QUType_ptr, "QListViewItem", QUParameter::In }
    };
    static const QUMethod slot_5 = {"OnSelectionChange", 1, param_slot_5 };
    static const QUMethod slot_6 = {"OnTeamChanged", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "AddObject(MOS_DynaObject&)", &slot_0, QMetaData::Public },
	{ "RemoveObject(MOS_DynaObject&)", &slot_1, QMetaData::Public },
	{ "SetSelectedElement(MOS_SelectedElement&)", &slot_2, QMetaData::Public },
	{ "OnRequestPopup(QListViewItem*,const QPoint&,int)", &slot_3, QMetaData::Private },
	{ "OnRequestCenter()", &slot_4, QMetaData::Private },
	{ "OnSelectionChange(QListViewItem*)", &slot_5, QMetaData::Private },
	{ "OnTeamChanged()", &slot_6, QMetaData::Private }
    };
    static const QUParameter param_signal_0[] = {
	{ "vPoint", &static_QUType_ptr, "MT_Vector2D", QUParameter::In }
    };
    static const QUMethod signal_0 = {"CenterOnPoint", 1, param_signal_0 };
    static const QUParameter param_signal_1[] = {
	{ "selectedElement", &static_QUType_ptr, "MOS_SelectedElement", QUParameter::InOut }
    };
    static const QUMethod signal_1 = {"ElementSelected", 1, param_signal_1 };
    static const QUParameter param_signal_2[] = {
	{ "popupMenu", &static_QUType_ptr, "QPopupMenu", QUParameter::InOut },
	{ "context", &static_QUType_ptr, "MOS_ActionContext", QUParameter::In }
    };
    static const QUMethod signal_2 = {"NewPopupMenu", 2, param_signal_2 };
    static const QMetaData signal_tbl[] = {
	{ "CenterOnPoint(const MT_Vector2D&)", &signal_0, QMetaData::Private },
	{ "ElementSelected(MOS_SelectedElement&)", &signal_1, QMetaData::Private },
	{ "NewPopupMenu(QPopupMenu&,const MOS_ActionContext&)", &signal_2, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"MOS_DynaObjectListView", parentObject,
	slot_tbl, 7,
	signal_tbl, 3,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_MOS_DynaObjectListView.setMetaObject( metaObj );
    return metaObj;
}

void* MOS_DynaObjectListView::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "MOS_DynaObjectListView" ) )
	return this;
    return QListView::qt_cast( clname );
}

#include <qobjectdefs.h>
#include <qsignalslotimp.h>

// SIGNAL CenterOnPoint
void MOS_DynaObjectListView::CenterOnPoint( const MT_Vector2D& t0 )
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

// SIGNAL ElementSelected
void MOS_DynaObjectListView::ElementSelected( MOS_SelectedElement& t0 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 1 );
    if ( !clist )
	return;
    QUObject o[2];
    static_QUType_ptr.set(o+1,&t0);
    activate_signal( clist, o );
}

// SIGNAL NewPopupMenu
void MOS_DynaObjectListView::NewPopupMenu( QPopupMenu& t0, const MOS_ActionContext& t1 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 2 );
    if ( !clist )
	return;
    QUObject o[3];
    static_QUType_ptr.set(o+1,&t0);
    static_QUType_ptr.set(o+2,&t1);
    activate_signal( clist, o );
}

bool MOS_DynaObjectListView::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: AddObject((MOS_DynaObject&)*((MOS_DynaObject*)static_QUType_ptr.get(_o+1))); break;
    case 1: RemoveObject((MOS_DynaObject&)*((MOS_DynaObject*)static_QUType_ptr.get(_o+1))); break;
    case 2: SetSelectedElement((MOS_SelectedElement&)*((MOS_SelectedElement*)static_QUType_ptr.get(_o+1))); break;
    case 3: OnRequestPopup((QListViewItem*)static_QUType_ptr.get(_o+1),(const QPoint&)*((const QPoint*)static_QUType_ptr.get(_o+2)),(int)static_QUType_int.get(_o+3)); break;
    case 4: OnRequestCenter(); break;
    case 5: OnSelectionChange((QListViewItem*)static_QUType_ptr.get(_o+1)); break;
    case 6: OnTeamChanged(); break;
    default:
	return QListView::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool MOS_DynaObjectListView::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: CenterOnPoint((const MT_Vector2D&)*((const MT_Vector2D*)static_QUType_ptr.get(_o+1))); break;
    case 1: ElementSelected((MOS_SelectedElement&)*((MOS_SelectedElement*)static_QUType_ptr.get(_o+1))); break;
    case 2: NewPopupMenu((QPopupMenu&)*((QPopupMenu*)static_QUType_ptr.get(_o+1)),(const MOS_ActionContext&)*((const MOS_ActionContext*)static_QUType_ptr.get(_o+2))); break;
    default:
	return QListView::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool MOS_DynaObjectListView::qt_property( int id, int f, QVariant* v)
{
    return QListView::qt_property( id, f, v);
}

bool MOS_DynaObjectListView::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
