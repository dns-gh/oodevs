/****************************************************************************
** MOS_InfoPanel meta object code from reading C++ file 'MOS_InfoPanel.h'
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

const char *MOS_InfoPanel::className() const
{
    return "MOS_InfoPanel";
}

QMetaObject *MOS_InfoPanel::metaObj = 0;
static QMetaObjectCleanUp cleanUp_MOS_InfoPanel( "MOS_InfoPanel", &MOS_InfoPanel::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString MOS_InfoPanel::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_InfoPanel", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString MOS_InfoPanel::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_InfoPanel", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* MOS_InfoPanel::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QWidgetStack::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "selectedElement", &static_QUType_ptr, "MOS_SelectedElement", QUParameter::InOut }
    };
    static const QUMethod slot_0 = {"SetSelectedElement", 1, param_slot_0 };
    static const QUMethod slot_1 = {"ClearAll", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "SetSelectedElement(MOS_SelectedElement&)", &slot_0, QMetaData::Public },
	{ "ClearAll()", &slot_1, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"MOS_InfoPanel", parentObject,
	slot_tbl, 2,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_MOS_InfoPanel.setMetaObject( metaObj );
    return metaObj;
}

void* MOS_InfoPanel::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "MOS_InfoPanel" ) )
	return this;
    return QWidgetStack::qt_cast( clname );
}

bool MOS_InfoPanel::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: SetSelectedElement((MOS_SelectedElement&)*((MOS_SelectedElement*)static_QUType_ptr.get(_o+1))); break;
    case 1: ClearAll(); break;
    default:
	return QWidgetStack::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool MOS_InfoPanel::qt_emit( int _id, QUObject* _o )
{
    return QWidgetStack::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool MOS_InfoPanel::qt_property( int id, int f, QVariant* v)
{
    return QWidgetStack::qt_property( id, f, v);
}

bool MOS_InfoPanel::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
