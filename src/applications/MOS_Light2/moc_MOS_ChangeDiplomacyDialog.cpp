/****************************************************************************
** MOS_ChangeDiplomacyDialog meta object code from reading C++ file 'MOS_ChangeDiplomacyDialog.h'
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

const char *MOS_ChangeDiplomacyDialog::className() const
{
    return "MOS_ChangeDiplomacyDialog";
}

QMetaObject *MOS_ChangeDiplomacyDialog::metaObj = 0;
static QMetaObjectCleanUp cleanUp_MOS_ChangeDiplomacyDialog( "MOS_ChangeDiplomacyDialog", &MOS_ChangeDiplomacyDialog::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString MOS_ChangeDiplomacyDialog::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_ChangeDiplomacyDialog", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString MOS_ChangeDiplomacyDialog::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_ChangeDiplomacyDialog", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* MOS_ChangeDiplomacyDialog::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QDialog::staticMetaObject();
    static const QUMethod slot_0 = {"Validate", 0, 0 };
    static const QUMethod slot_1 = {"Reject", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "Validate()", &slot_0, QMetaData::Private },
	{ "Reject()", &slot_1, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"MOS_ChangeDiplomacyDialog", parentObject,
	slot_tbl, 2,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_MOS_ChangeDiplomacyDialog.setMetaObject( metaObj );
    return metaObj;
}

void* MOS_ChangeDiplomacyDialog::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "MOS_ChangeDiplomacyDialog" ) )
	return this;
    return QDialog::qt_cast( clname );
}

bool MOS_ChangeDiplomacyDialog::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: Validate(); break;
    case 1: Reject(); break;
    default:
	return QDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool MOS_ChangeDiplomacyDialog::qt_emit( int _id, QUObject* _o )
{
    return QDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool MOS_ChangeDiplomacyDialog::qt_property( int id, int f, QVariant* v)
{
    return QDialog::qt_property( id, f, v);
}

bool MOS_ChangeDiplomacyDialog::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
