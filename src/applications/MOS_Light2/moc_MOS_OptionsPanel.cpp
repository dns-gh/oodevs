/****************************************************************************
** MOS_OptionsPanel meta object code from reading C++ file 'MOS_OptionsPanel.h'
**
** Created: ven. 22. juil. 13:35:03 2005
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

const char *MOS_OptionsPanel::className() const
{
    return "MOS_OptionsPanel";
}

QMetaObject *MOS_OptionsPanel::metaObj = 0;
static QMetaObjectCleanUp cleanUp_MOS_OptionsPanel( "MOS_OptionsPanel", &MOS_OptionsPanel::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString MOS_OptionsPanel::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_OptionsPanel", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString MOS_OptionsPanel::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_OptionsPanel", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* MOS_OptionsPanel::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QWidget::staticMetaObject();
    static const QUMethod slot_0 = {"OnCheckpoint", 0, 0 };
    static const QUMethod slot_1 = {"Apply", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "OnCheckpoint()", &slot_0, QMetaData::Private },
	{ "Apply()", &slot_1, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"MOS_OptionsPanel", parentObject,
	slot_tbl, 2,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_MOS_OptionsPanel.setMetaObject( metaObj );
    return metaObj;
}

void* MOS_OptionsPanel::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "MOS_OptionsPanel" ) )
	return this;
    return QWidget::qt_cast( clname );
}

bool MOS_OptionsPanel::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: OnCheckpoint(); break;
    case 1: Apply(); break;
    default:
	return QWidget::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool MOS_OptionsPanel::qt_emit( int _id, QUObject* _o )
{
    return QWidget::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool MOS_OptionsPanel::qt_property( int id, int f, QVariant* v)
{
    return QWidget::qt_property( id, f, v);
}

bool MOS_OptionsPanel::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
