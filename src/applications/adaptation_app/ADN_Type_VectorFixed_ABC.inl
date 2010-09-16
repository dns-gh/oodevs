//*****************************************************************************
//
// $Created: JDY 03-09-01 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Type_VectorFixed_ABC.inl $
// $Author: Ape $
// $Modtime: 14/01/05 10:08 $
// $Revision: 3 $
// $Workfile: ADN_Type_VectorFixed_ABC.inl $
//
//*****************************************************************************


//-----------------------------------------------------------------------------
// Name: ADN_Type_VectorFixed_ABC constructor
// Created: JDY 03-08-28
//-----------------------------------------------------------------------------
template <class T,class Cmp>
ADN_Type_VectorFixed_ABC<T,Cmp>::ADN_Type_VectorFixed_ABC( const char* szName )
:   ADN_Type_Vector_ABC<T>( true, szName )
{
}

//-----------------------------------------------------------------------------
// Name: ADN_Type_VectorFixed_ABC destructor
// Created: JDY 03-08-28
//-----------------------------------------------------------------------------
template <class T,class Cmp>
ADN_Type_VectorFixed_ABC<T,Cmp>::~ADN_Type_VectorFixed_ABC()
{
    mt_clear_owned_ptrs( *this );
}


//-----------------------------------------------------------------------------
// Name: ADN_Type_VectorFixed_ABC<T>::AutoCreatePrivate
// Created: JDY 03-08-28
//-----------------------------------------------------------------------------
template <class T,class Cmp>
void ADN_Type_VectorFixed_ABC<T,Cmp>::AutoCreatePrivate(void* ptr)
{
    if( ptr && std::find_if( begin(), end(), Cmp((T_Item*)ptr) ) == end() ) 
    {
        T* pNewItem = new T((T_Item*)ptr);
        AddItemPrivate( pNewItem );
        EndVector();
        emit ItemAdded( pNewItem );
    }
}
