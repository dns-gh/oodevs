/****************************************************************************
** MT_GLDLayerDialog meta object code from reading C++ file 'MT_GLDLayerDialog.h'
**
** Created: mar. 12. juil. 12:42:02 2005
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

const char *MT_GLDLayerDialog::className() const
{
    return "MT_GLDLayerDialog";
}

QMetaObject *MT_GLDLayerDialog::metaObj = 0;
static QMetaObjectCleanUp cleanUp_MT_GLDLayerDialog( "MT_GLDLayerDialog", &MT_GLDLayerDialog::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString MT_GLDLayerDialog::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MT_GLDLayerDialog", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString MT_GLDLayerDialog::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MT_GLDLayerDialog", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* MT_GLDLayerDialog::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QDialog::staticMetaObject();
    static const QUMethod slot_0 = {"OnApply", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "OnApply()", &slot_0, QMetaData::Private }
    };
    static const QUParameter param_signal_0[] = {
	{ "layer", &static_QUType_ptr, "MT_GLDLayer", QUParameter::InOut }
    };
    static const QUMethod signal_0 = {"LayerEdited", 1, param_signal_0 };
    static const QMetaData signal_tbl[] = {
	{ "LayerEdited(MT_GLDLayer&)", &signal_0, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"MT_GLDLayerDialog", parentObject,
	slot_tbl, 1,
	signal_tbl, 1,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_MT_GLDLayerDialog.setMetaObject( metaObj );
    return metaObj;
}

void* MT_GLDLayerDialog::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "MT_GLDLayerDialog" ) )
	return this;
    return QDialog::qt_cast( clname );
}

#include <qobjectdefs.h>
#include <qsignalslotimp.h>

// SIGNAL LayerEdited
void MT_GLDLayerDialog::LayerEdited( MT_GLDLayer& t0 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 0 );
    if ( !clist )
	return;
    QUObject o[2];
    static_QUType_ptr.set(o+1,&t0);
    activate_signal( clist, o );
}

bool MT_GLDLayerDialog::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: OnApply(); break;
    default:
	return QDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool MT_GLDLayerDialog::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: LayerEdited((MT_GLDLayer&)*((MT_GLDLayer*)static_QUType_ptr.get(_o+1))); break;
    default:
	return QDialog::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool MT_GLDLayerDialog::qt_property( int id, int f, QVariant* v)
{
    return QDialog::qt_property( id, f, v);
}

bool MT_GLDLayerDialog::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
