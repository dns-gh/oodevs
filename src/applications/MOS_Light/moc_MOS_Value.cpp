/****************************************************************************
** MOS_Value meta object code from reading C++ file 'MOS_Value.h'
**
** Created: lun. 11. juil. 18:50:11 2005
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

const char *MOS_Value::className() const
{
    return "MOS_Value";
}

QMetaObject *MOS_Value::metaObj = 0;
static QMetaObjectCleanUp cleanUp_MOS_Value( "MOS_Value", &MOS_Value::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString MOS_Value::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_Value", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString MOS_Value::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_Value", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* MOS_Value::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QFrame::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "nValue", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_0 = {"SlotSetValueSlider", 1, param_slot_0 };
    static const QUParameter param_slot_1[] = {
	{ "nValue", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_1 = {"SlotSetValueSpinBox", 1, param_slot_1 };
    static const QUParameter param_slot_2[] = {
	{ "nValue", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_2 = {"SlotSetValueSpeedo", 1, param_slot_2 };
    static const QUParameter param_slot_3[] = {
	{ "bState", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_3 = {"SlotCheckBoxToggled", 1, param_slot_3 };
    static const QUMethod slot_4 = {"SlotButtonPressed", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "SlotSetValueSlider(int)", &slot_0, QMetaData::Protected },
	{ "SlotSetValueSpinBox(int)", &slot_1, QMetaData::Protected },
	{ "SlotSetValueSpeedo(int)", &slot_2, QMetaData::Protected },
	{ "SlotCheckBoxToggled(bool)", &slot_3, QMetaData::Protected },
	{ "SlotButtonPressed()", &slot_4, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"MOS_Value", parentObject,
	slot_tbl, 5,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_MOS_Value.setMetaObject( metaObj );
    return metaObj;
}

void* MOS_Value::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "MOS_Value" ) )
	return this;
    return QFrame::qt_cast( clname );
}

bool MOS_Value::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: SlotSetValueSlider((int)static_QUType_int.get(_o+1)); break;
    case 1: SlotSetValueSpinBox((int)static_QUType_int.get(_o+1)); break;
    case 2: SlotSetValueSpeedo((int)static_QUType_int.get(_o+1)); break;
    case 3: SlotCheckBoxToggled((bool)static_QUType_bool.get(_o+1)); break;
    case 4: SlotButtonPressed(); break;
    default:
	return QFrame::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool MOS_Value::qt_emit( int _id, QUObject* _o )
{
    return QFrame::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool MOS_Value::qt_property( int id, int f, QVariant* v)
{
    return QFrame::qt_property( id, f, v);
}

bool MOS_Value::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
