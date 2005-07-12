/****************************************************************************
** ADN_ListView_Sensors meta object code from reading C++ file 'ADN_ListView_Sensors.h'
**
** Created: ven. 22. avr. 17:31:39 2005
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

const char *ADN_ListView_Sensors::className() const
{
    return "ADN_ListView_Sensors";
}

QMetaObject *ADN_ListView_Sensors::metaObj = 0;
static QMetaObjectCleanUp cleanUp_ADN_ListView_Sensors( "ADN_ListView_Sensors", &ADN_ListView_Sensors::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString ADN_ListView_Sensors::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ADN_ListView_Sensors", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString ADN_ListView_Sensors::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ADN_ListView_Sensors", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* ADN_ListView_Sensors::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = ADN_ListView::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "pItem", &static_QUType_ptr, "QListViewItem", QUParameter::In }
    };
    static const QUMethod slot_0 = {"OnOnItem", 1, param_slot_0 };
    static const QMetaData slot_tbl[] = {
	{ "OnOnItem(QListViewItem*)", &slot_0, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"ADN_ListView_Sensors", parentObject,
	slot_tbl, 1,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_ADN_ListView_Sensors.setMetaObject( metaObj );
    return metaObj;
}

void* ADN_ListView_Sensors::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "ADN_ListView_Sensors" ) )
	return this;
    return ADN_ListView::qt_cast( clname );
}

bool ADN_ListView_Sensors::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: OnOnItem((QListViewItem*)static_QUType_ptr.get(_o+1)); break;
    default:
	return ADN_ListView::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool ADN_ListView_Sensors::qt_emit( int _id, QUObject* _o )
{
    return ADN_ListView::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool ADN_ListView_Sensors::qt_property( int id, int f, QVariant* v)
{
    return ADN_ListView::qt_property( id, f, v);
}

bool ADN_ListView_Sensors::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
