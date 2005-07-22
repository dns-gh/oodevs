/****************************************************************************
** MOS_ConnectDialog meta object code from reading C++ file 'MOS_ConnectDialog.h'
**
** Created: jeu. 21. juil. 13:57:40 2005
**      by: The Qt MOC ($Id: $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "MOS_ConnectDialog.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *MOS_ConnectDialog::className() const
{
    return "MOS_ConnectDialog";
}

QMetaObject *MOS_ConnectDialog::metaObj = 0;
static QMetaObjectCleanUp cleanUp_MOS_ConnectDialog( "MOS_ConnectDialog", &MOS_ConnectDialog::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString MOS_ConnectDialog::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_ConnectDialog", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString MOS_ConnectDialog::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_ConnectDialog", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* MOS_ConnectDialog::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QDialog::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "bTest", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_0 = {"SlotSetModeTest", 1, param_slot_0 };
    static const QUParameter param_slot_1[] = {
	{ "bReplay", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_1 = {"SlotSetModeReplay", 1, param_slot_1 };
    static const QUParameter param_slot_2[] = {
	{ "bRec", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_2 = {"SlotSetModeRec", 1, param_slot_2 };
    static const QUMethod slot_3 = {"SlotSelectOutDir", 0, 0 };
    static const QUParameter param_slot_4[] = {
	{ "strName", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_4 = {"SlotOutDirChanged", 1, param_slot_4 };
    static const QUMethod slot_5 = {"Validate", 0, 0 };
    static const QUMethod slot_6 = {"Reject", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "SlotSetModeTest(bool)", &slot_0, QMetaData::Private },
	{ "SlotSetModeReplay(bool)", &slot_1, QMetaData::Private },
	{ "SlotSetModeRec(bool)", &slot_2, QMetaData::Private },
	{ "SlotSelectOutDir()", &slot_3, QMetaData::Private },
	{ "SlotOutDirChanged(const QString&)", &slot_4, QMetaData::Private },
	{ "Validate()", &slot_5, QMetaData::Private },
	{ "Reject()", &slot_6, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"MOS_ConnectDialog", parentObject,
	slot_tbl, 7,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_MOS_ConnectDialog.setMetaObject( metaObj );
    return metaObj;
}

void* MOS_ConnectDialog::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "MOS_ConnectDialog" ) )
	return this;
    return QDialog::qt_cast( clname );
}

bool MOS_ConnectDialog::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: SlotSetModeTest((bool)static_QUType_bool.get(_o+1)); break;
    case 1: SlotSetModeReplay((bool)static_QUType_bool.get(_o+1)); break;
    case 2: SlotSetModeRec((bool)static_QUType_bool.get(_o+1)); break;
    case 3: SlotSelectOutDir(); break;
    case 4: SlotOutDirChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 5: Validate(); break;
    case 6: Reject(); break;
    default:
	return QDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool MOS_ConnectDialog::qt_emit( int _id, QUObject* _o )
{
    return QDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool MOS_ConnectDialog::qt_property( int id, int f, QVariant* v)
{
    return QDialog::qt_property( id, f, v);
}

bool MOS_ConnectDialog::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
