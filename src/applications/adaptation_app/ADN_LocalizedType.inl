// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: ADN_LocalizedType constructor
// Created: ABR 2013-07-12
// -----------------------------------------------------------------------------
template< typename T >
ADN_LocalizedType< T >::ADN_LocalizedType()
    : translation_( nullptr )
{
    AddTranslationChecker();
    connect( &ADN_Workspace::GetWorkspace().GetLanguages().GetGuiABC(), SIGNAL( LanguageChanged() ), this, SLOT( OnLanguageChanged() ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_LocalizedType constructor
// Created: ABR 2013-07-12
// -----------------------------------------------------------------------------
template< typename T >
ADN_LocalizedType< T >::ADN_LocalizedType( const T& val )
    : ADN_Type_ABC< T >( val )
    , translation_( nullptr )
{
    AddTranslationChecker();
    connect( &ADN_Workspace::GetWorkspace().GetLanguages().GetGuiABC(), SIGNAL( LanguageChanged() ), this, SLOT( OnLanguageChanged() ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_LocalizedType destructor
// Created: ABR 2013-07-12
// -----------------------------------------------------------------------------
template< typename T >
ADN_LocalizedType< T >::~ADN_LocalizedType()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_LocalizedType::HasTranslation
// Created: ABR 2013-07-18
// -----------------------------------------------------------------------------
template< typename T >
bool ADN_LocalizedType< T >::HasTranslation() const
{
    return translation_ && kernel::Language::CurrentIsValid();
}

// -----------------------------------------------------------------------------
// Name: ADN_LocalizedType::OnLanguageChanged
// Created: ABR 2013-07-12
// -----------------------------------------------------------------------------
template< typename T >
void ADN_LocalizedType< T >::OnLanguageChanged()
{
    SetData( GetData() );
    SetType( HasTranslation() ? translation_->values_[ kernel::Language::GetCurrent() ].type_ : kernel::eTranslationType_None );
}

// -----------------------------------------------------------------------------
// Name: ADN_LocalizedType::GetData
// Created: ABR 2013-07-12
// -----------------------------------------------------------------------------
template< typename T >
const T& ADN_LocalizedType< T >::GetData() const
{
    return HasTranslation() ? translation_->values_.at( kernel::Language::GetCurrent() ).value_ : GetKey() ;
}

// -----------------------------------------------------------------------------
// Name: ADN_LocalizedType::SetData
// Created: ABR 2013-07-12
// -----------------------------------------------------------------------------
template< typename T >
void ADN_LocalizedType< T >::SetData( const T& data )
{
    if( HasTranslation() )
    {
        std::string& value = translation_->values_[ kernel::Language::GetCurrent() ].value_;
        value = data;
        emit DataChanged( ( void* ) &value );
    }
    else
    {
        if( data == val_ )
            emit DataChanged( ( void* ) &data );
        else
            SetKey( data );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_LocalizedType::GetKey
// Created: ABR 2013-07-15
// -----------------------------------------------------------------------------
template< typename T >
const T& ADN_LocalizedType< T >::GetKey() const
{
    return ADN_Type_ABC< T >::GetData();
}

// -----------------------------------------------------------------------------
// Name: ADN_LocalizedType::SetKey
// Created: ABR 2013-07-16
// -----------------------------------------------------------------------------
template< typename T >
void ADN_LocalizedType< T >::SetKey( const T& key )
{
    ADN_Type_ABC< T >::SetData( key );
    if( translation_ )
        translation_->key_ = key;
}

// -----------------------------------------------------------------------------
// Name: ADN_LocalizedType::GetTranslation
// Created: ABR 2013-07-15
// -----------------------------------------------------------------------------
template< typename T >
const kernel::Translation* ADN_LocalizedType< T >::GetTranslation() const
{
    return translation_;
}

// -----------------------------------------------------------------------------
// Name: ADN_LocalizedType::SetLocalizedTexts
// Created: ABR 2013-07-12
// -----------------------------------------------------------------------------
template< typename T >
void ADN_LocalizedType< T >::SetTranslation( const std::string& key, kernel::Translation* translation )
{
    SetKey( key );
    if( !translation )
        return;
    translation_ = translation;
    for( auto it = translation_->values_.begin(); it != translation_->values_.end(); ++it )
        if( it->second.value_.empty() )
            it->second.value_ = key;
}

// -----------------------------------------------------------------------------
// Name: ADN_LocalizedType::GetType
// Created: ABR 2013-07-15
// -----------------------------------------------------------------------------
template< class T >
kernel::E_TranslationType ADN_LocalizedType< T >::GetType() const
{
    kernel::E_TranslationType type = HasTranslation() ? translation_->values_[ kernel::Language::GetCurrent() ].type_ : kernel::eTranslationType_None;
    emit TypeChanged( type );
    return type;
}

// -----------------------------------------------------------------------------
// Name: ADN_LocalizedType::SetType
// Created: ABR 2013-07-15
// -----------------------------------------------------------------------------
template< class T >
void ADN_LocalizedType< T >::SetType( kernel::E_TranslationType type )
{
    kernel::E_TranslationType newType = kernel::eTranslationType_None;
    if( HasTranslation() )
    {
        translation_->values_[ kernel::Language::GetCurrent() ].type_ = type;
        newType = type;
    }
    emit TypeChanged( newType );
    CheckValidity();
}

// -----------------------------------------------------------------------------
// Name: ADN_LocalizedType::operator =
// Created: ABR 2013-07-12
// -----------------------------------------------------------------------------
template< typename T >
ADN_LocalizedType< T >& ADN_LocalizedType< T >::operator =( const T& val )
{
    SetData( val );
    return *this;
}

// -----------------------------------------------------------------------------
// Name: ADN_LocalizedType::operator ==
// Created: ABR 2013-07-16
// -----------------------------------------------------------------------------
template< class T >
bool ADN_LocalizedType< T >::operator ==(const T& val) const
{
    return GetKey() == val;
}

// -----------------------------------------------------------------------------
// Name: ADN_LocalizedType::operator !=
// Created: ABR 2013-07-16
// -----------------------------------------------------------------------------
template< class T >
bool ADN_LocalizedType< T >::operator !=(const T& val) const
{
    return GetKey() != val;
}

// -----------------------------------------------------------------------------
// Name: ADN_Type_ABC::Initialize
// Created: APE 2005-01-12
// -----------------------------------------------------------------------------
template< class T >
void ADN_LocalizedType< T >::Initialize( ADN_Connector_ABC& dest ) const
{
    dest.SetData( ( void* ) &GetData() );
}

// -----------------------------------------------------------------------------
// Name: ADN_LocalizedType::OnApplyClicked
// Created: ABR 2013-07-15
// -----------------------------------------------------------------------------
template< class T >
void ADN_LocalizedType< T >::OnTypeChanged( int type )
{
    assert( type >= 0 && type < kernel::eNbrTranslationType );
    SetType( static_cast< kernel::E_TranslationType >( type ) );
}
