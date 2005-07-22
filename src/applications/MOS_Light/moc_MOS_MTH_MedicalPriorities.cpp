/****************************************************************************
** MOS_MTH_MedicalPriorities meta object code from reading C++ file 'MOS_MTH_MedicalPriorities.h'
**
** Created: jeu. 21. juil. 13:57:56 2005
**      by: The Qt MOC ($Id: $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "MOS_MTH_MedicalPriorities.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *MOS_MTH_MedicalPriorities::className() const
{
    return "MOS_MTH_MedicalPriorities";
}

QMetaObject *MOS_MTH_MedicalPriorities::metaObj = 0;
static QMetaObjectCleanUp cleanUp_MOS_MTH_MedicalPriorities( "MOS_MTH_MedicalPriorities", &MOS_MTH_MedicalPriorities::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString MOS_MTH_MedicalPriorities::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_MTH_MedicalPriorities", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString MOS_MTH_MedicalPriorities::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_MTH_MedicalPriorities", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* MOS_MTH_MedicalPriorities::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QWidget::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "pItem", &static_QUType_ptr, "QListViewItem", QUParameter::In },
	{ "pos", &static_QUType_varptr, "\x0e", QUParameter::In },
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_0 = {"OnContextMenu", 3, param_slot_0 };
    static const QMetaData slot_tbl[] = {
	{ "OnContextMenu(QListViewItem*,const QPoint&,int)", &slot_0, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"MOS_MTH_MedicalPriorities", parentObject,
	slot_tbl, 1,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_MOS_MTH_MedicalPriorities.setMetaObject( metaObj );
    return metaObj;
}

void* MOS_MTH_MedicalPriorities::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "MOS_MTH_MedicalPriorities" ) )
	return this;
    if ( !qstrcmp( clname, "MOS_MissionTool_ABC" ) )
	return (MOS_MissionTool_ABC*)this;
    return QWidget::qt_cast( clname );
}

bool MOS_MTH_MedicalPriorities::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: OnContextMenu((QListViewItem*)static_QUType_ptr.get(_o+1),(const QPoint&)*((const QPoint*)static_QUType_ptr.get(_o+2)),(int)static_QUType_int.get(_o+3)); break;
    default:
	return QWidget::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool MOS_MTH_MedicalPriorities::qt_emit( int _id, QUObject* _o )
{
    return QWidget::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool MOS_MTH_MedicalPriorities::qt_property( int id, int f, QVariant* v)
{
    return QWidget::qt_property( id, f, v);
}

bool MOS_MTH_MedicalPriorities::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
