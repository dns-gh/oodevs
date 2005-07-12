/****************************************************************************
** ADN_Equipement_GUI meta object code from reading C++ file 'ADN_Equipement_GUI.h'
**
** Created: jeu. 7. juil. 13:46:09 2005
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

const char *ADN_Equipement_GUI::className() const
{
    return "ADN_Equipement_GUI";
}

QMetaObject *ADN_Equipement_GUI::metaObj = 0;
static QMetaObjectCleanUp cleanUp_ADN_Equipement_GUI( "ADN_Equipement_GUI", &ADN_Equipement_GUI::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString ADN_Equipement_GUI::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ADN_Equipement_GUI", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString ADN_Equipement_GUI::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ADN_Equipement_GUI", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* ADN_Equipement_GUI::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = ADN_GUI_ABC::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "nIndex", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_0 = {"IndirectTypeComboActivated", 1, param_slot_0 };
    static const QMetaData slot_tbl[] = {
	{ "IndirectTypeComboActivated(int)", &slot_0, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"ADN_Equipement_GUI", parentObject,
	slot_tbl, 1,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_ADN_Equipement_GUI.setMetaObject( metaObj );
    return metaObj;
}

void* ADN_Equipement_GUI::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "ADN_Equipement_GUI" ) )
	return this;
    return ADN_GUI_ABC::qt_cast( clname );
}

bool ADN_Equipement_GUI::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: IndirectTypeComboActivated((int)static_QUType_int.get(_o+1)); break;
    default:
	return ADN_GUI_ABC::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool ADN_Equipement_GUI::qt_emit( int _id, QUObject* _o )
{
    return ADN_GUI_ABC::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool ADN_Equipement_GUI::qt_property( int id, int f, QVariant* v)
{
    return ADN_GUI_ABC::qt_property( id, f, v);
}

bool ADN_Equipement_GUI::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
