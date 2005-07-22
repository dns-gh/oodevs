/****************************************************************************
** ADN_Automata_SubUnitsTable meta object code from reading C++ file 'ADN_Automata_SubUnitsTable.h'
**
** Created: jeu. 21. juil. 11:15:29 2005
**      by: The Qt MOC ($Id: $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "ADN_Automata_SubUnitsTable.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *ADN_Automata_SubUnitsTable::className() const
{
    return "ADN_Automata_SubUnitsTable";
}

QMetaObject *ADN_Automata_SubUnitsTable::metaObj = 0;
static QMetaObjectCleanUp cleanUp_ADN_Automata_SubUnitsTable( "ADN_Automata_SubUnitsTable", &ADN_Automata_SubUnitsTable::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString ADN_Automata_SubUnitsTable::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ADN_Automata_SubUnitsTable", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString ADN_Automata_SubUnitsTable::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ADN_Automata_SubUnitsTable", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* ADN_Automata_SubUnitsTable::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = ADN_Table2::staticMetaObject();
    static const QUMethod slot_0 = {"MenuListItemSelected", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "MenuListItemSelected()", &slot_0, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"ADN_Automata_SubUnitsTable", parentObject,
	slot_tbl, 1,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_ADN_Automata_SubUnitsTable.setMetaObject( metaObj );
    return metaObj;
}

void* ADN_Automata_SubUnitsTable::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "ADN_Automata_SubUnitsTable" ) )
	return this;
    return ADN_Table2::qt_cast( clname );
}

bool ADN_Automata_SubUnitsTable::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: MenuListItemSelected(); break;
    default:
	return ADN_Table2::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool ADN_Automata_SubUnitsTable::qt_emit( int _id, QUObject* _o )
{
    return ADN_Table2::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool ADN_Automata_SubUnitsTable::qt_property( int id, int f, QVariant* v)
{
    return ADN_Table2::qt_property( id, f, v);
}

bool ADN_Automata_SubUnitsTable::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
