//*****************************************************************************
//
// $Created: JDY 03-07-22 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Type_Enum.inl $
// $Author: Ape $
// $Modtime: 17/02/05 15:46 $
// $Revision: 4 $
// $Workfile: ADN_Type_Enum.inl $
//
//*****************************************************************************

#include "ADN_ChangeValueCommand_Enum.h"
#include "ADN_Workspace.h"

// =============================================================================
// 
// =============================================================================

template <class T,int nb>
typename ADN_Type_Enum< T, nb >::T_Converter ADN_Type_Enum< T, nb >::converter_ = 0;

// -----------------------------------------------------------------------------
// Name: ADN_Type_Enum::ConvertValue
/** @param  nVal 
    @param  nConversion 
    @return 
*/
// Created: AGN 2004-05-13
// -----------------------------------------------------------------------------
template <class T,int nb>
std::string ADN_Type_Enum< T, nb >::ConvertValue( T nVal, ENT_Tr_ABC::E_Conversion nConversion )
{
    if( converter_ == 0 )
        return itostring( nVal );
    else
        return converter_( nVal, nConversion );
}

// -----------------------------------------------------------------------------
// Name: ADN_Type_Enum::SetConverter
/** @param  enumConverter 
*/
// Created: AGN 2004-05-13
// -----------------------------------------------------------------------------
template <class T,int nb>
void ADN_Type_Enum< T, nb >::SetConverter( typename ADN_Type_Enum< T, nb >::T_Converter enumConverter )
{
    converter_ = enumConverter;
}

// =============================================================================
// 
// =============================================================================

//-----------------------------------------------------------------------------
// Name: ADN_Type_Enum constructor
// Created: JDY 03-07-22
//-----------------------------------------------------------------------------
template <class T,int nb>
ADN_Type_Enum<T,nb>::ADN_Type_Enum()
: ADN_Connector_Vector_ABC()
, data_((T)0)
{
}



//-----------------------------------------------------------------------------
// Name: ADN_Type_Enum constructor
// Created: JDY 03-07-22
//-----------------------------------------------------------------------------
template <class T,int nb>
ADN_Type_Enum<T,nb>::ADN_Type_Enum( const T& value )
: ADN_Connector_Vector_ABC()
, data_( value )
{
}



//-----------------------------------------------------------------------------
// Name: ADN_Type_Enum destructor
// Created: JDY 03-07-18
//-----------------------------------------------------------------------------
template <class T,int nb>
ADN_Type_Enum<T,nb>::~ADN_Type_Enum()
{
}


//-----------------------------------------------------------------------------
// Name: ADN_Type_Enum<T,nb>::T_TypePtr 
// Created: JDY 03-07-18
//-----------------------------------------------------------------------------
template <class T,int nb>
T ADN_Type_Enum<T,nb>::GetData()
{
    return data_;
}

//-----------------------------------------------------------------------------
// Name: ADN_Type_Enum<T,nb>::operator =
// Created: JDY 03-07-18
//-----------------------------------------------------------------------------
template <class T,int nb>
ADN_Type_Enum<T,nb>& ADN_Type_Enum<T,nb>::operator =(const ADN_Type_Enum& o)
{
    SetData(o.GetData(),false);
    return *this;
}

//-----------------------------------------------------------------------------
// Name: ADN_Type_Enum<T,nb>::operator =
// Created: JDY 03-07-18
//-----------------------------------------------------------------------------
template <class T,int nb>
ADN_Type_Enum<T,nb>& ADN_Type_Enum<T,nb>::operator =(const T& val)
{
    SetDataPrivate(val,false);
    return *this;
}

//-----------------------------------------------------------------------------
// Name: ADN_Type_ABC<T>::operator 
// Created: JDY 03-07-22
//-----------------------------------------------------------------------------
template <class T,int nb>
bool ADN_Type_Enum<T,nb>::operator ==(const ADN_Type_Enum& o) const
{
    return data_==o.data_;
}

//-----------------------------------------------------------------------------
// Name: ADN_Type_ABC<T>::operator 
// Created: JDY 03-07-22
//-----------------------------------------------------------------------------
template <class T,int nb>
bool ADN_Type_Enum<T,nb>::operator ==(const T& val) const
{
    return data_==val;
}
    
//-----------------------------------------------------------------------------
// Name: ADN_Type_ABC<T>::operator 
// Created: JDY 03-07-22
//-----------------------------------------------------------------------------
template <class T,int nb>
bool ADN_Type_Enum<T,nb>::operator !=(const ADN_Type_Enum& o) const
{
    return data_!=o.data_;
}

//-----------------------------------------------------------------------------
// Name: ADN_Type_ABC<T>::operator 
// Created: JDY 03-07-22
//-----------------------------------------------------------------------------
template <class T,int nb>
bool ADN_Type_Enum<T,nb>::operator !=(const T& val) const
{
    return data_!=val;
}


//-----------------------------------------------------------------------------
// Name: ADN_Type_Enum<T,nb>::SetDataPrivate
// Created: JDY 03-07-21
//-----------------------------------------------------------------------------
template <class T,int nb>
void ADN_Type_Enum<T,nb>::SetDataPrivate(void *data)
{
    SetDataPrivate(*(T*)data,true);
}


//-----------------------------------------------------------------------------
// Name: ADN_Type_Enum<T,nb>::SetDataPrivate
// Created: JDY 03-07-18
//-----------------------------------------------------------------------------
template <class T,int nb>
void ADN_Type_Enum<T,nb>::SetDataPrivate(const T& value, bool bCanBeUndone )
{
    if ( data_!= value)
    {
        if( bCanBeUndone )
        {
            ADN_Workspace::GetWorkspace().AddCommand( new ADN_ChangeValueCommand_Enum< T, nb >( *this, data_, value ) );
        }

        data_     =value;
        emit DataChanged(&data_);
    }  
}


// -----------------------------------------------------------------------------
// Name: ADN_Type_Enum::Initialize
/** @param  dest 
*/
// Created: AGN 2004-03-22
// -----------------------------------------------------------------------------
template <class T,int nb>
void ADN_Type_Enum<T,nb>::Initialize( ADN_Connector_Vector_ABC& dest ) const
{
    // send all the elements of the enum
    for (int i=0;i<nb;++i)
    {
        T tmp=(T)i;
        dest.AddItemNoEmit( &tmp );
    }
    dest.AddItemNoEmit( 0 );

    dest.SetData( & const_cast< ADN_Type_Enum<T,nb>* >( this )->data_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Type_Enum::GetName
/** @return 
*/
// Created: AGN 2004-05-13
// -----------------------------------------------------------------------------
template <class T,int nb>
std::string ADN_Type_Enum<T,nb>::GetNodeName()
{
    return strDataName_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Type_Enum::SetDataName
/** @param  strName 
*/
// Created: AGN 2004-05-13
// -----------------------------------------------------------------------------
template <class T,int nb>
void ADN_Type_Enum<T,nb>::SetDataName( const std::string& strName )
{
    strDataName_ = strName;
}
