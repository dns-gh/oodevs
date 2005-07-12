/****************************************************************************
** MOS_ReportListView meta object code from reading C++ file 'MOS_ReportListView.h'
**
** Created: mar. 12. juil. 12:42:02 2005
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

const char *MOS_ReportListView::className() const
{
    return "MOS_ReportListView";
}

QMetaObject *MOS_ReportListView::metaObj = 0;
static QMetaObjectCleanUp cleanUp_MOS_ReportListView( "MOS_ReportListView", &MOS_ReportListView::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString MOS_ReportListView::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_ReportListView", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString MOS_ReportListView::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_ReportListView", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* MOS_ReportListView::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QListView::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "agent", &static_QUType_ptr, "MOS_Agent", QUParameter::InOut },
	{ "report", &static_QUType_ptr, "MOS_Report_ABC", QUParameter::InOut }
    };
    static const QUMethod slot_0 = {"OnReportCreated", 2, param_slot_0 };
    static const QUParameter param_slot_1[] = {
	{ 0, &static_QUType_ptr, "QListViewItem", QUParameter::In },
	{ 0, &static_QUType_varptr, "\x0e", QUParameter::In },
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_1 = {"OnClick", 3, param_slot_1 };
    static const QUMethod slot_2 = {"OnRequestCenter", 0, 0 };
    static const QUParameter param_slot_3[] = {
	{ "pItem", &static_QUType_ptr, "QListViewItem", QUParameter::In },
	{ "pos", &static_QUType_varptr, "\x0e", QUParameter::In },
	{ "nCol", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_3 = {"OnRequestPopup", 3, param_slot_3 };
    static const QUMethod slot_4 = {"OnClearAll", 0, 0 };
    static const QUMethod slot_5 = {"OnClearTrace", 0, 0 };
    static const QUMethod slot_6 = {"OnClearUpTo", 0, 0 };
    static const QUMethod slot_7 = {"NotifyReadingReports", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "OnReportCreated(MOS_Agent&,MOS_Report_ABC&)", &slot_0, QMetaData::Private },
	{ "OnClick(QListViewItem*,const QPoint&,int)", &slot_1, QMetaData::Private },
	{ "OnRequestCenter()", &slot_2, QMetaData::Private },
	{ "OnRequestPopup(QListViewItem*,const QPoint&,int)", &slot_3, QMetaData::Private },
	{ "OnClearAll()", &slot_4, QMetaData::Private },
	{ "OnClearTrace()", &slot_5, QMetaData::Private },
	{ "OnClearUpTo()", &slot_6, QMetaData::Private },
	{ "NotifyReadingReports()", &slot_7, QMetaData::Private }
    };
    static const QUParameter param_signal_0[] = {
	{ "selectedElement", &static_QUType_ptr, "MOS_SelectedElement", QUParameter::InOut }
    };
    static const QUMethod signal_0 = {"ElementSelected", 1, param_signal_0 };
    static const QUParameter param_signal_1[] = {
	{ "vPoint", &static_QUType_ptr, "MT_Vector2D", QUParameter::In }
    };
    static const QUMethod signal_1 = {"CenterOnPoint", 1, param_signal_1 };
    static const QUParameter param_signal_2[] = {
	{ "popupMenu", &static_QUType_ptr, "QPopupMenu", QUParameter::InOut },
	{ "context", &static_QUType_ptr, "MOS_ActionContext", QUParameter::In }
    };
    static const QUMethod signal_2 = {"NewPopupMenu", 2, param_signal_2 };
    static const QUParameter param_signal_3[] = {
	{ "agent", &static_QUType_ptr, "MOS_Agent", QUParameter::InOut }
    };
    static const QUMethod signal_3 = {"ReadingReports", 1, param_signal_3 };
    static const QMetaData signal_tbl[] = {
	{ "ElementSelected(MOS_SelectedElement&)", &signal_0, QMetaData::Private },
	{ "CenterOnPoint(const MT_Vector2D&)", &signal_1, QMetaData::Private },
	{ "NewPopupMenu(QPopupMenu&,const MOS_ActionContext&)", &signal_2, QMetaData::Private },
	{ "ReadingReports(MOS_Agent&)", &signal_3, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"MOS_ReportListView", parentObject,
	slot_tbl, 8,
	signal_tbl, 4,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_MOS_ReportListView.setMetaObject( metaObj );
    return metaObj;
}

void* MOS_ReportListView::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "MOS_ReportListView" ) )
	return this;
    return QListView::qt_cast( clname );
}

#include <qobjectdefs.h>
#include <qsignalslotimp.h>

// SIGNAL ElementSelected
void MOS_ReportListView::ElementSelected( MOS_SelectedElement& t0 )
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

// SIGNAL CenterOnPoint
void MOS_ReportListView::CenterOnPoint( const MT_Vector2D& t0 )
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
void MOS_ReportListView::NewPopupMenu( QPopupMenu& t0, const MOS_ActionContext& t1 )
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

// SIGNAL ReadingReports
void MOS_ReportListView::ReadingReports( MOS_Agent& t0 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 3 );
    if ( !clist )
	return;
    QUObject o[2];
    static_QUType_ptr.set(o+1,&t0);
    activate_signal( clist, o );
}

bool MOS_ReportListView::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: OnReportCreated((MOS_Agent&)*((MOS_Agent*)static_QUType_ptr.get(_o+1)),(MOS_Report_ABC&)*((MOS_Report_ABC*)static_QUType_ptr.get(_o+2))); break;
    case 1: OnClick((QListViewItem*)static_QUType_ptr.get(_o+1),(const QPoint&)*((const QPoint*)static_QUType_ptr.get(_o+2)),(int)static_QUType_int.get(_o+3)); break;
    case 2: OnRequestCenter(); break;
    case 3: OnRequestPopup((QListViewItem*)static_QUType_ptr.get(_o+1),(const QPoint&)*((const QPoint*)static_QUType_ptr.get(_o+2)),(int)static_QUType_int.get(_o+3)); break;
    case 4: OnClearAll(); break;
    case 5: OnClearTrace(); break;
    case 6: OnClearUpTo(); break;
    case 7: NotifyReadingReports(); break;
    default:
	return QListView::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool MOS_ReportListView::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: ElementSelected((MOS_SelectedElement&)*((MOS_SelectedElement*)static_QUType_ptr.get(_o+1))); break;
    case 1: CenterOnPoint((const MT_Vector2D&)*((const MT_Vector2D*)static_QUType_ptr.get(_o+1))); break;
    case 2: NewPopupMenu((QPopupMenu&)*((QPopupMenu*)static_QUType_ptr.get(_o+1)),(const MOS_ActionContext&)*((const MOS_ActionContext*)static_QUType_ptr.get(_o+2))); break;
    case 3: ReadingReports((MOS_Agent&)*((MOS_Agent*)static_QUType_ptr.get(_o+1))); break;
    default:
	return QListView::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool MOS_ReportListView::qt_property( int id, int f, QVariant* v)
{
    return QListView::qt_property( id, f, v);
}

bool MOS_ReportListView::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
