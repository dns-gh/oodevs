/****************************************************************************
** MOS_GLWidget meta object code from reading C++ file 'MOS_GLWidget.h'
**
** Created: jeu. 21. juil. 15:28:03 2005
**      by: The Qt MOC ($Id: $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "MOS_GLWidget.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *MOS_GLWidget::className() const
{
    return "MOS_GLWidget";
}

QMetaObject *MOS_GLWidget::metaObj = 0;
static QMetaObjectCleanUp cleanUp_MOS_GLWidget( "MOS_GLWidget", &MOS_GLWidget::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString MOS_GLWidget::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_GLWidget", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString MOS_GLWidget::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_GLWidget", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* MOS_GLWidget::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QGLWidget::staticMetaObject();
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
	{ "MouseMove(QMouseEvent*,const MT_Vector2D&)", &signal_0, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"MOS_GLWidget", parentObject,
	slot_tbl, 1,
	signal_tbl, 1,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_MOS_GLWidget.setMetaObject( metaObj );
    return metaObj;
}

void* MOS_GLWidget::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "MOS_GLWidget" ) )
	return this;
    return QGLWidget::qt_cast( clname );
}

#include <qobjectdefs.h>
#include <qsignalslotimp.h>

// SIGNAL MouseMove
void MOS_GLWidget::MouseMove( QMouseEvent* t0, const MT_Vector2D& t1 )
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

bool MOS_GLWidget::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: OnCenterOnPoint((const MT_Vector2D&)*((const MT_Vector2D*)static_QUType_ptr.get(_o+1))); break;
    default:
	return QGLWidget::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool MOS_GLWidget::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: MouseMove((QMouseEvent*)static_QUType_ptr.get(_o+1),(const MT_Vector2D&)*((const MT_Vector2D*)static_QUType_ptr.get(_o+2))); break;
    default:
	return QGLWidget::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool MOS_GLWidget::qt_property( int id, int f, QVariant* v)
{
    return QGLWidget::qt_property( id, f, v);
}

bool MOS_GLWidget::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
