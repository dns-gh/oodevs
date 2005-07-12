/****************************************************************************
** ADN_ComboBox meta object code from reading C++ file 'ADN_ComboBox.h'
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

const char *ADN_ComboBox::className() const
{
    return "ADN_ComboBox";
}

QMetaObject *ADN_ComboBox::metaObj = 0;
static QMetaObjectCleanUp cleanUp_ADN_ComboBox( "ADN_ComboBox", &ADN_ComboBox::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString ADN_ComboBox::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ADN_ComboBox", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString ADN_ComboBox::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ADN_ComboBox", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* ADN_ComboBox::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QComboBox::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "ndx", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_0 = {"ItemSelected", 1, param_slot_0 };
    static const QUMethod slot_1 = {"UpdateEnableState", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "ItemSelected(int)", &slot_0, QMetaData::Private },
	{ "UpdateEnableState()", &slot_1, QMetaData::Protected }
    };
    static const QUParameter param_signal_0[] = {
	{ "nIndex", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod signal_0 = {"Activated", 1, param_signal_0 };
    static const QMetaData signal_tbl[] = {
	{ "Activated(int)", &signal_0, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"ADN_ComboBox", parentObject,
	slot_tbl, 2,
	signal_tbl, 1,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_ADN_ComboBox.setMetaObject( metaObj );
    return metaObj;
}

void* ADN_ComboBox::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "ADN_ComboBox" ) )
	return this;
    if ( !qstrcmp( clname, "ADN_Gfx_ABC" ) )
	return (ADN_Gfx_ABC*)this;
    return QComboBox::qt_cast( clname );
}

// SIGNAL Activated
void ADN_ComboBox::Activated( int t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 0, t0 );
}

bool ADN_ComboBox::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: ItemSelected((int)static_QUType_int.get(_o+1)); break;
    case 1: UpdateEnableState(); break;
    default:
	return QComboBox::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool ADN_ComboBox::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: Activated((int)static_QUType_int.get(_o+1)); break;
    default:
	return QComboBox::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool ADN_ComboBox::qt_property( int id, int f, QVariant* v)
{
    return QComboBox::qt_property( id, f, v);
}

bool ADN_ComboBox::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
