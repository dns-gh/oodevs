/****************************************************************************
** ADN_TableDialog meta object code from reading C++ file 'ADN_TableDialog.h'
**
** Created: jeu. 21. juil. 11:15:55 2005
**      by: The Qt MOC ($Id: $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "ADN_TableDialog.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *ADN_TableDialog::className() const
{
    return "ADN_TableDialog";
}

QMetaObject *ADN_TableDialog::metaObj = 0;
static QMetaObjectCleanUp cleanUp_ADN_TableDialog( "ADN_TableDialog", &ADN_TableDialog::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString ADN_TableDialog::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ADN_TableDialog", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString ADN_TableDialog::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ADN_TableDialog", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* ADN_TableDialog::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QDialog::staticMetaObject();
    static const QUMethod slot_0 = {"OnContextMenu", 0, 0 };
    static const QUMethod slot_1 = {"PrintTable", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "OnContextMenu()", &slot_0, QMetaData::Private },
	{ "PrintTable()", &slot_1, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"ADN_TableDialog", parentObject,
	slot_tbl, 2,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_ADN_TableDialog.setMetaObject( metaObj );
    return metaObj;
}

void* ADN_TableDialog::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "ADN_TableDialog" ) )
	return this;
    return QDialog::qt_cast( clname );
}

bool ADN_TableDialog::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: OnContextMenu(); break;
    case 1: PrintTable(); break;
    default:
	return QDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool ADN_TableDialog::qt_emit( int _id, QUObject* _o )
{
    return QDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool ADN_TableDialog::qt_property( int id, int f, QVariant* v)
{
    return QDialog::qt_property( id, f, v);
}

bool ADN_TableDialog::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
