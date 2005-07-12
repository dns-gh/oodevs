/****************************************************************************
** ADN_BoolEditor meta object code from reading C++ file 'ADN_BoolEditor.h'
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

const char *ADN_BoolEditor::className() const
{
    return "ADN_BoolEditor";
}

QMetaObject *ADN_BoolEditor::metaObj = 0;
static QMetaObjectCleanUp cleanUp_ADN_BoolEditor( "ADN_BoolEditor", &ADN_BoolEditor::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString ADN_BoolEditor::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ADN_BoolEditor", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString ADN_BoolEditor::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ADN_BoolEditor", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* ADN_BoolEditor::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QFrame::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ 0, &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_0 = {"SetState", 1, param_slot_0 };
    static const QMetaData slot_tbl[] = {
	{ "SetState(bool)", &slot_0, QMetaData::Public }
    };
    static const QUParameter param_signal_0[] = {
	{ 0, &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod signal_0 = {"StateChanged", 1, param_signal_0 };
    static const QMetaData signal_tbl[] = {
	{ "StateChanged(bool)", &signal_0, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"ADN_BoolEditor", parentObject,
	slot_tbl, 1,
	signal_tbl, 1,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_ADN_BoolEditor.setMetaObject( metaObj );
    return metaObj;
}

void* ADN_BoolEditor::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "ADN_BoolEditor" ) )
	return this;
    return QFrame::qt_cast( clname );
}

// SIGNAL StateChanged
void ADN_BoolEditor::StateChanged( bool t0 )
{
    activate_signal_bool( staticMetaObject()->signalOffset() + 0, t0 );
}

bool ADN_BoolEditor::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: SetState((bool)static_QUType_bool.get(_o+1)); break;
    default:
	return QFrame::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool ADN_BoolEditor::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: StateChanged((bool)static_QUType_bool.get(_o+1)); break;
    default:
	return QFrame::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool ADN_BoolEditor::qt_property( int id, int f, QVariant* v)
{
    return QFrame::qt_property( id, f, v);
}

bool ADN_BoolEditor::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
