// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

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
#include "DiffusionListFunctors.h"
#include "DiffusionListLineEdit.h"
#include "RichCheckBox.h"
#include "RichWidget.h"
#include "RichGroupBox.h"
#include "SubObjectName.h"
#include <tools/Language.h>
#include <boost/lexical_cast.hpp>

using namespace gui;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ExtensionsPanel constructor
// Created: JSR 2010-10-04
// -----------------------------------------------------------------------------
ExtensionsPanel::ExtensionsPanel( QMainWindow* parent, kernel::Controllers& controllers, const kernel::ExtensionTypes& extensions,
                                  const tools::Resolver< Agent_ABC >& agents, const tools::Resolver< kernel::Formation_ABC >& formations,
                                  const kernel::Profile_ABC& profile )
    : RichDockWidget( controllers, parent, "extensions", tr( "Extensions" ) )
    , controllers_    ( controllers )
    , extensions_     ( extensions )
    , profile_        ( profile )
    , diffusionDialog_( new DiffusionListDialog( parent, controllers, agents, formations, extensions, "DiffusionListDialog" ) )
    , selected_       ( controllers )
    , pGroupBox_      ( 0 )
    , updating_       ( false )
{
    SubObjectName subObject( this->objectName() );
    QScrollArea* area = new QScrollArea;
    QWidget* mainWidget = new QWidget;
    area->setWidget( mainWidget );
    pMainLayout_ = new QVBoxLayout;
    mainWidget->setLayout( pMainLayout_ );
    pMainLayout_->setMargin( 5 );
    pMainLayout_->setSpacing( 5 );
    pExtensionLayout_ = new QWidget;
    pMainLayout_->addWidget( pExtensionLayout_ );
    pMainLayout_->addStretch( 1 );
    area->setWidgetResizable( true );
    area->setAlignment( Qt::AlignTop );
    setWidget( area );
    controllers_.Update( *this );
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
            const std::string& typeName = selected_->GetTypeName();
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
                pGroupBox_ = new RichGroupBox( "enabledGroupBox", tr( "Enabled" ) );
                pGroupBoxLayout_ = new QGridLayout( pGroupBox_ );
                int currentRow = 0;
                for( ExtensionType::RCIT_AttributesTypes it = attributes.rbegin(); it != attributes.rend(); ++it, ++currentRow )
                    AddWidget( **it, currentRow );
                pGroupBoxLayout_->setColStretch( 1, 4 );
                delete pExtensionLayout_->layout();
                QVBoxLayout* pExtensionLayoutLayout = new QVBoxLayout( pExtensionLayout_ );
                pExtensionLayoutLayout->addWidget( pGroupBox_ );
                pExtensionLayoutLayout->setAlignment( Qt::AlignTop );
                UpdateDependencies();
                const DictionaryExtensions* ext = selected_->Retrieve< DictionaryExtensions >();
                if( !ext )
                    return;
                pGroupBox_->show();
                pGroupBox_->setEnabled( !IsReadOnly() );
                pGroupBox_->setCheckable( !IsReadOnly() );
                pGroupBox_->setChecked( ext->IsEnabled() );
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
            if( 0 == len )
                return Acceptable; // All extensions are optional.
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

    void FillCombo( RichWidget< QComboBox >& combo, const kernel::AttributeType& attribute, const tools::StringResolver< DictionaryType >& resolver )
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
            return dico->GetKey( label.toStdString(), kind, language );
        return "";
    }
}

