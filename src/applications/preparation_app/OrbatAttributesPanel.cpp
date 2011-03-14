// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "OrbatAttributesPanel.h"
#include "moc_OrbatAttributesPanel.cpp"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/AttributeType.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/DictionaryExtensions.h"
#include "clients_kernel/DictionaryType.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/ExtensionType.h"
#include "clients_kernel/ExtensionTypes.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/Inhabitant_ABC.h"
#include "preparation/Tools.h"
#include <boost/lexical_cast.hpp>
#include <qtextcodec.h>
#include <qobjectlist.h>

using namespace kernel;

namespace
{
    std::string ReadLang()
    {
        QSettings settings;
        settings.setPath( "MASA Group", tools::translate( "Application", "SWORD" ) );
        return settings.readEntry( "/Common/Language", QTextCodec::locale() ).ascii();
    }
}

// -----------------------------------------------------------------------------
// Name: OrbatAttributesPanel constructor
// Created: JSR 2010-10-04
// -----------------------------------------------------------------------------
OrbatAttributesPanel::OrbatAttributesPanel( QMainWindow* parent, Controllers& controllers, const ExtensionTypes& extensions )
    : QDockWindow ( parent, "extensions" )
    , controllers_( controllers )
    , extensions_ ( extensions )
    , selected_   ( 0 )
    , pGroupBox_  ( 0 )
{
    setResizeEnabled( true );
    setCloseMode( QDockWindow::Always );
    setCaption( tr( "Extensions" ) );
    QScrollView* scrollView = new QScrollView( this );
    scrollView->setHScrollBarMode( QScrollView::AlwaysOff );
    scrollView->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Preferred );
    scrollView->setResizePolicy( QScrollView::AutoOneFit );
    scrollView->setFrameStyle( QFrame::Box | QFrame::Sunken );
    setWidget( scrollView );
    pMainLayout_ = new QVBox( scrollView->viewport() );
    scrollView->addChild( pMainLayout_ );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: OrbatAttributesPanel destructor
