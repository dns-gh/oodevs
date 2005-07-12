/****************************************************************************
** MOS_TraceEditor meta object code from reading C++ file 'MOS_TraceEditor.h'
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

const char *MOS_TraceEditor::className() const
{
    return "MOS_TraceEditor";
}

QMetaObject *MOS_TraceEditor::metaObj = 0;
static QMetaObjectCleanUp cleanUp_MOS_TraceEditor( "MOS_TraceEditor", &MOS_TraceEditor::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString MOS_TraceEditor::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_TraceEditor", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString MOS_TraceEditor::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_TraceEditor", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* MOS_TraceEditor::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QFrame::staticMetaObject();
    metaObj = QMetaObject::new_metaobject(
	"MOS_TraceEditor", parentObject,
	0, 0,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_MOS_TraceEditor.setMetaObject( metaObj );
    return metaObj;
}

void* MOS_TraceEditor::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "MOS_TraceEditor" ) )
	return this;
    return QFrame::qt_cast( clname );
}

bool MOS_TraceEditor::qt_invoke( int _id, QUObject* _o )
{
    return QFrame::qt_invoke(_id,_o);
}

bool MOS_TraceEditor::qt_emit( int _id, QUObject* _o )
{
    return QFrame::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool MOS_TraceEditor::qt_property( int id, int f, QVariant* v)
{
    return QFrame::qt_property( id, f, v);
}

bool MOS_TraceEditor::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
