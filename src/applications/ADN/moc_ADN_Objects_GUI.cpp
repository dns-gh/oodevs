/****************************************************************************
** ADN_Objects_GUI meta object code from reading C++ file 'ADN_Objects_GUI.h'
**
** Created: jeu. 21. juil. 11:15:55 2005
**      by: The Qt MOC ($Id: $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "ADN_Objects_GUI.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *ADN_Objects_GUI::className() const
{
    return "ADN_Objects_GUI";
}

QMetaObject *ADN_Objects_GUI::metaObj = 0;
static QMetaObjectCleanUp cleanUp_ADN_Objects_GUI( "ADN_Objects_GUI", &ADN_Objects_GUI::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString ADN_Objects_GUI::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ADN_Objects_GUI", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString ADN_Objects_GUI::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ADN_Objects_GUI", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* ADN_Objects_GUI::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = ADN_GUI_ABC::staticMetaObject();
    static const QUMethod slot_0 = {"OnSpeedImpactComboChanged", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "OnSpeedImpactComboChanged()", &slot_0, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"ADN_Objects_GUI", parentObject,
	slot_tbl, 1,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_ADN_Objects_GUI.setMetaObject( metaObj );
    return metaObj;
}

void* ADN_Objects_GUI::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "ADN_Objects_GUI" ) )
	return this;
    return ADN_GUI_ABC::qt_cast( clname );
}

bool ADN_Objects_GUI::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: OnSpeedImpactComboChanged(); break;
    default:
	return ADN_GUI_ABC::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool ADN_Objects_GUI::qt_emit( int _id, QUObject* _o )
{
    return ADN_GUI_ABC::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool ADN_Objects_GUI::qt_property( int id, int f, QVariant* v)
{
    return ADN_GUI_ABC::qt_property( id, f, v);
}

bool ADN_Objects_GUI::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