// Created: JSR 2010-10-04
// -----------------------------------------------------------------------------
OrbatAttributesPanel::~OrbatAttributesPanel()
{
    DeleteWidgets();
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: OrbatAttributesPanel::NotifySelected
// Created: JSR 2010-10-04
// -----------------------------------------------------------------------------
void OrbatAttributesPanel::NotifySelected( const Entity_ABC* element )
{
    if( selected_ == element )
        return;
    DeleteWidgets();
    ExtensionType* type = extensions_.tools::StringResolver< ExtensionType >::Find( "orbat-attributes" );
    if( type )
    {
        selected_ = const_cast< Entity_ABC* >( element );
        if( selected_ )
        {
            QString typeName = selected_->GetTypeName();
            ExtensionType::T_AttributesTypes attributes;
            if( typeName == Agent_ABC::typeName_ )
                type->GetAttributeTypes( "unit", attributes );
            else if( typeName == Automat_ABC::typeName_ )
                type->GetAttributeTypes( "automat", attributes );
            else if( typeName == Formation_ABC::typeName_ )
                type->GetAttributeTypes( "formation", attributes );
            else if( typeName == Population_ABC::typeName_ )
                type->GetAttributeTypes( "crowd", attributes );
            else if( typeName == Inhabitant_ABC::typeName_ )
                type->GetAttributeTypes( "population", attributes );
            if( attributes.size() )
            {
                pGroupBox_ = new QGroupBox( 1, Qt::Horizontal, tr( "Enabled" ), pMainLayout_ );
                pGroupBox_->setCheckable( true );
                pGroupBox_->setMargin( 5 );
                for( ExtensionType::CIT_AttributesTypes it = attributes.begin(); it != attributes.end(); ++it )
                    AddWidget( **it );
                const DictionaryExtensions* ext = selected_->Retrieve< DictionaryExtensions >();
                pGroupBox_->setChecked( ext ? ext->IsEnabled() : false );
                pGroupBox_->show();
                connect( pGroupBox_, SIGNAL( toggled( bool ) ), this, SLOT( OnActivationChanged( bool ) ) );
                return;
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: OrbatAttributesPanel::NotifyDeleted
// Created: JSR 2010-10-04
// -----------------------------------------------------------------------------
void OrbatAttributesPanel::NotifyDeleted( const Entity_ABC& element )
{
   if( selected_ == &element )
        DeleteWidgets();
}

namespace
{
    class QMinMaxValidator : public QValidator
    {
    public:
        QMinMaxValidator( QObject* parent, int min, int max, QValidator* validator = 0 )
            : QValidator( parent )
            , min_      ( min )
            , max_      ( max )
            , validator_( validator )
            , trailing_ ( 0 )
        {
            // NOTHING
        }

    virtual State validate( QString& input, int& pos ) const
    {
        if( validator_ )
        {
            input.remove( '*' );
            input.remove( ' ' );
        }
        else if( trailing_ && input.length() >= trailing_ && input.right( trailing_ ) == QString().fill( '*', trailing_ ) )
            input = input.left( input.length() - trailing_ );
        trailing_ = 0;
        int len = input.length();
        State state = Acceptable;
        if( validator_ )
            state = validator_->validate( input, pos );
        if( max_ != -1 && len > max_)
        {
            input.truncate( max_ );
            pos = std::min( pos, max_ );
        }
        if( min_ != -1 && len < min_ )
        {
            trailing_ = min_ - len;
            input.append( QString().fill( '*', trailing_ ) );
            if( state == Acceptable )
                state = Intermediate;
        }
        return state;
    }

    private:
        int min_;
        int max_;
        QValidator* validator_;
        mutable unsigned int trailing_;
    };

    void FillCombo( QComboBox& combo, const kernel::AttributeType& attribute, const tools::StringResolver< DictionaryType >& resolver )
    {
        std::string dictionary;
        std::string kind;
        std::string language;
        attribute.GetDictionaryValues( dictionary, kind, language );
        DictionaryType* dico = resolver.Find( dictionary );
        if( dico )
        {
            QStringList list;
            dico->GetStringList( list, kind, language );
            combo.insertStringList( list );
        }
    }

    std::string GetDictionaryString( const std::string& key, const kernel::AttributeType& attribute, const tools::StringResolver< DictionaryType >& resolver )
    {
        static const std::string defaultString;
        std::string dictionary;
        std::string kind;
        std::string language;
        attribute.GetDictionaryValues( dictionary, kind, language );
        DictionaryType* dico = resolver.Find( dictionary );
        if( dico )
            return dico->GetLabel( key, kind, language );
        return defaultString;
    }

    std::string GetDictionaryKey( const QString& label, const kernel::AttributeType& attribute, const tools::StringResolver< DictionaryType >& resolver )
    {
        std::string dictionary;
        std::string kind;
        std::string language;
        attribute.GetDictionaryValues( dictionary, kind, language );
        DictionaryType* dico = resolver.Find( dictionary );
        if( dico )
            return dico->GetKey( label.ascii(), kind, language );
        return "";
    }
}

// -----------------------------------------------------------------------------
// Name: OrbatAttributesPanel::AddWidget
// Created: JSR 2010-10-04
// -----------------------------------------------------------------------------
void OrbatAttributesPanel::AddWidget( const kernel::AttributeType& attribute )
{
    static const std::string language = ReadLang();
    const DictionaryExtensions* ext = selected_->Retrieve< DictionaryExtensions >();
    std::string value( ext ? ext->GetValue( attribute.GetName() ) : "" );
    if( attribute.GetType() ==AttributeType::ETypeBoolean )
    {
        QCheckBox* box = new QCheckBox( attribute.GetLabel( language, "" ).c_str(), pGroupBox_, attribute.GetName().c_str() );
        widgets_.push_back( box );
        box->setChecked( value == "true" );
        connect( box, SIGNAL( toggled( bool ) ), this, SLOT( Commit() ) );
        return;
    }
    int min;
    int max;
    attribute.GetMinMaxLength( min, max );
    QHBox* box = new QHBox( pGroupBox_ );
    box->setStretchFactor( new QLabel( attribute.GetLabel( language, "" ).c_str(), box ), 1 );
    switch( attribute.GetType() )
    {
    case AttributeType::ETypeString:
        {
            QLineEdit* edit = new QLineEdit( box, attribute.GetName().c_str() );
            if( min != -1 || max != -1 )
                edit->setValidator( new QMinMaxValidator( edit, min, max ) );
            edit->insert( value.c_str() );
            box->setStretchFactor( edit, 1 );
            widgets_.push_back( edit );
            connect( edit, SIGNAL( textChanged( const QString & ) ), this, SLOT( Commit() ) );
        }
        break;
    case AttributeType::ETypeAlphanumeric:
        {
            QLineEdit* edit = new QLineEdit( box, attribute.GetName().c_str() );
            edit->setValidator( new QMinMaxValidator( edit, min, max, new QRegExpValidator( QRegExp( "[a-zA-Z0-9]*" ), edit ) ) );
            edit->insert( value.c_str() );
            box->setStretchFactor( edit, 1 );
            widgets_.push_back( edit );
            connect( edit, SIGNAL( textChanged( const QString & ) ), this, SLOT( Commit() ) );
        }
        break;
    case AttributeType::ETypeNumeric:
        {
            QLineEdit* edit = new QLineEdit( box, attribute.GetName().c_str() );
            edit->setValidator( new QMinMaxValidator( edit, min, max, new QIntValidator( edit ) ) );
            edit->insert( value.c_str() );
            box->setStretchFactor( edit, 1 );
            widgets_.push_back( edit );
            connect( edit, SIGNAL( textChanged( const QString & ) ), this, SLOT( Commit() ) );
        }
        break;
    case AttributeType::ETypeDictionary:
        {
            QComboBox* combo = new QComboBox( box, attribute.GetName().c_str() );
            FillCombo( *combo, attribute, extensions_ );
            try
            {
                const std::string& selected = GetDictionaryString( value, attribute, extensions_ );
                if( !selected.empty() )
                    combo->setCurrentText( selected.c_str() );
            }
            catch( boost::bad_lexical_cast& )
            {
                // NOTHING
            }
            box->setStretchFactor( combo, 1 );
            widgets_.push_back( combo );
            connect( combo, SIGNAL( activated( int ) ), this, SLOT( Commit() ) );
        }
        break;
    case AttributeType::ETypeLoosyDictionary:
        {
            QComboBox* combo = new QComboBox( true, box, attribute.GetName().c_str() );
            FillCombo( *combo, attribute, extensions_ );
            if( min != -1 || max != 1 )
            {
                QMinMaxValidator* validator = new QMinMaxValidator( combo, min, max );
                QString qvalue( value.c_str() );
                int pos = 0;
                validator->validate( qvalue, pos );
                combo->setValidator( validator );
                combo->setCurrentText( qvalue );
            }
            else
                combo->setCurrentText( value.c_str() );
            box->setStretchFactor( combo, 1 );
            widgets_.push_back( combo );
            connect( combo, SIGNAL( activated( int ) ), this, SLOT( Commit() ) );
        }
        break;
    default:
        break;
    }
}

// -----------------------------------------------------------------------------
// Name: OrbatAttributesPanel::DeleteWidgets
// Created: JSR 2010-10-04
// -----------------------------------------------------------------------------
void OrbatAttributesPanel::DeleteWidgets()
{
    delete pGroupBox_;
    pGroupBox_ = 0;
    widgets_.clear();
}

// -----------------------------------------------------------------------------
// Name: OrbatAttributesPanel::OnActivationChanged
// Created: JSR 2010-10-04
// -----------------------------------------------------------------------------
void OrbatAttributesPanel::OnActivationChanged( bool activate )
{
    DictionaryExtensions* ext = selected_->Retrieve< DictionaryExtensions >();
    if( !ext )
        selected_->Attach( *( ext = new DictionaryExtensions( "orbat-attributes", extensions_ ) ) );
    ext->SetEnabled( activate );
    if( activate )
        Commit();
    else
        controllers_.controller_.Update( *selected_ );
}

// -----------------------------------------------------------------------------
// Name: OrbatAttributesPanel::Commit
// Created: JSR 2010-10-04
// -----------------------------------------------------------------------------
void OrbatAttributesPanel::Commit()
{
    DictionaryExtensions* ext = selected_->Retrieve< DictionaryExtensions >();
    ExtensionType* type = extensions_.tools::StringResolver< ExtensionType >::Find( "orbat-attributes" );
    if( !ext || !type )
        return;
    for( std::vector< QWidget* >::const_iterator it = widgets_.begin(); it != widgets_.end(); ++it )
    {
        AttributeType* attribute = type->tools::StringResolver< AttributeType >::Find( ( *it )->name() );
        if( attribute )
        {
            switch( attribute->GetType() )
            {
            case AttributeType::ETypeBoolean:
                ext->SetValue( ( *it )->name(), static_cast< QCheckBox* >( *it )->isChecked() ? "true" : "false" );
                break;
            case AttributeType::ETypeString:
            case AttributeType::ETypeAlphanumeric:
            case AttributeType::ETypeNumeric:
                {
                    QLineEdit* edit = static_cast< QLineEdit* >( *it );
                    QString text = edit->text();
                    int pos = 0;
                    if( !edit->validator() || edit->validator()->validate( text, pos ) == QValidator::Acceptable )
                    {
                        ext->SetValue( ( *it )->name(), text.ascii() );
                        edit->setPaletteBackgroundColor( Qt::white );  
                    }
                     else
                         edit->setPaletteBackgroundColor( Qt::yellow );  
                }
                break;
            case AttributeType::ETypeDictionary:
                {
                    QComboBox* combo = static_cast< QComboBox* >( *it );
                    const std::string key = GetDictionaryKey( combo->currentText(), *attribute, extensions_ );
                    if( !key.empty() )
                        ext->SetValue( ( *it )->name(), key );
                }
                break;
            case AttributeType::ETypeLoosyDictionary:
                {
                    QComboBox* combo = static_cast< QComboBox* >( *it );
                    QString text = combo->currentText();
                    int pos = 0;
                    if( !combo->validator() || combo->validator()->validate( text, pos ) == QValidator::Acceptable )
                    {
                        ext->SetValue( ( *it )->name(), text.ascii() );
                        combo->setPaletteBackgroundColor( Qt::white );  
                    }
                    else
                        combo->setPaletteBackgroundColor( Qt::yellow );  
                }
                break;
            default:
                break;
            }
        }
    }
    controllers_.controller_.Update( *selected_ );
}
