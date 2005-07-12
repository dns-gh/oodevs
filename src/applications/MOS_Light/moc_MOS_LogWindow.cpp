/****************************************************************************
** MOS_LogWindow meta object code from reading C++ file 'MOS_LogWindow.h'
**
** Created: lun. 11. juil. 18:50:12 2005
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

const char *MOS_LogWindow::className() const
{
    return "MOS_LogWindow";
}

QMetaObject *MOS_LogWindow::metaObj = 0;
static QMetaObjectCleanUp cleanUp_MOS_LogWindow( "MOS_LogWindow", &MOS_LogWindow::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString MOS_LogWindow::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_LogWindow", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString MOS_LogWindow::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_LogWindow", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* MOS_LogWindow::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QListView::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ 0, &static_QUType_ptr, "QListViewItem", QUParameter::In },
	{ 0, &static_QUType_varptr, "\x0e", QUParameter::In },
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_0 = {"SlotRightButtonPressed", 3, param_slot_0 };
    static const QUMethod slot_1 = {"SlotSetAutoScroll", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "SlotRightButtonPressed(QListViewItem*,const QPoint&,int)", &slot_0, QMetaData::Protected },
	{ "SlotSetAutoScroll()", &slot_1, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"MOS_LogWindow", parentObject,
	slot_tbl, 2,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_MOS_LogWindow.setMetaObject( metaObj );
    return metaObj;
}

void* MOS_LogWindow::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "MOS_LogWindow" ) )
	return this;
    if ( !qstrcmp( clname, "MT_Logger_ABC" ) )
	return (MT_Logger_ABC*)this;
    return QListView::qt_cast( clname );
}

bool MOS_LogWindow::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: SlotRightButtonPressed((QListViewItem*)static_QUType_ptr.get(_o+1),(const QPoint&)*((const QPoint*)static_QUType_ptr.get(_o+2)),(int)static_QUType_int.get(_o+3)); break;
    case 1: SlotSetAutoScroll(); break;
    default:
	return QListView::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool MOS_LogWindow::qt_emit( int _id, QUObject* _o )
{
    return QListView::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool MOS_LogWindow::qt_property( int id, int f, QVariant* v)
{
    return QListView::qt_property( id, f, v);
}

bool MOS_LogWindow::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