// -----------------------------------------------------------------------------
// Name: ExtensionsPanel::AddWidget
// Created: JSR 2010-10-04
// -----------------------------------------------------------------------------
void ExtensionsPanel::AddWidget( const kernel::AttributeType& attribute, int currentRow )
{
    if( !selected_ )
        return;
    static const std::string& language = tools::Language::Current();
    const DictionaryExtensions* ext = selected_->Retrieve< DictionaryExtensions >();
    std::string value( ext ? ext->GetValue( attribute.GetName() ) : "" );
    if( attribute.GetType() == AttributeType::ETypeBoolean )
    {
        RichCheckBox* box = new RichCheckBox( attribute.GetName().c_str(), attribute.GetLabel( language, "" ).c_str() );
        pGroupBoxLayout_->addWidget( box, currentRow, 0 );
        widgets_[ attribute.GetName() ] = box;
        box->setChecked( value == "true" );
        connect( box, SIGNAL( toggled( bool ) ), SLOT( Commit() ) );
        return;
    }
    int min;
    int max;
    attribute.GetMinMaxLength( min, max );
    QLabel* attributeNameLabel = new QLabel( attribute.GetLabel( language, "" ).c_str() );
    pGroupBoxLayout_->addWidget( attributeNameLabel );
    switch( attribute.GetType() )
    {
    case AttributeType::ETypeString:
        {
            RichLineEdit* edit = new RichLineEdit( attribute.GetName().c_str() );
            if( min != -1 || max != -1 )
                edit->setValidator( new QMinMaxValidator( edit, min, max ) );
            edit->insert( value.c_str() );
            pGroupBoxLayout_->addWidget( edit, currentRow, 1 );
            widgets_[ attribute.GetName() ] = edit;
            connect( edit, SIGNAL( textChanged( const QString & ) ), SLOT( Commit() ) );
        }
        break;
    case AttributeType::ETypeAlphanumeric:
        {
            RichLineEdit* edit = new RichLineEdit( attribute.GetName().c_str() );
            edit->setValidator( new QMinMaxValidator( edit, min, max, new QRegExpValidator( QRegExp( "[a-zA-Z0-9.]*" ), edit ) ) );
            edit->insert( value.c_str() );
            pGroupBoxLayout_->addWidget( edit, currentRow, 1 );
            widgets_[ attribute.GetName() ] = edit;
            connect( edit, SIGNAL( textChanged( const QString & ) ), SLOT( Commit() ) );
        }
        break;
    case AttributeType::ETypeNumeric:
        {
            RichLineEdit* edit = new RichLineEdit( attribute.GetName().c_str() );
            edit->setValidator( new QMinMaxValidator( edit, min, max, new QIntValidator( edit ) ) );
            edit->insert( value.c_str() );
            pGroupBoxLayout_->addWidget( edit, currentRow, 1 );
            widgets_[ attribute.GetName() ] = edit;
            connect( edit, SIGNAL( textChanged( const QString & ) ), SLOT( Commit() ) );
        }
        break;
    case AttributeType::ETypeDictionary:
        {
            RichWidget< QComboBox >* combo = new RichWidget< QComboBox >( attribute.GetName().c_str() );
            FillCombo( *combo, attribute, extensions_ );
            try
            {
                const std::string& selected = GetDictionaryString( value, attribute, extensions_ );
                if( !selected.empty() )
                    combo->setCurrentText( selected.c_str() );
            }
            catch( const boost::bad_lexical_cast& )
            {
                // NOTHING
            }
            pGroupBoxLayout_->addWidget( combo, currentRow, 1 );
            widgets_[ attribute.GetName() ] = combo;
            connect( combo, SIGNAL( activated( int ) ), SLOT( Commit() ) );
        }
        break;
    case AttributeType::ETypeLoosyDictionary:
        {
            RichWidget< QComboBox >* combo = new RichWidget< QComboBox >( attribute.GetName().c_str() );
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
            pGroupBoxLayout_->addWidget( combo, currentRow, 1 );
            widgets_[ attribute.GetName() ] = combo;
            connect( combo, SIGNAL( activated( int ) ), SLOT( Commit() ) );
        }
        break;
    case AttributeType::ETypeDiffusionList:
        {
            if( !diffusionDialog_.get() )
                return;
            const std::string extensionName = extensions_.GetNameByType( AttributeType::ETypeDiffusionList );
            assert( !extensionName.empty() );
            DiffusionListLineEdit* edit = new DiffusionListLineEdit( attribute.GetName().c_str(), 0, controllers_, selected_, *diffusionDialog_, extensionName );
            edit->insert( value.c_str() );
            edit->setValidator( new QMinMaxValidator( edit, min, max, new QRegExpValidator( DiffusionListData::regexp_, edit ) ) );
            pGroupBoxLayout_->addWidget( edit, currentRow, 1 );
            widgets_[ attribute.GetName() ] = edit;
            connect( edit, SIGNAL( textChanged( const QString & ) ), SLOT( Commit() ) );
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
    DictionaryExtensions* dico = selected_.ConstCast()->Retrieve< DictionaryExtensions >();
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
    DictionaryExtensions* ext = selected_.ConstCast()->Retrieve< DictionaryExtensions >();
    ExtensionType* type = extensions_.tools::StringResolver< ExtensionType >::Find( "orbat-attributes" );
    if( !ext || !type )
        return;
    for( auto it = widgets_.begin(); it != widgets_.end(); ++it )
    {
        AttributeType* attribute = type->tools::StringResolver< AttributeType >::Find( it->first );
        if( attribute )
        {
            const bool enabled = it->second->isEnabled();
            switch( attribute->GetType() )
            {
            case AttributeType::ETypeBoolean:
                ext->SetValue( it->first, static_cast< RichCheckBox* >( it->second )->isChecked() && enabled ? "true" : "false" );
                break;
            case AttributeType::ETypeString:
            case AttributeType::ETypeAlphanumeric:
            case AttributeType::ETypeNumeric:
            case AttributeType::ETypeDiffusionList:
                {
                    RichLineEdit* edit = static_cast< RichLineEdit* >( it->second );
                    QString text = edit->text();
                    int pos = 0;
                    if( !edit->validator() || edit->validator()->validate( text, pos ) == QValidator::Acceptable )
                    {
                        ext->SetValue( it->first, enabled ? text.toStdString() : "" );
                        edit->setPaletteBackgroundColor( Qt::white );
                    }
                    else
                        edit->setPaletteBackgroundColor( Qt::yellow );
                }
                break;
            case AttributeType::ETypeDictionary:
                {
                    RichWidget< QComboBox >* combo = static_cast< RichWidget< QComboBox >* >( "combo", it->second );
                    const std::string key = GetDictionaryKey( combo->currentText(), *attribute, extensions_ );
                    if( !key.empty() )
                        ext->SetValue( it->first, enabled ? key : "" );
                    else
                        ext->Reset( it->first );
                }
                break;
            case AttributeType::ETypeLoosyDictionary:
                {
                    RichWidget< QComboBox >* combo = static_cast< RichWidget< QComboBox >* >( "combo", it->second );
                    QString text = combo->currentText();
                    int pos = 0;
                    if( !combo->validator() || combo->validator()->validate( text, pos ) == QValidator::Acceptable )
                    {
                        ext->SetValue( it->first, enabled ? text.toStdString() : "" );
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
    const DictionaryExtensions* ext = selected_->Retrieve< DictionaryExtensions >();
    ExtensionType* type = extensions_.tools::StringResolver< ExtensionType >::Find( "orbat-attributes" );
    if( !ext || !type )
        return;
    updating_ = true;
    for( auto it = widgets_.begin(); it != widgets_.end(); ++it )
    {
        AttributeType* attribute = type->tools::StringResolver< AttributeType >::Find( it->first );
        if( attribute )
        {
            const bool enabled = it->second->isEnabled();
            QString value = ext->GetValue( it->first ).c_str();
            switch( attribute->GetType() )
            {
            case AttributeType::ETypeBoolean:
                static_cast< RichCheckBox* >( it->second )->setChecked( it->second->isEnabled() );
                break;
            case AttributeType::ETypeString:
            case AttributeType::ETypeAlphanumeric:
            case AttributeType::ETypeNumeric:
            case AttributeType::ETypeDiffusionList:
                {
                    RichLineEdit* edit = static_cast< RichLineEdit* >( it->second );
                    edit->setText( enabled ? value : "" );
                    int pos = 0;
                    edit->setPaletteBackgroundColor( ( !edit->validator() || edit->validator()->validate( value, pos ) == QValidator::Acceptable ) ? Qt::white : Qt::yellow );
                }
                break;
            case AttributeType::ETypeDictionary:
                {
                    RichWidget< QComboBox >* combo = static_cast< RichWidget< QComboBox >* >( "combo", it->second );
                    const std::string& selected = GetDictionaryString( value.toStdString(), *attribute, extensions_ );
                    if( !selected.empty() )
                        combo->setCurrentText( selected.c_str() );
                }
                break;
            case AttributeType::ETypeLoosyDictionary:
                {
                    RichWidget< QComboBox >* combo = static_cast< RichWidget< QComboBox >* >( "combo", it->second );
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
    const DictionaryExtensions* dico = selected_->Retrieve< DictionaryExtensions >();
    ExtensionType* type = extensions_.tools::StringResolver< ExtensionType >::Find( "orbat-attributes" );
    if( !dico || !type )
        return;
    const DictionaryExtensions::T_Extensions& extensions = dico->GetExtensions();
    for( auto it = widgets_.begin(); it != widgets_.end(); ++it )
    {
        AttributeType* attribute = type->tools::StringResolver< AttributeType >::Find( it->first );
        if( attribute )
            it->second->setEnabled( attribute->IsActive( extensions ) && attribute->IsEditable( profile_ ) );
    }
}
