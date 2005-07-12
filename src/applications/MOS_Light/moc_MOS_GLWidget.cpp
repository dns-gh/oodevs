/****************************************************************************
** MOS_GLWidget meta object code from reading C++ file 'MOS_GLWidget.h'
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

const char *MOS_GLWidget::className() const
{
    return "MOS_GLWidget";
}

QMetaObject *MOS_GLWidget::metaObj = 0;
static QMetaObjectCleanUp cleanUp_MOS_GLWidget( "MOS_GLWidget", &MOS_GLWidget::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString MOS_GLWidget::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_GLWidget", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString MOS_GLWidget::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_GLWidget", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* MOS_GLWidget::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QGLWidget::staticMetaObject();
    static const QUMethod slot_0 = {"SlotSetXTranslationLeft", 0, 0 };
    static const QUMethod slot_1 = {"SlotSetXTranslationRight", 0, 0 };
    static const QUMethod slot_2 = {"SlotSetYTranslationUp", 0, 0 };
    static const QUMethod slot_3 = {"SlotSetYTranslationDown", 0, 0 };
    static const QUMethod slot_4 = {"SlotSetCenter", 0, 0 };
    static const QUParameter param_slot_5[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_5 = {"SlotSetZoom", 1, param_slot_5 };
    static const QMetaData slot_tbl[] = {
	{ "SlotSetXTranslationLeft()", &slot_0, QMetaData::Public },
	{ "SlotSetXTranslationRight()", &slot_1, QMetaData::Public },
	{ "SlotSetYTranslationUp()", &slot_2, QMetaData::Public },
	{ "SlotSetYTranslationDown()", &slot_3, QMetaData::Public },
	{ "SlotSetCenter()", &slot_4, QMetaData::Public },
	{ "SlotSetZoom(int)", &slot_5, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"MOS_GLWidget", parentObject,
	slot_tbl, 6,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_MOS_GLWidget.setMetaObject( metaObj );
    return metaObj;
}

void* MOS_GLWidget::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "MOS_GLWidget" ) )
	return this;
    return QGLWidget::qt_cast( clname );
}

bool MOS_GLWidget::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: SlotSetXTranslationLeft(); break;
    case 1: SlotSetXTranslationRight(); break;
    case 2: SlotSetYTranslationUp(); break;
    case 3: SlotSetYTranslationDown(); break;
    case 4: SlotSetCenter(); break;
    case 5: SlotSetZoom((int)static_QUType_int.get(_o+1)); break;
    default:
	return QGLWidget::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool MOS_GLWidget::qt_emit( int _id, QUObject* _o )
{
    return QGLWidget::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool MOS_GLWidget::qt_property( int id, int f, QVariant* v)
{
    return QGLWidget::qt_property( id, f, v);
}

bool MOS_GLWidget::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
