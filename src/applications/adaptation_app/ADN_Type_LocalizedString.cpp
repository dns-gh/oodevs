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

// -----------------------------------------------------------------------------
// Name: ADN_Type_LocalizedString constructor
// Created: ABR 2013-07-12
// -----------------------------------------------------------------------------
ADN_Type_LocalizedString::ADN_Type_LocalizedString()
    : ADN_Type_ABC< std::string >()
{
    Initialize();
}

// -----------------------------------------------------------------------------
// Name: ADN_Type_LocalizedString constructor
// Created: ABR 2013-07-12
// -----------------------------------------------------------------------------
ADN_Type_LocalizedString::ADN_Type_LocalizedString( const std::string& val )
    : ADN_Type_ABC< std::string >( val )
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
    AddTranslationChecker();
    connect( &ADN_Workspace::GetWorkspace().GetLanguages().GetGuiABC(), SIGNAL( LanguageChanged() ), this, SLOT( OnLanguageChanged() ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_Type_LocalizedString::CheckTranslation
// Created: ABR 2013-07-18
// -----------------------------------------------------------------------------
void ADN_Type_LocalizedString::CheckTranslation() const
{
    if( !translation_ )
        throw MASA_EXCEPTION( "Translation not set yet for localized type" );
}

// -----------------------------------------------------------------------------
// Name: ADN_Type_LocalizedString::GetData
// Created: ABR 2013-07-12
// -----------------------------------------------------------------------------
const std::string& ADN_Type_LocalizedString::GetData() const
{
    CheckTranslation();
    return translation_->Value();
}

// -----------------------------------------------------------------------------
// Name: ADN_Type_LocalizedString::SetData
// Created: ABR 2013-07-12
// -----------------------------------------------------------------------------
void ADN_Type_LocalizedString::SetData( const std::string& data )
{
    if( !translation_ )
    {
        if( !context_ )
            throw MASA_EXCEPTION( "Translation context not set for localized type: " + data );
        translation_ = ( *context_ )[ data ];
        translation_->InitEmptyValues( ADN_Workspace::GetWorkspace().GetLanguages().GetData().languages_ );
    }
    translation_->SetValue( data );
    emit DataChanged( ( void* ) &data );
}

// -----------------------------------------------------------------------------
// Name: ADN_Type_LocalizedString::operator =
// Created: ABR 2013-07-12
// -----------------------------------------------------------------------------
ADN_Type_LocalizedString& ADN_Type_LocalizedString::operator =( const std::string& val )
{
    SetData( val );
    return *this;
}

// -----------------------------------------------------------------------------
// Name: ADN_Type_LocalizedString::GetKey
// Created: ABR 2013-07-15
// -----------------------------------------------------------------------------
const std::string& ADN_Type_LocalizedString::GetKey() const
{
    CheckTranslation();
    return translation_->Key();
}

// -----------------------------------------------------------------------------
// Name: ADN_Type_LocalizedString::GetType
// Created: ABR 2013-07-15
// -----------------------------------------------------------------------------
kernel::E_TranslationType ADN_Type_LocalizedString::GetType() const
{
    CheckTranslation();
    kernel::E_TranslationType type = translation_->Type();
    emit TypeChanged( type );
    return type;
}

// -----------------------------------------------------------------------------
// Name: ADN_Type_LocalizedString::SetType
// Created: ABR 2013-07-15
// -----------------------------------------------------------------------------
void ADN_Type_LocalizedString::SetType( kernel::E_TranslationType type )
{
    CheckTranslation();
    translation_->SetType( type );
    emit TypeChanged( type );
    CheckValidity();
}

// -----------------------------------------------------------------------------
// Name: ADN_Type_LocalizedString::operator ==
// Created: ABR 2013-07-16
// -----------------------------------------------------------------------------
bool ADN_Type_LocalizedString::operator ==(const std::string& val) const
{
    return GetKey() == val;
}

// -----------------------------------------------------------------------------
// Name: ADN_Type_LocalizedString::operator !=
// Created: ABR 2013-07-16
// -----------------------------------------------------------------------------
bool ADN_Type_LocalizedString::operator !=(const std::string& val) const
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
    SetData( GetData() );
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
}

// -----------------------------------------------------------------------------
// Name: ADN_Type_LocalizedString::AddTranslationChecker
// Created: ABR 2013-08-23
// -----------------------------------------------------------------------------
void ADN_Type_LocalizedString::AddTranslationChecker()
{
    checkers_.push_back( new TranslationChecker( eWarning, tr( "Unfinished translation") ) );
}
