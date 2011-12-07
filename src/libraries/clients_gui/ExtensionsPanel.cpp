// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

/* TRANSLATOR gui::ExtensionsPanel */

#include "clients_gui_pch.h"
#include "ExtensionsPanel.h"
#include "moc_ExtensionsPanel.cpp"

#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/AttributeType.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/DictionaryExtensions.h"
#include "clients_kernel/DictionaryType.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/ExtensionDependency.h"
#include "clients_kernel/ExtensionType.h"
#include "clients_kernel/ExtensionTypes.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Inhabitant_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/Tools.h"
#include "DiffusionListDialog.h"
#include "DiffusionListLineEdit.h"
#include "DiffusionListHierarchy.h"
#include <boost/lexical_cast.hpp>

using namespace gui;
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
// Name: ExtensionsPanel constructor
// Created: JSR 2010-10-04
// -----------------------------------------------------------------------------
ExtensionsPanel::ExtensionsPanel( QMainWindow* parent, kernel::Controllers& controllers, const kernel::ExtensionTypes& extensions, const tools::Resolver< Agent_ABC >& agents,
                                  ItemFactory_ABC& factory, const EntitySymbols& icons, const Profile_ABC& profile, const char* name /* = 0*/ )
    : QDockWidget( name, parent )
    , controllers_    ( controllers )
    , extensions_     ( extensions )
    , diffusionDialog_( new DiffusionListDialog( parent, controllers, agents, extensions, factory, icons, profile, "ExtensionPanel_DiffusionListDialog" ) )
    , selected_       ( 0 )
    , pGroupBox_      ( 0 )
    , updating_       ( false )
{
    setObjectName( "extensions" );
    pMainLayout_ = new Q3VBox( this );
    pExtensionLayout_ = new Q3VBox( pMainLayout_ );
    setWindowTitle( tr( "Extensions" ) );
    setWidget( pMainLayout_ );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ExtensionsPanel destructor
// Created: JSR 2010-10-04
// -----------------------------------------------------------------------------
ExtensionsPanel::~ExtensionsPanel()
{
    DeleteWidgets();
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: ExtensionsPanel::NotifySelected
// Created: JSR 2010-10-04
// -----------------------------------------------------------------------------
void ExtensionsPanel::NotifySelected( const Entity_ABC* element )
{
    if( selected_ == element )
        return;
    DeleteWidgets();
    selected_ = const_cast< Entity_ABC* >( element );
    ExtensionType* type = extensions_.tools::StringResolver< ExtensionType >::Find( "orbat-attributes" );
    if( type )
    {
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
            else if( typeName == Team_ABC::typeName_ )
                type->GetAttributeTypes( "party", attributes );
            if( attributes.size() )
            {
                pGroupBox_ = new Q3GroupBox( 1, Qt::Horizontal, tr( "Enabled" ), pExtensionLayout_ );
                pGroupBox_->setCheckable( true );
                pGroupBox_->setMargin( 5 );
                for( ExtensionType::RCIT_AttributesTypes it = attributes.rbegin(); it != attributes.rend(); ++it )
                    AddWidget( **it );
                UpdateDependencies();
                const DictionaryExtensions* ext = selected_->Retrieve< DictionaryExtensions >();
                if( !ext )
                    return;
                pGroupBox_->setChecked( ext->IsEnabled() );
                pGroupBox_->show();
                connect( pGroupBox_, SIGNAL( toggled( bool ) ), this, SLOT( OnActivationChanged( bool ) ) );
                return;
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ExtensionsPanel::NotifyDeleted
// Created: JSR 2010-10-04
// -----------------------------------------------------------------------------
void ExtensionsPanel::NotifyDeleted( const Entity_ABC& element )
{
    if( selected_ == &element )
    {
        selected_ = 0;
        DeleteWidgets();
    }
}

// -----------------------------------------------------------------------------
// Name: ExtensionsPanel::NotifyUpdated
// Created: JSR 2011-09-13
// -----------------------------------------------------------------------------
void ExtensionsPanel::NotifyUpdated( const kernel::DictionaryExtensions& extensions )
{
    if( selected_ && selected_->Retrieve< kernel::DictionaryExtensions >() == &extensions )
    {
        updating_ = true;
        if( extensions.IsEnabled() && !pGroupBox_->isChecked() )
            pGroupBox_->setChecked( true );
        else if( !extensions.IsEnabled() && pGroupBox_->isChecked() )
            pGroupBox_->setChecked( false );
        updating_ = false;
        UpdateDependencies();
        UpdateDisplay();
    }
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
            else if( trailing_ && input.length() >= static_cast< int >( trailing_ ) && input.right( trailing_ ) == QString().fill( '*', trailing_ ) )
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
// Name: ExtensionsPanel::AddWidget
// Created: JSR 2010-10-04
// -----------------------------------------------------------------------------
void ExtensionsPanel::AddWidget( const kernel::AttributeType& attribute )
{
    if( !selected_ )
        return;
    static const std::string language = ReadLang();
    const DictionaryExtensions* ext = selected_->Retrieve< DictionaryExtensions >();
    std::string value( ext ? ext->GetValue( attribute.GetName() ) : "" );
    if( attribute.GetType() == AttributeType::ETypeBoolean )
    {
        QCheckBox* box = new QCheckBox( attribute.GetLabel( language, "" ).c_str(), pGroupBox_, attribute.GetName().c_str() );
        widgets_.push_back( box );
        box->setChecked( value == "true" );
        connect( box, SIGNAL( toggled( bool ) ), SLOT( Commit() ) );
        return;
    }
    int min;
    int max;
    attribute.GetMinMaxLength( min, max );
    Q3HBox* box = new Q3HBox( pGroupBox_ );
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
            connect( edit, SIGNAL( textChanged( const QString & ) ), SLOT( Commit() ) );
        }
        break;
    case AttributeType::ETypeAlphanumeric:
        {
            QLineEdit* edit = new QLineEdit( box, attribute.GetName().c_str() );
            edit->setValidator( new QMinMaxValidator( edit, min, max, new QRegExpValidator( QRegExp( "[a-zA-Z0-9.]*" ), edit ) ) );
            edit->insert( value.c_str() );
            box->setStretchFactor( edit, 1 );
            widgets_.push_back( edit );
            connect( edit, SIGNAL( textChanged( const QString & ) ), SLOT( Commit() ) );
        }
        break;
    case AttributeType::ETypeNumeric:
        {
            QLineEdit* edit = new QLineEdit( box, attribute.GetName().c_str() );
            edit->setValidator( new QMinMaxValidator( edit, min, max, new QIntValidator( edit ) ) );
            edit->insert( value.c_str() );
            box->setStretchFactor( edit, 1 );
            widgets_.push_back( edit );
            connect( edit, SIGNAL( textChanged( const QString & ) ), SLOT( Commit() ) );
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
            connect( combo, SIGNAL( activated( int ) ), SLOT( Commit() ) );
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
            connect( combo, SIGNAL( activated( int ) ), SLOT( Commit() ) );
        }
        break;
    case AttributeType::ETypeDiffusionList:
        {
            const std::string extensionName = extensions_.GetNameByType( AttributeType::ETypeDiffusionList );
            assert( !extensionName.empty() );
            DiffusionListLineEdit* edit = new DiffusionListLineEdit( box, controllers_, selected_, *diffusionDialog_, extensionName, attribute.GetName().c_str() );
            edit->setValidator( new QMinMaxValidator( edit, min, max, new QRegExpValidator( DiffusionListHierarchy::diffusionRegexp_, edit ) ) );
            edit->insert( value.c_str() );
            box->setStretchFactor( edit, 1 );
            widgets_.push_back( edit );
            connect( edit, SIGNAL( textChanged( const QString& ) ), SLOT( Commit() ) );
        }
        break;
    default:
        break;
    }
}

// -----------------------------------------------------------------------------
// Name: ExtensionsPanel::DeleteWidgets
// Created: JSR 2010-10-04
// -----------------------------------------------------------------------------
void ExtensionsPanel::DeleteWidgets()
{
    delete pGroupBox_;
    pGroupBox_ = 0;
    widgets_.clear();
}

// -----------------------------------------------------------------------------
// Name: ExtensionsPanel::OnActivationChanged
// Created: JSR 2010-10-04
// -----------------------------------------------------------------------------
void ExtensionsPanel::OnActivationChanged( bool activate )
{
    if( !selected_ )
        return;
    DictionaryExtensions* dico = selected_->Retrieve< DictionaryExtensions >();
    if( !dico )
        return;
    dico->SetEnabled( activate );
    if( activate )
        Commit();
    else
        controllers_.controller_.Update( *selected_ );
}

// -----------------------------------------------------------------------------
// Name: ExtensionsPanel::Commit
// Created: JSR 2010-10-04
// -----------------------------------------------------------------------------
void ExtensionsPanel::Commit()
{
    if( updating_ || !selected_ )
        return;
    DictionaryExtensions* ext = selected_->Retrieve< DictionaryExtensions >();
    ExtensionType* type = extensions_.tools::StringResolver< ExtensionType >::Find( "orbat-attributes" );
    if( !ext || !type )
        return;
    for( CIT_Widgets it = widgets_.begin(); it != widgets_.end(); ++it )
    {
        AttributeType* attribute = type->tools::StringResolver< AttributeType >::Find( ( *it )->name() );
        if( attribute )
        {
            const bool enabled = ( *it )->isEnabled();
            switch( attribute->GetType() )
            {
            case AttributeType::ETypeBoolean:
                ext->SetValue( ( *it )->name(), static_cast< QCheckBox* >( *it )->isChecked() && enabled ? "true" : "false" );
                break;
            case AttributeType::ETypeString:
            case AttributeType::ETypeAlphanumeric:
            case AttributeType::ETypeNumeric:
            case AttributeType::ETypeDiffusionList:
                {
                    QLineEdit* edit = static_cast< QLineEdit* >( *it );
                    QString text = edit->text();
                    int pos = 0;
                    if( !edit->validator() || edit->validator()->validate( text, pos ) == QValidator::Acceptable )
                    {
                        ext->SetValue( edit->name(), enabled ? text.ascii() : "" );
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
                        ext->SetValue( combo->name(), enabled ? key : "" );
                }
                break;
            case AttributeType::ETypeLoosyDictionary:
                {
                    QComboBox* combo = static_cast< QComboBox* >( *it );
                    QString text = combo->currentText();
                    int pos = 0;
                    if( !combo->validator() || combo->validator()->validate( text, pos ) == QValidator::Acceptable )
                    {
                        ext->SetValue( ( *it )->name(), enabled ? text.ascii() : "" );
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
    UpdateDependencies();
}

// -----------------------------------------------------------------------------
// Name: ExtensionsPanel::UpdateDisplay
// Created: ABR 2011-05-16
// -----------------------------------------------------------------------------
void ExtensionsPanel::UpdateDisplay()
{
    if( !selected_ )
        return;
    DictionaryExtensions* ext = selected_->Retrieve< DictionaryExtensions >();
    ExtensionType* type = extensions_.tools::StringResolver< ExtensionType >::Find( "orbat-attributes" );
    if( !ext || !type )
        return;
    updating_ = true;
    for( CIT_Widgets it = widgets_.begin(); it != widgets_.end(); ++it )
    {
        AttributeType* attribute = type->tools::StringResolver< AttributeType >::Find( ( *it )->name() );
        if( attribute )
        {
            const bool enabled = ( *it )->isEnabled();
            QString value = ext->GetValue( ( *it )->name() ).c_str();
            switch( attribute->GetType() )
            {
            case AttributeType::ETypeBoolean:
                static_cast< QCheckBox* >( *it )->setChecked( ( *it )->isEnabled() );
                break;
            case AttributeType::ETypeString:
            case AttributeType::ETypeAlphanumeric:
            case AttributeType::ETypeNumeric:
            case AttributeType::ETypeDiffusionList:
                {
                    QLineEdit* edit = static_cast< QLineEdit* >( *it );
                    edit->setText( enabled ? value : "" );
                    int pos = 0;
                    edit->setPaletteBackgroundColor( ( !edit->validator() || edit->validator()->validate( value, pos ) == QValidator::Acceptable ) ? Qt::white : Qt::yellow );
                }
                break;
            case AttributeType::ETypeDictionary:
                {
                    QComboBox* combo = static_cast< QComboBox* >( *it );
                    const std::string& selected = GetDictionaryString( value.ascii(), *attribute, extensions_ );
                    if( !selected.empty() )
                        combo->setCurrentText( selected.c_str() );
                }
                break;
            case AttributeType::ETypeLoosyDictionary:
                {
                    QComboBox* combo = static_cast< QComboBox* >( *it );
                    combo->setCurrentText( enabled ? value : "" );
                    int pos = 0;
                    combo->setPaletteBackgroundColor( ( !combo->validator() || combo->validator()->validate( value, pos ) == QValidator::Acceptable ) ? Qt::white : Qt::yellow );
                }
                break;
            default:
                break;
            }
        }
    }
    updating_ = false;
}

// -----------------------------------------------------------------------------
// Name: ExtensionsPanel::UpdateDependencies
// Created: ABR 2011-05-03
// -----------------------------------------------------------------------------
void ExtensionsPanel::UpdateDependencies()
{
    if( !selected_ )
        return;
    DictionaryExtensions* dico = selected_->Retrieve< DictionaryExtensions >();
    ExtensionType* type = extensions_.tools::StringResolver< ExtensionType >::Find( "orbat-attributes" );
    if( !dico || !type )
        return;
    const DictionaryExtensions::T_Extensions& extensions = dico->GetExtensions();
    for( CIT_Widgets widget = widgets_.begin(); widget != widgets_.end(); ++widget )
    {
        AttributeType* attribute = type->tools::StringResolver< AttributeType >::Find( ( *widget )->name() );
        if( attribute )
            ( *widget )->setEnabled( attribute->IsActive( extensions ) );
    }
}
