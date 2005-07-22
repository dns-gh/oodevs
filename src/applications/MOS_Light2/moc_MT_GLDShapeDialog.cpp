/****************************************************************************
** MT_GLDShapeDialog meta object code from reading C++ file 'MT_GLDShapeDialog.h'
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

const char *MT_GLDShapeDialog::className() const
{
    return "MT_GLDShapeDialog";
}

QMetaObject *MT_GLDShapeDialog::metaObj = 0;
static QMetaObjectCleanUp cleanUp_MT_GLDShapeDialog( "MT_GLDShapeDialog", &MT_GLDShapeDialog::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString MT_GLDShapeDialog::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MT_GLDShapeDialog", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString MT_GLDShapeDialog::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MT_GLDShapeDialog", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* MT_GLDShapeDialog::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QDialog::staticMetaObject();
    static const QUMethod slot_0 = {"OnApply", 0, 0 };
    static const QUMethod slot_1 = {"OnText", 0, 0 };
    static const QUMethod slot_2 = {"OnFontSize", 0, 0 };
    static const QUMethod slot_3 = {"OnLineColor", 0, 0 };
    static const QUMethod slot_4 = {"OnLineWidth", 0, 0 };
    static const QUMethod slot_5 = {"OnLinePattern", 0, 0 };
    static const QUMethod slot_6 = {"OnArrowStart", 0, 0 };
    static const QUMethod slot_7 = {"OnArrowEnd", 0, 0 };
    static const QUMethod slot_8 = {"OnSize", 0, 0 };
    static const QUMethod slot_9 = {"OnFillColor", 0, 0 };
    static const QUMethod slot_10 = {"OnFillOpacity", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "OnApply()", &slot_0, QMetaData::Private },
	{ "OnText()", &slot_1, QMetaData::Private },
	{ "OnFontSize()", &slot_2, QMetaData::Private },
	{ "OnLineColor()", &slot_3, QMetaData::Private },
	{ "OnLineWidth()", &slot_4, QMetaData::Private },
	{ "OnLinePattern()", &slot_5, QMetaData::Private },
	{ "OnArrowStart()", &slot_6, QMetaData::Private },
	{ "OnArrowEnd()", &slot_7, QMetaData::Private },
	{ "OnSize()", &slot_8, QMetaData::Private },
	{ "OnFillColor()", &slot_9, QMetaData::Private },
	{ "OnFillOpacity()", &slot_10, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"MT_GLDShapeDialog", parentObject,
	slot_tbl, 11,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_MT_GLDShapeDialog.setMetaObject( metaObj );
    return metaObj;
}

void* MT_GLDShapeDialog::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "MT_GLDShapeDialog" ) )
	return this;
    return QDialog::qt_cast( clname );
}

bool MT_GLDShapeDialog::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: OnApply(); break;
    case 1: OnText(); break;
    case 2: OnFontSize(); break;
    case 3: OnLineColor(); break;
    case 4: OnLineWidth(); break;
    case 5: OnLinePattern(); break;
    case 6: OnArrowStart(); break;
    case 7: OnArrowEnd(); break;
    case 8: OnSize(); break;
    case 9: OnFillColor(); break;
    case 10: OnFillOpacity(); break;
    default:
	return QDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool MT_GLDShapeDialog::qt_emit( int _id, QUObject* _o )
{
    return QDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool MT_GLDShapeDialog::qt_property( int id, int f, QVariant* v)
{
    return QDialog::qt_property( id, f, v);
}

bool MT_GLDShapeDialog::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
