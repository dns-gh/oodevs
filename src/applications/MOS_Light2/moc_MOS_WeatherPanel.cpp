/****************************************************************************
** MOS_WeatherPanel meta object code from reading C++ file 'MOS_WeatherPanel.h'
**
** Created: mar. 12. juil. 12:42:02 2005
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

const char *MOS_WeatherPanel::className() const
{
    return "MOS_WeatherPanel";
}

QMetaObject *MOS_WeatherPanel::metaObj = 0;
static QMetaObjectCleanUp cleanUp_MOS_WeatherPanel( "MOS_WeatherPanel", &MOS_WeatherPanel::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString MOS_WeatherPanel::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_WeatherPanel", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString MOS_WeatherPanel::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_WeatherPanel", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* MOS_WeatherPanel::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QVBox::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "pItem", &static_QUType_ptr, "QListViewItem", QUParameter::In }
    };
    static const QUMethod slot_0 = {"OnSelectionChanged", 1, param_slot_0 };
    static const QUParameter param_slot_1[] = {
	{ "pItem", &static_QUType_ptr, "QListViewItem", QUParameter::In },
	{ "pos", &static_QUType_varptr, "\x0e", QUParameter::In }
    };
    static const QUMethod slot_1 = {"OnRequestPopup", 2, param_slot_1 };
    static const QUMethod slot_2 = {"OnDelete", 0, 0 };
    static const QUMethod slot_3 = {"OnCreate", 0, 0 };
    static const QUMethod slot_4 = {"SetLocation", 0, 0 };
    static const QUMethod slot_5 = {"LocationDone", 0, 0 };
    static const QUMethod slot_6 = {"OnDataChanged", 0, 0 };
    static const QUMethod slot_7 = {"OnOk", 0, 0 };
    static const QUMethod slot_8 = {"OnCancel", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "OnSelectionChanged(QListViewItem*)", &slot_0, QMetaData::Private },
	{ "OnRequestPopup(QListViewItem*,const QPoint&)", &slot_1, QMetaData::Private },
	{ "OnDelete()", &slot_2, QMetaData::Private },
	{ "OnCreate()", &slot_3, QMetaData::Private },
	{ "SetLocation()", &slot_4, QMetaData::Private },
	{ "LocationDone()", &slot_5, QMetaData::Private },
	{ "OnDataChanged()", &slot_6, QMetaData::Private },
	{ "OnOk()", &slot_7, QMetaData::Private },
	{ "OnCancel()", &slot_8, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"MOS_WeatherPanel", parentObject,
	slot_tbl, 9,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_MOS_WeatherPanel.setMetaObject( metaObj );
    return metaObj;
}

void* MOS_WeatherPanel::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "MOS_WeatherPanel" ) )
	return this;
    return QVBox::qt_cast( clname );
}

bool MOS_WeatherPanel::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: OnSelectionChanged((QListViewItem*)static_QUType_ptr.get(_o+1)); break;
    case 1: OnRequestPopup((QListViewItem*)static_QUType_ptr.get(_o+1),(const QPoint&)*((const QPoint*)static_QUType_ptr.get(_o+2))); break;
    case 2: OnDelete(); break;
    case 3: OnCreate(); break;
    case 4: SetLocation(); break;
    case 5: LocationDone(); break;
    case 6: OnDataChanged(); break;
    case 7: OnOk(); break;
    case 8: OnCancel(); break;
    default:
	return QVBox::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool MOS_WeatherPanel::qt_emit( int _id, QUObject* _o )
{
    return QVBox::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool MOS_WeatherPanel::qt_property( int id, int f, QVariant* v)
{
    return QVBox::qt_property( id, f, v);
}

bool MOS_WeatherPanel::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
