/****************************************************************************
** ADN_Table meta object code from reading C++ file 'ADN_Table.h'
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

const char *ADN_Table::className() const
{
    return "ADN_Table";
}

QMetaObject *ADN_Table::metaObj = 0;
static QMetaObjectCleanUp cleanUp_ADN_Table( "ADN_Table", &ADN_Table::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString ADN_Table::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ADN_Table", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString ADN_Table::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ADN_Table", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* ADN_Table::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QTable::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "nRow", &static_QUType_int, 0, QUParameter::In },
	{ "nCol", &static_QUType_int, 0, QUParameter::In },
	{ "pt", &static_QUType_varptr, "\x0e", QUParameter::In }
    };
    static const QUMethod slot_0 = {"OnContextMenu", 3, param_slot_0 };
    static const QUMethod slot_1 = {"UpdateEnableState", 0, 0 };
    static const QUParameter param_slot_2[] = {
	{ "row", &static_QUType_int, 0, QUParameter::In },
	{ "col", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_2 = {"doValueChanged", 2, param_slot_2 };
    static const QMetaData slot_tbl[] = {
	{ "OnContextMenu(int,int,const QPoint&)", &slot_0, QMetaData::Protected },
	{ "UpdateEnableState()", &slot_1, QMetaData::Protected },
	{ "doValueChanged(int,int)", &slot_2, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"ADN_Table", parentObject,
	slot_tbl, 3,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_ADN_Table.setMetaObject( metaObj );
    return metaObj;
}

void* ADN_Table::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "ADN_Table" ) )
	return this;
    if ( !qstrcmp( clname, "ADN_Gfx_ABC" ) )
	return (ADN_Gfx_ABC*)this;
    return QTable::qt_cast( clname );
}

bool ADN_Table::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: OnContextMenu((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2),(const QPoint&)*((const QPoint*)static_QUType_ptr.get(_o+3))); break;
    case 1: UpdateEnableState(); break;
    case 2: doValueChanged((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2)); break;
    default:
	return QTable::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool ADN_Table::qt_emit( int _id, QUObject* _o )
{
    return QTable::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool ADN_Table::qt_property( int id, int f, QVariant* v)
{
    return QTable::qt_property( id, f, v);
}

bool ADN_Table::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
