/****************************************************************************
** MOS_OrderConduiteMissionDialog meta object code from reading C++ file 'MOS_OrderConduiteMissionDialog.h'
**
** Created: lun. 11. juil. 18:50:12 2005
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

const char *MOS_OrderConduiteMissionDialog::className() const
{
    return "MOS_OrderConduiteMissionDialog";
}

QMetaObject *MOS_OrderConduiteMissionDialog::metaObj = 0;
static QMetaObjectCleanUp cleanUp_MOS_OrderConduiteMissionDialog( "MOS_OrderConduiteMissionDialog", &MOS_OrderConduiteMissionDialog::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString MOS_OrderConduiteMissionDialog::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_OrderConduiteMissionDialog", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString MOS_OrderConduiteMissionDialog::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_OrderConduiteMissionDialog", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* MOS_OrderConduiteMissionDialog::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QDialog::staticMetaObject();
    static const QUMethod slot_0 = {"SlotValidate", 0, 0 };
    static const QUMethod slot_1 = {"SlotCancel", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "SlotValidate()", &slot_0, QMetaData::Private },
	{ "SlotCancel()", &slot_1, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"MOS_OrderConduiteMissionDialog", parentObject,
	slot_tbl, 2,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_MOS_OrderConduiteMissionDialog.setMetaObject( metaObj );
    return metaObj;
}

void* MOS_OrderConduiteMissionDialog::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "MOS_OrderConduiteMissionDialog" ) )
	return this;
    return QDialog::qt_cast( clname );
}

bool MOS_OrderConduiteMissionDialog::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: SlotValidate(); break;
    case 1: SlotCancel(); break;
    default:
	return QDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool MOS_OrderConduiteMissionDialog::qt_emit( int _id, QUObject* _o )
{
    return QDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool MOS_OrderConduiteMissionDialog::qt_property( int id, int f, QVariant* v)
{
    return QDialog::qt_property( id, f, v);
}

bool MOS_OrderConduiteMissionDialog::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
