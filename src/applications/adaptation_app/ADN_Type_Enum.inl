// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "ADN_Workspace.h"

template< class T, int nb >
typename ADN_Type_Enum< T, nb >::T_Converter ADN_Type_Enum< T, nb >::converter_ = 0;

// -----------------------------------------------------------------------------
// Name: ADN_Type_Enum::ConvertValue
// Created: AGN 2004-05-13
// -----------------------------------------------------------------------------
template< class T, int nb >
const std::string& ADN_Type_Enum< T, nb >::Convert( ENT_Tr_ABC::E_Conversion nConversion ) const
{
    assert( converter_ );
    return converter_( data_, nConversion );
}

// -----------------------------------------------------------------------------
// Name: ADN_Type_Enum::SetConverter
// Created: AGN 2004-05-13
// -----------------------------------------------------------------------------
template< class T, int nb >
void ADN_Type_Enum< T, nb >::SetConverter( typename ADN_Type_Enum< T, nb >::T_Converter enumConverter )
{
    converter_ = enumConverter;
}

//-----------------------------------------------------------------------------
// Name: ADN_Type_Enum constructor
// Created: JDY 03-07-22
//-----------------------------------------------------------------------------
template< class T, int nb >
ADN_Type_Enum< T, nb >::ADN_Type_Enum()
    : ADN_Connector_ABC()
    , data_((T)0)
    , sortAlphabetically_( true )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: ADN_Type_Enum constructor
// Created: JDY 03-07-22
//-----------------------------------------------------------------------------
template< class T, int nb >
ADN_Type_Enum< T, nb >::ADN_Type_Enum( const T& value )
    : ADN_Connector_ABC()
    , data_( value )
    , sortAlphabetically_( true )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: ADN_Type_Enum destructor
// Created: JDY 03-07-18
//-----------------------------------------------------------------------------
template< class T, int nb >
ADN_Type_Enum< T, nb >::~ADN_Type_Enum()
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: ADN_Type_Enum< T, nb >::T_TypePtr
// Created: JDY 03-07-18
//-----------------------------------------------------------------------------
template< class T, int nb >
const T ADN_Type_Enum< T, nb >::GetData() const
{
    return data_;
}

//-----------------------------------------------------------------------------
// Name: ADN_Type_Enum< T, nb >::operator =
// Created: JDY 03-07-18
//-----------------------------------------------------------------------------
template< class T, int nb >
ADN_Type_Enum< T, nb >& ADN_Type_Enum< T, nb >::operator =(const ADN_Type_Enum& o)
{
    SetData( o.GetData() );
    return *this;
}

//-----------------------------------------------------------------------------
// Name: ADN_Type_Enum< T, nb >::operator =
// Created: JDY 03-07-18
//-----------------------------------------------------------------------------
template< class T, int nb >
ADN_Type_Enum< T, nb >& ADN_Type_Enum< T, nb >::operator =(const T& val)
{
    SetDataPrivate( val );
    return *this;
}

//-----------------------------------------------------------------------------
// Name: ADN_Type_ABC<T>::operator
// Created: JDY 03-07-22
//-----------------------------------------------------------------------------
template< class T, int nb >
bool ADN_Type_Enum< T, nb >::operator ==(const ADN_Type_Enum& o) const
{
    return data_==o.data_;
}

//-----------------------------------------------------------------------------
// Name: ADN_Type_ABC<T>::operator
// Created: JDY 03-07-22
//-----------------------------------------------------------------------------
template< class T, int nb >
bool ADN_Type_Enum< T, nb >::operator ==(const T& val) const
{
    return data_==val;
}

//-----------------------------------------------------------------------------
// Name: ADN_Type_ABC<T>::operator
// Created: JDY 03-07-22
//-----------------------------------------------------------------------------
template< class T, int nb >
bool ADN_Type_Enum< T, nb >::operator !=(const ADN_Type_Enum& o) const
{
    return data_!=o.data_;
}

//-----------------------------------------------------------------------------
// Name: ADN_Type_ABC<T>::operator
// Created: JDY 03-07-22
//-----------------------------------------------------------------------------
template< class T, int nb >
bool ADN_Type_Enum< T, nb >::operator !=(const T& val) const
{
    return data_!=val;
}

//-----------------------------------------------------------------------------
// Name: ADN_Type_Enum< T, nb >::SetDataPrivate
// Created: JDY 03-07-21
//-----------------------------------------------------------------------------
template< class T, int nb >
void ADN_Type_Enum< T, nb >::SetDataPrivate(void *data)
{
    SetDataPrivate( *(T*)data );
    ADN_Workspace::GetWorkspace().SetMainWindowModified( true );
}

//-----------------------------------------------------------------------------
// Name: ADN_Type_Enum< T, nb >::SetDataPrivate
// Created: JDY 03-07-18
//-----------------------------------------------------------------------------
template< class T, int nb >
void ADN_Type_Enum< T, nb >::SetDataPrivate( const T& value )
{
    if( data_!= value )
    {
        data_ = value;
        emit DataChanged( &data_ );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Type_Enum::Initialize
// Created: AGN 2004-03-22
// -----------------------------------------------------------------------------
template< class T, int nb >
void ADN_Type_Enum< T, nb >::Initialize( ADN_Connector_ABC& dest ) const
{
    if( ADN_Connector_EnumComboBox* combo = dynamic_cast< ADN_Connector_EnumComboBox* >( &dest ) )
    {
        assert( converter_ );
        for( int i = 0; i < nb; ++i )
            combo->AddEnumValue( converter_( static_cast< T >( i ), ENT_Tr_ABC::eToTr ), sortAlphabetically_ ? -1 : i );
    }
    dest.SetData( &const_cast< ADN_Type_Enum< T, nb >* >( this )->data_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Type_Enum::SetAlphabeticalSort
// Created: ABR 2012-11-06
// -----------------------------------------------------------------------------
template< class T, int nb >
void ADN_Type_Enum< T, nb >::SetAlphabeticalSort( bool sort )
{
    sortAlphabetically_ = sort;
}
