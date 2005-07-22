/****************************************************************************
** MOS_Mission_ABC meta object code from reading C++ file 'MOS_Mission_ABC.h'
**
** Created: jeu. 21. juil. 14:07:26 2005
**      by: The Qt MOC ($Id: $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "MOS_Mission_ABC.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *MOS_Mission_ABC::className() const
{
    return "MOS_Mission_ABC";
}

QMetaObject *MOS_Mission_ABC::metaObj = 0;
static QMetaObjectCleanUp cleanUp_MOS_Mission_ABC( "MOS_Mission_ABC", &MOS_Mission_ABC::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString MOS_Mission_ABC::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_Mission_ABC", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString MOS_Mission_ABC::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_Mission_ABC", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* MOS_Mission_ABC::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QWidget::staticMetaObject();
    static const QUMethod slot_0 = {"SlotValidate", 0, 0 };
    static const QUMethod slot_1 = {"SlotCancel", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "SlotValidate()", &slot_0, QMetaData::Protected },
	{ "SlotCancel()", &slot_1, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"MOS_Mission_ABC", parentObject,
	slot_tbl, 2,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_MOS_Mission_ABC.setMetaObject( metaObj );
    return metaObj;
}

void* MOS_Mission_ABC::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "MOS_Mission_ABC" ) )
	return this;
    if ( !qstrcmp( clname, "MOS_GLMouse_ABC" ) )
	return (MOS_GLMouse_ABC*)this;
    return QWidget::qt_cast( clname );
}

bool MOS_Mission_ABC::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: SlotValidate(); break;
    case 1: SlotCancel(); break;
    default:
	return QWidget::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool MOS_Mission_ABC::qt_emit( int _id, QUObject* _o )
{
    return QWidget::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool MOS_Mission_ABC::qt_property( int id, int f, QVariant* v)
{
    return QWidget::qt_property( id, f, v);
}

bool MOS_Mission_ABC::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
