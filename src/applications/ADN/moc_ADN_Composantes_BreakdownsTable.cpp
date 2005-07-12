/****************************************************************************
** ADN_Composantes_BreakdownsTable meta object code from reading C++ file 'ADN_Composantes_BreakdownsTable.h'
**
** Created: jeu. 7. juil. 13:46:09 2005
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

const char *ADN_Composantes_BreakdownsTable::className() const
{
    return "ADN_Composantes_BreakdownsTable";
}

QMetaObject *ADN_Composantes_BreakdownsTable::metaObj = 0;
static QMetaObjectCleanUp cleanUp_ADN_Composantes_BreakdownsTable( "ADN_Composantes_BreakdownsTable", &ADN_Composantes_BreakdownsTable::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString ADN_Composantes_BreakdownsTable::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ADN_Composantes_BreakdownsTable", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString ADN_Composantes_BreakdownsTable::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ADN_Composantes_BreakdownsTable", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* ADN_Composantes_BreakdownsTable::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = ADN_Table2::staticMetaObject();
    static const QUMethod slot_0 = {"OnModified", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "OnModified()", &slot_0, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"ADN_Composantes_BreakdownsTable", parentObject,
	slot_tbl, 1,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_ADN_Composantes_BreakdownsTable.setMetaObject( metaObj );
    return metaObj;
}

void* ADN_Composantes_BreakdownsTable::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "ADN_Composantes_BreakdownsTable" ) )
	return this;
    return ADN_Table2::qt_cast( clname );
}

bool ADN_Composantes_BreakdownsTable::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: OnModified(); break;
    default:
	return ADN_Table2::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool ADN_Composantes_BreakdownsTable::qt_emit( int _id, QUObject* _o )
{
    return ADN_Table2::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool ADN_Composantes_BreakdownsTable::qt_property( int id, int f, QVariant* v)
{
    return ADN_Table2::qt_property( id, f, v);
}

bool ADN_Composantes_BreakdownsTable::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
