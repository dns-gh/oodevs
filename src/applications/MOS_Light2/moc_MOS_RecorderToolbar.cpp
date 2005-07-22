/****************************************************************************
** MOS_RecorderToolbar meta object code from reading C++ file 'MOS_RecorderToolbar.h'
**
** Created: ven. 22. juil. 13:35:02 2005
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

const char *MOS_RecorderToolbar::className() const
{
    return "MOS_RecorderToolbar";
}

QMetaObject *MOS_RecorderToolbar::metaObj = 0;
static QMetaObjectCleanUp cleanUp_MOS_RecorderToolbar( "MOS_RecorderToolbar", &MOS_RecorderToolbar::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString MOS_RecorderToolbar::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_RecorderToolbar", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString MOS_RecorderToolbar::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_RecorderToolbar", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* MOS_RecorderToolbar::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QToolBar::staticMetaObject();
    static const QUMethod slot_0 = {"Play", 0, 0 };
    static const QUMethod slot_1 = {"Stop", 0, 0 };
    static const QUMethod slot_2 = {"Record", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "Play()", &slot_0, QMetaData::Private },
	{ "Stop()", &slot_1, QMetaData::Private },
	{ "Record()", &slot_2, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"MOS_RecorderToolbar", parentObject,
	slot_tbl, 3,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_MOS_RecorderToolbar.setMetaObject( metaObj );
    return metaObj;
}

void* MOS_RecorderToolbar::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "MOS_RecorderToolbar" ) )
	return this;
    return QToolBar::qt_cast( clname );
}

bool MOS_RecorderToolbar::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: Play(); break;
    case 1: Stop(); break;
    case 2: Record(); break;
    default:
	return QToolBar::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool MOS_RecorderToolbar::qt_emit( int _id, QUObject* _o )
{
    return QToolBar::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool MOS_RecorderToolbar::qt_property( int id, int f, QVariant* v)
{
    return QToolBar::qt_property( id, f, v);
}

bool MOS_RecorderToolbar::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
