/****************************************************************************
** MOS_MTH_PointList_Editor meta object code from reading C++ file 'MOS_MTH_PointList_Editor.h'
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

const char *MOS_MTH_PointList_Editor::className() const
{
    return "MOS_MTH_PointList_Editor";
}

QMetaObject *MOS_MTH_PointList_Editor::metaObj = 0;
static QMetaObjectCleanUp cleanUp_MOS_MTH_PointList_Editor( "MOS_MTH_PointList_Editor", &MOS_MTH_PointList_Editor::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString MOS_MTH_PointList_Editor::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_MTH_PointList_Editor", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString MOS_MTH_PointList_Editor::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_MTH_PointList_Editor", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* MOS_MTH_PointList_Editor::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QWidget::staticMetaObject();
    static const QUMethod slot_0 = {"SlotOk", 0, 0 };
    static const QUMethod slot_1 = {"SlotCancel", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "SlotOk()", &slot_0, QMetaData::Private },
	{ "SlotCancel()", &slot_1, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"MOS_MTH_PointList_Editor", parentObject,
	slot_tbl, 2,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_MOS_MTH_PointList_Editor.setMetaObject( metaObj );
    return metaObj;
}

void* MOS_MTH_PointList_Editor::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "MOS_MTH_PointList_Editor" ) )
	return this;
    return QWidget::qt_cast( clname );
}

bool MOS_MTH_PointList_Editor::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: SlotOk(); break;
    case 1: SlotCancel(); break;
    default:
	return QWidget::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool MOS_MTH_PointList_Editor::qt_emit( int _id, QUObject* _o )
{
    return QWidget::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool MOS_MTH_PointList_Editor::qt_property( int id, int f, QVariant* v)
{
    return QWidget::qt_property( id, f, v);
}

bool MOS_MTH_PointList_Editor::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
