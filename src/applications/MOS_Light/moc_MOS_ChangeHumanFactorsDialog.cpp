/****************************************************************************
** MOS_ChangeHumanFactorsDialog meta object code from reading C++ file 'MOS_ChangeHumanFactorsDialog.h'
**
** Created: jeu. 21. juil. 13:57:40 2005
**      by: The Qt MOC ($Id: $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "MOS_ChangeHumanFactorsDialog.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *MOS_ChangeHumanFactorsDialog::className() const
{
    return "MOS_ChangeHumanFactorsDialog";
}

QMetaObject *MOS_ChangeHumanFactorsDialog::metaObj = 0;
static QMetaObjectCleanUp cleanUp_MOS_ChangeHumanFactorsDialog( "MOS_ChangeHumanFactorsDialog", &MOS_ChangeHumanFactorsDialog::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString MOS_ChangeHumanFactorsDialog::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_ChangeHumanFactorsDialog", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString MOS_ChangeHumanFactorsDialog::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_ChangeHumanFactorsDialog", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* MOS_ChangeHumanFactorsDialog::staticMetaObject()
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
	"MOS_ChangeHumanFactorsDialog", parentObject,
	slot_tbl, 2,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_MOS_ChangeHumanFactorsDialog.setMetaObject( metaObj );
    return metaObj;
}

void* MOS_ChangeHumanFactorsDialog::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "MOS_ChangeHumanFactorsDialog" ) )
	return this;
    return QDialog::qt_cast( clname );
}

bool MOS_ChangeHumanFactorsDialog::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: Validate(); break;
    case 1: Reject(); break;
    default:
	return QDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool MOS_ChangeHumanFactorsDialog::qt_emit( int _id, QUObject* _o )
{
    return QDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool MOS_ChangeHumanFactorsDialog::qt_property( int id, int f, QVariant* v)
{
    return QDialog::qt_property( id, f, v);
}

bool MOS_ChangeHumanFactorsDialog::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
