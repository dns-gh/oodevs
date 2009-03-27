// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: ADN_Type_Choice constructor
// Created: JCR 2008-08-20
// -----------------------------------------------------------------------------
template < typename T >
ADN_Type_Choice< T >::ADN_Type_Choice( const T_ChoicesVector& choices )
    : ADN_Connector_Vector_ABC()    
{
    choices_ = choices;
}
    
// -----------------------------------------------------------------------------
// Name: ADN_Type_Choice destructor
// Created: JCR 2008-08-20
// -----------------------------------------------------------------------------
template < typename T >
ADN_Type_Choice< T >::~ADN_Type_Choice()
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: ADN_Type_Enum< T >::T_TypePtr 
// Created: JDY 03-07-18
//-----------------------------------------------------------------------------
template < typename T >
T& ADN_Type_Choice< T >::GetData()
{
    return data_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Type_Choice::operator =
// Created: JCR 2008-08-20
// -----------------------------------------------------------------------------
template < typename T >
ADN_Type_Choice< T >& ADN_Type_Choice< T >::operator =( const ADN_Type_Choice& o )
{
    if ( CheckData( o.GetData() ) )
        SetData( &o.GetData(), false );
    return *this;
}

// -----------------------------------------------------------------------------
// Name: ADN_Type_Choice::operator =
// Created: JCR 2008-08-20
// -----------------------------------------------------------------------------
template < typename T >
ADN_Type_Choice< T >& ADN_Type_Choice< T >::operator =( const T& data )
{    
    SetDataPrivate( data, false );
    return *this;
}
 
// -----------------------------------------------------------------------------
// Name: ADN_Type_Choice::operator ==
// Created: JCR 2008-08-20
// -----------------------------------------------------------------------------
template < typename T >
bool ADN_Type_Choice< T >::operator ==(const ADN_Type_Choice& o) const
{
    return data_ == o.data_;
}
    
// -----------------------------------------------------------------------------
// Name: ADN_Type_Choice::operator ==
// Created: JCR 2008-08-20
// -----------------------------------------------------------------------------
template < typename T >
bool ADN_Type_Choice< T >::operator ==(const T& val) const
{
    return data_ == val;
}
    
// -----------------------------------------------------------------------------
// Name: ADN_Type_Choice::operator !=
// Created: JCR 2008-08-20
// -----------------------------------------------------------------------------
template < typename T >
bool ADN_Type_Choice< T >::operator !=(const ADN_Type_Choice& o) const
{
    return data_ != o.data_;
}
    
// -----------------------------------------------------------------------------
// Name: ADN_Type_Choice::operator !=
// Created: JCR 2008-08-20
// -----------------------------------------------------------------------------
template < typename T >
bool ADN_Type_Choice< T >::operator !=( const T& val ) const
{
    return data_ != val;
}

// -----------------------------------------------------------------------------
// Name: ADN_Type_Choice::CheckData
// Created: JCR 2008-08-20
// -----------------------------------------------------------------------------
template < typename T >
bool ADN_Type_Choice< T >::CheckData( const T& data ) const
{    
    return std::find( choices_.begin(), choices_.end(), data ) != choices_.end();    
}

//-----------------------------------------------------------------------------
// Name: ADN_Type_Choice< T >::SetDataPrivate
// Created: JDY 03-07-21
//-----------------------------------------------------------------------------
template < typename T >
void ADN_Type_Choice< T >::SetDataPrivate( void *data )
{
    int i = *static_cast< int* >( data );
    assert( i < choices_.size() );
    T& tmp = choices_[ i ];
    if ( CheckData( tmp ) )
        SetDataPrivate( tmp, true );
}


//-----------------------------------------------------------------------------
// Name: ADN_Type_Choice< T >::SetDataPrivate
// Created: JDY 03-07-18
//-----------------------------------------------------------------------------
template < typename T >
void ADN_Type_Choice< T >::SetDataPrivate( const T& value, bool bCanBeUndone )
{
    if ( data_ != value )
    {
        if( bCanBeUndone )
        {
//            ADN_Workspace::GetWorkspace().AddCommand( new ADN_ChangeValueCommand_Enum< T, nb >( *this, data_, value ) );
        }

        data_ = value;
        emit DataChanged( &data_ );
    }  
}

// -----------------------------------------------------------------------------
// Name: ADN_Type_Enum::Initialize
/** @param  dest 
*/
// Created: AGN 2004-03-22
// -----------------------------------------------------------------------------
template < typename T >
void ADN_Type_Choice< T >::Initialize( ADN_Connector_Vector_ABC& dest ) const
{
    // send all the elements of the enum
    for ( CIT_ChoicesVector it = choices_.begin(); it < choices_.end(); ++it )    
        dest.AddItemNoEmit( (void*)&(*it) );    
    dest.AddItemNoEmit( 0 );
    dest.SetData( & const_cast< ADN_Type_Choice< T >* >( this )->data_ );
}