/****************************************************************************
** MOS_MainWindow meta object code from reading C++ file 'MOS_MainWindow.h'
**
** Created: ven. 22. juil. 13:35:03 2005
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

const char *MOS_MainWindow::className() const
{
    return "MOS_MainWindow";
}

QMetaObject *MOS_MainWindow::metaObj = 0;
static QMetaObjectCleanUp cleanUp_MOS_MainWindow( "MOS_MainWindow", &MOS_MainWindow::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString MOS_MainWindow::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_MainWindow", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString MOS_MainWindow::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_MainWindow", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* MOS_MainWindow::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QMainWindow::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "nTimeInSeconds", &static_QUType_varptr, "\x11", QUParameter::In }
    };
    static const QUMethod slot_0 = {"DisplayTime", 1, param_slot_0 };
    static const QUParameter param_slot_1[] = {
	{ "pEvent", &static_QUType_ptr, "QMouseEvent", QUParameter::In },
	{ "vGLPos", &static_QUType_ptr, "MT_Vector2D", QUParameter::In }
    };
    static const QUMethod slot_1 = {"DisplayMouseLocation", 2, param_slot_1 };
    static const QUParameter param_slot_2[] = {
	{ "bTickStart", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_2 = {"OnTickStartEnd", 1, param_slot_2 };
    static const QUMethod slot_3 = {"OnLag", 0, 0 };
    static const QUParameter param_slot_4[] = {
	{ "b", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_4 = {"OnConnexionStatusChanged", 1, param_slot_4 };
    static const QUMethod slot_5 = {"DimensionsChanged", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "DisplayTime(uint)", &slot_0, QMetaData::Public },
	{ "DisplayMouseLocation(QMouseEvent*,const MT_Vector2D&)", &slot_1, QMetaData::Public },
	{ "OnTickStartEnd(bool)", &slot_2, QMetaData::Public },
	{ "OnLag()", &slot_3, QMetaData::Public },
	{ "OnConnexionStatusChanged(bool)", &slot_4, QMetaData::Public },
	{ "DimensionsChanged()", &slot_5, QMetaData::Public }
    };
    static const QUParameter param_signal_0[] = {
	{ "selectedElement", &static_QUType_ptr, "MOS_SelectedElement", QUParameter::InOut }
    };
    static const QUMethod signal_0 = {"ElementSelected", 1, param_signal_0 };
    static const QUParameter param_signal_1[] = {
	{ "popupMenu", &static_QUType_ptr, "QPopupMenu", QUParameter::InOut },
	{ "context", &static_QUType_ptr, "MOS_ActionContext", QUParameter::In }
    };
    static const QUMethod signal_1 = {"NewPopupMenu", 2, param_signal_1 };
    static const QUParameter param_signal_2[] = {
	{ "vPoint", &static_QUType_ptr, "MT_Vector2D", QUParameter::In }
    };
    static const QUMethod signal_2 = {"CenterOnPoint", 1, param_signal_2 };
    static const QUMethod signal_3 = {"TeamChanged", 0, 0 };
    static const QUParameter param_signal_4[] = {
	{ "agent", &static_QUType_ptr, "MOS_Agent", QUParameter::InOut }
    };
    static const QUMethod signal_4 = {"ReadingReports", 1, param_signal_4 };
    static const QUParameter param_signal_5[] = {
	{ "strCategory", &static_QUType_ptr, "std::string", QUParameter::In },
	{ "gtia", &static_QUType_ptr, "MOS_Gtia", QUParameter::InOut },
	{ "pParent", &static_QUType_ptr, "MOS_Agent", QUParameter::In }
    };
    static const QUMethod signal_5 = {"ODBNewAgent", 3, param_signal_5 };
    static const QMetaData signal_tbl[] = {
	{ "ElementSelected(MOS_SelectedElement&)", &signal_0, QMetaData::Public },
	{ "NewPopupMenu(QPopupMenu&,const MOS_ActionContext&)", &signal_1, QMetaData::Public },
	{ "CenterOnPoint(const MT_Vector2D&)", &signal_2, QMetaData::Public },
	{ "TeamChanged()", &signal_3, QMetaData::Public },
	{ "ReadingReports(MOS_Agent&)", &signal_4, QMetaData::Public },
	{ "ODBNewAgent(const std::string&,MOS_Gtia&,MOS_Agent*)", &signal_5, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"MOS_MainWindow", parentObject,
	slot_tbl, 6,
	signal_tbl, 6,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_MOS_MainWindow.setMetaObject( metaObj );
    return metaObj;
}

void* MOS_MainWindow::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "MOS_MainWindow" ) )
	return this;
    return QMainWindow::qt_cast( clname );
}

#include <qobjectdefs.h>
#include <qsignalslotimp.h>

// SIGNAL ElementSelected
void MOS_MainWindow::ElementSelected( MOS_SelectedElement& t0 )
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

// SIGNAL NewPopupMenu
void MOS_MainWindow::NewPopupMenu( QPopupMenu& t0, const MOS_ActionContext& t1 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 1 );
    if ( !clist )
	return;
    QUObject o[3];
    static_QUType_ptr.set(o+1,&t0);
    static_QUType_ptr.set(o+2,&t1);
    activate_signal( clist, o );
}

// SIGNAL CenterOnPoint
void MOS_MainWindow::CenterOnPoint( const MT_Vector2D& t0 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 2 );
    if ( !clist )
	return;
    QUObject o[2];
    static_QUType_ptr.set(o+1,&t0);
    activate_signal( clist, o );
}

// SIGNAL TeamChanged
void MOS_MainWindow::TeamChanged()
{
    activate_signal( staticMetaObject()->signalOffset() + 3 );
}

// SIGNAL ReadingReports
void MOS_MainWindow::ReadingReports( MOS_Agent& t0 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 4 );
    if ( !clist )
	return;
    QUObject o[2];
    static_QUType_ptr.set(o+1,&t0);
    activate_signal( clist, o );
}

// SIGNAL ODBNewAgent
void MOS_MainWindow::ODBNewAgent( const std::string& t0, MOS_Gtia& t1, MOS_Agent* t2 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 5 );
    if ( !clist )
	return;
    QUObject o[4];
    static_QUType_ptr.set(o+1,&t0);
    static_QUType_ptr.set(o+2,&t1);
    static_QUType_ptr.set(o+3,t2);
    activate_signal( clist, o );
}

bool MOS_MainWindow::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: DisplayTime((uint)(*((uint*)static_QUType_ptr.get(_o+1)))); break;
    case 1: DisplayMouseLocation((QMouseEvent*)static_QUType_ptr.get(_o+1),(const MT_Vector2D&)*((const MT_Vector2D*)static_QUType_ptr.get(_o+2))); break;
    case 2: OnTickStartEnd((bool)static_QUType_bool.get(_o+1)); break;
    case 3: OnLag(); break;
    case 4: OnConnexionStatusChanged((bool)static_QUType_bool.get(_o+1)); break;
    case 5: DimensionsChanged(); break;
    default:
	return QMainWindow::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool MOS_MainWindow::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: ElementSelected((MOS_SelectedElement&)*((MOS_SelectedElement*)static_QUType_ptr.get(_o+1))); break;
    case 1: NewPopupMenu((QPopupMenu&)*((QPopupMenu*)static_QUType_ptr.get(_o+1)),(const MOS_ActionContext&)*((const MOS_ActionContext*)static_QUType_ptr.get(_o+2))); break;
    case 2: CenterOnPoint((const MT_Vector2D&)*((const MT_Vector2D*)static_QUType_ptr.get(_o+1))); break;
    case 3: TeamChanged(); break;
    case 4: ReadingReports((MOS_Agent&)*((MOS_Agent*)static_QUType_ptr.get(_o+1))); break;
    case 5: ODBNewAgent((const std::string&)*((const std::string*)static_QUType_ptr.get(_o+1)),(MOS_Gtia&)*((MOS_Gtia*)static_QUType_ptr.get(_o+2)),(MOS_Agent*)static_QUType_ptr.get(_o+3)); break;
    default:
	return QMainWindow::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool MOS_MainWindow::qt_property( int id, int f, QVariant* v)
{
    return QMainWindow::qt_property( id, f, v);
}

bool MOS_MainWindow::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
