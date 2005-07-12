/****************************************************************************
** MOS_UnitToolbar meta object code from reading C++ file 'MOS_UnitToolbar.h'
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

const char *MOS_UnitToolbar::className() const
{
    return "MOS_UnitToolbar";
}

QMetaObject *MOS_UnitToolbar::metaObj = 0;
static QMetaObjectCleanUp cleanUp_MOS_UnitToolbar( "MOS_UnitToolbar", &MOS_UnitToolbar::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString MOS_UnitToolbar::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_UnitToolbar", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString MOS_UnitToolbar::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_UnitToolbar", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* MOS_UnitToolbar::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QToolBar::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "rValue", &static_QUType_ptr, "float", QUParameter::In }
    };
    static const QUMethod slot_0 = {"OnVisionLines", 1, param_slot_0 };
    static const QUParameter param_slot_1[] = {
	{ "rValue", &static_QUType_ptr, "float", QUParameter::In }
    };
    static const QUMethod slot_1 = {"OnVisionCones", 1, param_slot_1 };
    static const QUParameter param_slot_2[] = {
	{ "rValue", &static_QUType_ptr, "float", QUParameter::In }
    };
    static const QUMethod slot_2 = {"OnVisionSurfaces", 1, param_slot_2 };
    static const QUParameter param_slot_3[] = {
	{ "rValue", &static_QUType_ptr, "float", QUParameter::In }
    };
    static const QUMethod slot_3 = {"OnPaths", 1, param_slot_3 };
    static const QUMethod slot_4 = {"OnOldPathsToggled", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "OnVisionLines(float)", &slot_0, QMetaData::Private },
	{ "OnVisionCones(float)", &slot_1, QMetaData::Private },
	{ "OnVisionSurfaces(float)", &slot_2, QMetaData::Private },
	{ "OnPaths(float)", &slot_3, QMetaData::Private },
	{ "OnOldPathsToggled()", &slot_4, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"MOS_UnitToolbar", parentObject,
	slot_tbl, 5,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_MOS_UnitToolbar.setMetaObject( metaObj );
    return metaObj;
}

void* MOS_UnitToolbar::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "MOS_UnitToolbar" ) )
	return this;
    return QToolBar::qt_cast( clname );
}

bool MOS_UnitToolbar::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: OnVisionLines((float)(*((float*)static_QUType_ptr.get(_o+1)))); break;
    case 1: OnVisionCones((float)(*((float*)static_QUType_ptr.get(_o+1)))); break;
    case 2: OnVisionSurfaces((float)(*((float*)static_QUType_ptr.get(_o+1)))); break;
    case 3: OnPaths((float)(*((float*)static_QUType_ptr.get(_o+1)))); break;
    case 4: OnOldPathsToggled(); break;
    default:
	return QToolBar::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool MOS_UnitToolbar::qt_emit( int _id, QUObject* _o )
{
    return QToolBar::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool MOS_UnitToolbar::qt_property( int id, int f, QVariant* v)
{
    return QToolBar::qt_property( id, f, v);
}

bool MOS_UnitToolbar::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
