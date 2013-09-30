// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Type_LocalizedString.h"
#include "ADN_Languages_Data.h"
#include "clients_kernel/Context.h"
#include "clients_kernel/Language.h"

// -----------------------------------------------------------------------------
// Name: ADN_Type_LocalizedString constructor
// Created: ABR 2013-07-12
// -----------------------------------------------------------------------------
ADN_Type_LocalizedString::ADN_Type_LocalizedString()
    : swappingLanguage_( false )
{
    Initialize();
}

// -----------------------------------------------------------------------------
// Name: ADN_Type_LocalizedString constructor
// Created: ABR 2013-07-12
// -----------------------------------------------------------------------------
ADN_Type_LocalizedString::ADN_Type_LocalizedString( const std::string& val )
    : ADN_Type_ABC< std::string >( val )
    , swappingLanguage_( false )
{
    Initialize();
}

// -----------------------------------------------------------------------------
// Name: ADN_Type_LocalizedString destructor
// Created: ABR 2013-07-12
// -----------------------------------------------------------------------------
ADN_Type_LocalizedString::~ADN_Type_LocalizedString()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Type_LocalizedString::Initialize
// Created: ABR 2013-08-23
// -----------------------------------------------------------------------------
void ADN_Type_LocalizedString::Initialize()
{
    AddTranslationCheckers();
    connect( &ADN_Workspace::GetWorkspace().GetLanguages().GetGuiABC(), SIGNAL( LanguageChanged() ), this, SLOT( OnLanguageChanged() ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_Type_LocalizedString::InitTranslation
// Created: ABR 2013-08-26
// -----------------------------------------------------------------------------
void ADN_Type_LocalizedString::InitTranslation( const std::string& data )
{
    if( !context_ )
        throw MASA_EXCEPTION( "Translation context not set for localized string: " + data );
    if( translation_ && !( kernel::Language::IsCurrentDefault() && !data.empty() && translation_->Key().empty() || translation_.unique() ) )
        return;
    translation_ = ( *context_ )[ data ];
    translation_->Initialize( ADN_Workspace::GetWorkspace().GetLanguages().GetData().languages_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Type_LocalizedString::GetData
// Created: ABR 2013-07-12
// -----------------------------------------------------------------------------
const std::string& ADN_Type_LocalizedString::GetData() const
{
    return GetValue( kernel::Language::Current() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Type_LocalizedString::SetData
// Created: ABR 2013-07-12
// -----------------------------------------------------------------------------
void ADN_Type_LocalizedString::SetData( const std::string& data )
{
    InitTranslation( data );
    if( swappingLanguage_ )
        return;
    if( kernel::Language::IsCurrentDefault() && translation_.use_count() > 2 && data != translation_->Key() )
    {
        boost::shared_ptr< kernel::LocalizedString > newTranslation = context_->CreateNew( data );
        newTranslation->CopyValues( *translation_ );
        translation_ = newTranslation;
    }
    else
        translation_->SetValue( data );
    emit DataChanged( ( void* ) &data );
}

// -----------------------------------------------------------------------------
// Name: ADN_Type_LocalizedString::GetValue
// Created: ABR 2013-08-27
// -----------------------------------------------------------------------------
const std::string& ADN_Type_LocalizedString::GetValue( const std::string& language ) const
{
    if( !kernel::Language::IsDefault( language ) && translation_->Value( language ).empty() )
        return translation_->Key();
    return translation_->Value( language );
}

// -----------------------------------------------------------------------------
// Name: ADN_Type_LocalizedString::SetValue
// Created: ABR 2013-08-27
// -----------------------------------------------------------------------------
void ADN_Type_LocalizedString::SetValue( const std::string& language, const std::string& data )
{
    InitTranslation( data );
    translation_->SetValue( language, data );
}

// -----------------------------------------------------------------------------
// Name: ADN_Type_LocalizedString::GetKey
// Created: ABR 2013-07-15
// -----------------------------------------------------------------------------
const std::string& ADN_Type_LocalizedString::GetKey() const
{
    return translation_->Key();
}

// -----------------------------------------------------------------------------
// Name: ADN_Type_LocalizedString::SetKey
// Created: ABR 2013-08-27
// -----------------------------------------------------------------------------
void ADN_Type_LocalizedString::SetKey( const std::string& key )
{
    InitTranslation( key );
    translation_->SetKey( key );
}

// -----------------------------------------------------------------------------
// Name: ADN_Type_LocalizedString::GetType
// Created: ABR 2013-07-15
// -----------------------------------------------------------------------------
kernel::E_TranslationType ADN_Type_LocalizedString::GetType() const
{
    kernel::E_TranslationType type = translation_->Type();
    return type;
}

// -----------------------------------------------------------------------------
// Name: ADN_Type_LocalizedString::SetType
// Created: ABR 2013-07-15
// -----------------------------------------------------------------------------
void ADN_Type_LocalizedString::SetType( kernel::E_TranslationType type )
{
    if( translation_->Key().empty() )
        return;
    InitTranslation( translation_->Key() );
    translation_->SetType( type );
    emit TypeChanged( type );
    CheckValidity();
}

// -----------------------------------------------------------------------------
// Name: ADN_Type_LocalizedString::operator =
// Created: ABR 2013-07-12
// -----------------------------------------------------------------------------
ADN_Type_LocalizedString& ADN_Type_LocalizedString::operator=( const std::string& val )
{
    SetData( val );
    return *this;
}

// -----------------------------------------------------------------------------
// Name: ADN_Type_LocalizedString::operator ==
// Created: ABR 2013-07-16
// -----------------------------------------------------------------------------
bool ADN_Type_LocalizedString::operator==( const std::string& val ) const
{
    return GetKey() == val;
}

// -----------------------------------------------------------------------------
// Name: ADN_Type_LocalizedString::operator !=
// Created: ABR 2013-07-16
// -----------------------------------------------------------------------------
bool ADN_Type_LocalizedString::operator!=( const std::string& val ) const
{
    return GetKey() != val;
}

// -----------------------------------------------------------------------------
// Name: ADN_Type_ABC::Initialize
// Created: APE 2005-01-12
// -----------------------------------------------------------------------------
void ADN_Type_LocalizedString::Initialize( ADN_Connector_ABC& dest ) const
{
    dest.SetData( ( void* ) &GetData() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Type_LocalizedString::OnLanguageChanged
// Created: ABR 2013-07-12
// -----------------------------------------------------------------------------
void ADN_Type_LocalizedString::OnLanguageChanged()
{
    if( !translation_ )
        return;
    if( !kernel::Language::IsCurrentDefault() && translation_->Value().empty() )
    {
        swappingLanguage_ = true;
        emit DataChanged( ( void* ) &translation_->Key() );
        swappingLanguage_ = false;
    }
    else
    {
        SetData( GetData() );
    }
    SetType( translation_->Type() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Type_LocalizedString::OnApplyClicked
// Created: ABR 2013-07-15
// -----------------------------------------------------------------------------
void ADN_Type_LocalizedString::OnTypeChanged( int type )
{
    assert( type >= 0 && type < kernel::eNbrTranslationType );
    SetType( static_cast< kernel::E_TranslationType >( type ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_Type_LocalizedString::SetContext
// Created: ABR 2013-08-23
// -----------------------------------------------------------------------------
void ADN_Type_LocalizedString::SetContext( boost::shared_ptr< kernel::Context > context )
{
    context_ = context;
    InitTranslation( "" );
}

// -----------------------------------------------------------------------------
// Name: ADN_Type_LocalizedString::AddTranslationCheckers
// Created: ABR 2013-08-23
// -----------------------------------------------------------------------------
void ADN_Type_LocalizedString::AddTranslationCheckers()
{
    checkers_.push_back( new TranslationChecker( eWarning, tr( "Unfinished translation") ) );
    checkers_.push_back( new TranslationUniquenessChecker( eError, tr( "Duplicate translation key with different translation values" ) ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_Type_LocalizedString::CheckUniqueTranslation
// Created: ABR 2013-08-26
// -----------------------------------------------------------------------------
bool ADN_Type_LocalizedString::CheckUniqueTranslation() const
{
    if( !context_ )
        throw MASA_EXCEPTION( "Translation context not set for localized string." );
    for( auto it = context_->begin(); it != context_->end(); ++it )
        if( *it != translation_ && translation_->Key() == ( *it )->Key() && *translation_ != **it )
            return false;
    return true;
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr< kernel::LocalizedString > ADN_Type_LocalizedString::GetTranslation
// Created: ABR 2013-08-28
// -----------------------------------------------------------------------------
boost::shared_ptr< kernel::LocalizedString > ADN_Type_LocalizedString::GetTranslation() const
{
    return translation_;
}
