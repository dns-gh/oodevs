/****************************************************************************
** MOS_RCEditor meta object code from reading C++ file 'MOS_RCEditor.h'
**
** Created: lun. 11. juil. 18:50:11 2005
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

const char *MOS_RCEditor::className() const
{
    return "MOS_RCEditor";
}

QMetaObject *MOS_RCEditor::metaObj = 0;
static QMetaObjectCleanUp cleanUp_MOS_RCEditor( "MOS_RCEditor", &MOS_RCEditor::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString MOS_RCEditor::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_RCEditor", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString MOS_RCEditor::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_RCEditor", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* MOS_RCEditor::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QFrame::staticMetaObject();
    metaObj = QMetaObject::new_metaobject(
	"MOS_RCEditor", parentObject,
	0, 0,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_MOS_RCEditor.setMetaObject( metaObj );
    return metaObj;
}

void* MOS_RCEditor::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "MOS_RCEditor" ) )
	return this;
    return QFrame::qt_cast( clname );
}

bool MOS_RCEditor::qt_invoke( int _id, QUObject* _o )
{
    return QFrame::qt_invoke(_id,_o);
}

bool MOS_RCEditor::qt_emit( int _id, QUObject* _o )
{
    return QFrame::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool MOS_RCEditor::qt_property( int id, int f, QVariant* v)
{
    return QFrame::qt_property( id, f, v);
}

bool MOS_RCEditor::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
