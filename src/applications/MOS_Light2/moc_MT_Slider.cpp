/****************************************************************************
** MT_Slider meta object code from reading C++ file 'MT_Slider.h'
**
** Created: jeu. 21. juil. 15:28:03 2005
**      by: The Qt MOC ($Id: $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "MT_Slider.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *MT_Slider::className() const
{
    return "MT_Slider";
}

QMetaObject *MT_Slider::metaObj = 0;
static QMetaObjectCleanUp cleanUp_MT_Slider( "MT_Slider", &MT_Slider::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString MT_Slider::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MT_Slider", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString MT_Slider::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MT_Slider", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* MT_Slider::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QWidget::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "nValue", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_0 = {"OnValueChanged", 1, param_slot_0 };
    static const QMetaData slot_tbl[] = {
	{ "OnValueChanged(int)", &slot_0, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"MT_Slider", parentObject,
	slot_tbl, 1,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_MT_Slider.setMetaObject( metaObj );
    return metaObj;
}

void* MT_Slider::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "MT_Slider" ) )
	return this;
    return QWidget::qt_cast( clname );
}

bool MT_Slider::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: OnValueChanged((int)static_QUType_int.get(_o+1)); break;
    default:
	return QWidget::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool MT_Slider::qt_emit( int _id, QUObject* _o )
{
    return QWidget::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool MT_Slider::qt_property( int id, int f, QVariant* v)
{
    return QWidget::qt_property( id, f, v);
}

bool MT_Slider::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
