/****************************************************************************
** MOS_GraphWindow meta object code from reading C++ file 'MOS_GraphWindow.h'
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

const char *MOS_GraphWindow::className() const
{
    return "MOS_GraphWindow";
}

QMetaObject *MOS_GraphWindow::metaObj = 0;
static QMetaObjectCleanUp cleanUp_MOS_GraphWindow( "MOS_GraphWindow", &MOS_GraphWindow::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString MOS_GraphWindow::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_GraphWindow", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString MOS_GraphWindow::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_GraphWindow", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* MOS_GraphWindow::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QFrame::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "bOn", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_0 = {"SlotMainLoopTraceToggled", 1, param_slot_0 };
    static const QUParameter param_slot_1[] = {
	{ "bOn", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_1 = {"SlotPerceptionTraceToggled", 1, param_slot_1 };
    static const QUParameter param_slot_2[] = {
	{ "bOn", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_2 = {"SlotDecisionTraceToggled", 1, param_slot_2 };
    static const QUParameter param_slot_3[] = {
	{ "bOn", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_3 = {"SlotActionTraceToggled", 1, param_slot_3 };
    static const QMetaData slot_tbl[] = {
	{ "SlotMainLoopTraceToggled(bool)", &slot_0, QMetaData::Private },
	{ "SlotPerceptionTraceToggled(bool)", &slot_1, QMetaData::Private },
	{ "SlotDecisionTraceToggled(bool)", &slot_2, QMetaData::Private },
	{ "SlotActionTraceToggled(bool)", &slot_3, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"MOS_GraphWindow", parentObject,
	slot_tbl, 4,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_MOS_GraphWindow.setMetaObject( metaObj );
    return metaObj;
}

void* MOS_GraphWindow::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "MOS_GraphWindow" ) )
	return this;
    return QFrame::qt_cast( clname );
}

bool MOS_GraphWindow::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: SlotMainLoopTraceToggled((bool)static_QUType_bool.get(_o+1)); break;
    case 1: SlotPerceptionTraceToggled((bool)static_QUType_bool.get(_o+1)); break;
    case 2: SlotDecisionTraceToggled((bool)static_QUType_bool.get(_o+1)); break;
    case 3: SlotActionTraceToggled((bool)static_QUType_bool.get(_o+1)); break;
    default:
	return QFrame::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool MOS_GraphWindow::qt_emit( int _id, QUObject* _o )
{
    return QFrame::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool MOS_GraphWindow::qt_property( int id, int f, QVariant* v)
{
    return QFrame::qt_property( id, f, v);
}

bool MOS_GraphWindow::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
