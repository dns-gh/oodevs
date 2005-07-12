/****************************************************************************
** MOS_GL3DWidget meta object code from reading C++ file 'MOS_GL3DWidget.h'
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

const char *MOS_GL3DWidget::className() const
{
    return "MOS_GL3DWidget";
}

QMetaObject *MOS_GL3DWidget::metaObj = 0;
static QMetaObjectCleanUp cleanUp_MOS_GL3DWidget( "MOS_GL3DWidget", &MOS_GL3DWidget::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString MOS_GL3DWidget::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_GL3DWidget", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString MOS_GL3DWidget::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_GL3DWidget", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* MOS_GL3DWidget::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = Widget3D::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "vPoint", &static_QUType_ptr, "MT_Vector2D", QUParameter::In }
    };
    static const QUMethod slot_0 = {"OnCenterOnPoint", 1, param_slot_0 };
    static const QMetaData slot_tbl[] = {
	{ "OnCenterOnPoint(const MT_Vector2D&)", &slot_0, QMetaData::Public }
    };
    static const QUParameter param_signal_0[] = {
	{ "pMouseEvent", &static_QUType_ptr, "QMouseEvent", QUParameter::In },
	{ "vGLPos", &static_QUType_ptr, "MT_Vector2D", QUParameter::In }
    };
    static const QUMethod signal_0 = {"MouseMove", 2, param_signal_0 };
    static const QMetaData signal_tbl[] = {
	{ "MouseMove(QMouseEvent*,const MT_Vector2D&)", &signal_0, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"MOS_GL3DWidget", parentObject,
	slot_tbl, 1,
	signal_tbl, 1,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_MOS_GL3DWidget.setMetaObject( metaObj );
    return metaObj;
}

void* MOS_GL3DWidget::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "MOS_GL3DWidget" ) )
	return this;
    return Widget3D::qt_cast( clname );
}

#include <qobjectdefs.h>
#include <qsignalslotimp.h>

// SIGNAL MouseMove
void MOS_GL3DWidget::MouseMove( QMouseEvent* t0, const MT_Vector2D& t1 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 0 );
    if ( !clist )
	return;
    QUObject o[3];
    static_QUType_ptr.set(o+1,t0);
    static_QUType_ptr.set(o+2,&t1);
    activate_signal( clist, o );
}

bool MOS_GL3DWidget::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: OnCenterOnPoint((const MT_Vector2D&)*((const MT_Vector2D*)static_QUType_ptr.get(_o+1))); break;
    default:
	return Widget3D::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool MOS_GL3DWidget::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: MouseMove((QMouseEvent*)static_QUType_ptr.get(_o+1),(const MT_Vector2D&)*((const MT_Vector2D*)static_QUType_ptr.get(_o+2))); break;
    default:
	return Widget3D::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool MOS_GL3DWidget::qt_property( int id, int f, QVariant* v)
{
    return Widget3D::qt_property( id, f, v);
}

bool MOS_GL3DWidget::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
