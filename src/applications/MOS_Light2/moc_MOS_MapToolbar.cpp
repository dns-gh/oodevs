/****************************************************************************
** MOS_MapToolbar meta object code from reading C++ file 'MOS_MapToolbar.h'
**
** Created: ven. 22. juil. 13:35:02 2005
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

const char *MOS_MapToolbar::className() const
{
    return "MOS_MapToolbar";
}

QMetaObject *MOS_MapToolbar::metaObj = 0;
static QMetaObjectCleanUp cleanUp_MOS_MapToolbar( "MOS_MapToolbar", &MOS_MapToolbar::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString MOS_MapToolbar::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_MapToolbar", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString MOS_MapToolbar::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_MapToolbar", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* MOS_MapToolbar::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QToolBar::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "rValue", &static_QUType_ptr, "float", QUParameter::In }
    };
    static const QUMethod slot_0 = {"OnSmallText", 1, param_slot_0 };
    static const QUParameter param_slot_1[] = {
	{ "rValue", &static_QUType_ptr, "float", QUParameter::In }
    };
    static const QUMethod slot_1 = {"OnLargeText", 1, param_slot_1 };
    static const QUParameter param_slot_2[] = {
	{ "rValue", &static_QUType_ptr, "float", QUParameter::In }
    };
    static const QUMethod slot_2 = {"OnDetails", 1, param_slot_2 };
    static const QUParameter param_slot_3[] = {
	{ "rValue", &static_QUType_ptr, "float", QUParameter::In }
    };
    static const QUMethod slot_3 = {"OnTacticalLines", 1, param_slot_3 };
    static const QUParameter param_slot_4[] = {
	{ "rValue", &static_QUType_ptr, "float", QUParameter::In }
    };
    static const QUMethod slot_4 = {"OnGrid", 1, param_slot_4 };
    static const QUMethod slot_5 = {"OnWeatherToggled", 0, 0 };
    static const QUMethod slot_6 = {"On3DToggle", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "OnSmallText(float)", &slot_0, QMetaData::Private },
	{ "OnLargeText(float)", &slot_1, QMetaData::Private },
	{ "OnDetails(float)", &slot_2, QMetaData::Private },
	{ "OnTacticalLines(float)", &slot_3, QMetaData::Private },
	{ "OnGrid(float)", &slot_4, QMetaData::Private },
	{ "OnWeatherToggled()", &slot_5, QMetaData::Private },
	{ "On3DToggle()", &slot_6, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"MOS_MapToolbar", parentObject,
	slot_tbl, 7,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_MOS_MapToolbar.setMetaObject( metaObj );
    return metaObj;
}

void* MOS_MapToolbar::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "MOS_MapToolbar" ) )
	return this;
    return QToolBar::qt_cast( clname );
}

bool MOS_MapToolbar::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: OnSmallText((float)(*((float*)static_QUType_ptr.get(_o+1)))); break;
    case 1: OnLargeText((float)(*((float*)static_QUType_ptr.get(_o+1)))); break;
    case 2: OnDetails((float)(*((float*)static_QUType_ptr.get(_o+1)))); break;
    case 3: OnTacticalLines((float)(*((float*)static_QUType_ptr.get(_o+1)))); break;
    case 4: OnGrid((float)(*((float*)static_QUType_ptr.get(_o+1)))); break;
    case 5: OnWeatherToggled(); break;
    case 6: On3DToggle(); break;
    default:
	return QToolBar::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool MOS_MapToolbar::qt_emit( int _id, QUObject* _o )
{
    return QToolBar::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool MOS_MapToolbar::qt_property( int id, int f, QVariant* v)
{
    return QToolBar::qt_property( id, f, v);
}

bool MOS_MapToolbar::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
