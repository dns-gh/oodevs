// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "ADN_ChangeValueCommand_Helper.h"
#include "ADN_Workspace.h"

//-----------------------------------------------------------------------------
// Name: ADN_Type_ABC constructor
// Created: JDY 03-06-23
//-----------------------------------------------------------------------------
template <class T>
ADN_Type_ABC<T>::ADN_Type_ABC()
    : ADN_Connector_ABC()
    , bUndoAvailable_( true )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: ADN_Type_ABC constructor
// Created: JDY 03-07-02
//-----------------------------------------------------------------------------
template <class T>
ADN_Type_ABC<T>::ADN_Type_ABC( const T& val )
    : ADN_Connector_ABC()
    , val_( val )
    , bUndoAvailable_( true )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: ADN_Type_ABC destructor
// Created: JDY 03-06-23
//-----------------------------------------------------------------------------
template <class T>
ADN_Type_ABC<T>::~ADN_Type_ABC()
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: ADN_Type_ABC<T>::operator 
// Created: JDY 03-07-02
//-----------------------------------------------------------------------------
template <class T>
ADN_Type_ABC<T>& ADN_Type_ABC<T>::operator =(const T& val)
{
    SetData(val,false);
    return *this;
}

//-----------------------------------------------------------------------------
// Name: ADN_Type_ABC<T>::operator 
// Created: JDY 03-07-22
//-----------------------------------------------------------------------------
template <class T>
bool ADN_Type_ABC<T>::operator ==(const T& val) const
{
    return val_==val;
}

//-----------------------------------------------------------------------------
// Name: ADN_Type_ABC<T>::operator 
// Created: JDY 03-07-22
//-----------------------------------------------------------------------------
template <class T>
bool ADN_Type_ABC<T>::operator !=(const T& val) const
{
    return val_!=val;
}

// -----------------------------------------------------------------------------
// Name: ADN_Type_ABC::Initialize
// Created: APE 2005-01-12
// -----------------------------------------------------------------------------
template <class T>
void ADN_Type_ABC<T>::Initialize( ADN_Connector_ABC& dest ) const
{
    dest.SetData( (void*)&val_ );
}

//-----------------------------------------------------------------------------
// Name: ADN_Type_ABC<T>::SetData
// Created: JDY 03-06-23
//-----------------------------------------------------------------------------
template <class T>
void ADN_Type_ABC<T>::SetData(const T& data, bool bCanBeUndone )
{
    if (val_!=data)
    {   
        if( bCanBeUndone && bUndoAvailable_ )
        {
            ADN_Workspace::GetWorkspace().AddCommand( ADN_ChangeValueCommand_Chooser< T >::CreateCommand( *this, val_, data ) );
        }
        
        val_ = data;
        // emit signal
        emit DataChanged((void*)&val_);
    }
}

//-----------------------------------------------------------------------------
// Name: ADN_Type_ABC<T>::GetData
// Created: JDY 03-06-23
//-----------------------------------------------------------------------------
template <class T>
inline
const T& ADN_Type_ABC<T>::GetData() const
{
    return val_;
}

//-----------------------------------------------------------------------------
// Name: ADN_Type_ABC<T>::SetDataPrivate
// Created: JDY 03-07-01
//-----------------------------------------------------------------------------
template <class T>
void  ADN_Type_ABC<T>::SetDataPrivate(void *data)
{    
    SetData(*static_cast< T* >( data ),true);
}

// -----------------------------------------------------------------------------
// Name: ADN_Type_ABC::GetName
/** @return 
*/
// Created: AGN 2004-05-13
// -----------------------------------------------------------------------------
template <class T>
std::string ADN_Type_ABC< T >::GetNodeName()
{
    return strDataName_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Type_ABC::SetDataName
/** @param  strName 
*/
// Created: AGN 2004-05-13
// -----------------------------------------------------------------------------
template <class T>
void ADN_Type_ABC< T >::SetDataName( const std::string& strName )
{
    strDataName_ = strName;
}

// -----------------------------------------------------------------------------
// Name: ADN_Type_ABC::SetUndoAvailable
// Created: AGN 2004-06-15
// -----------------------------------------------------------------------------
template <class T>
void ADN_Type_ABC< T >::SetUndoAvailable( bool b )
{
    bUndoAvailable_ = b;
}
