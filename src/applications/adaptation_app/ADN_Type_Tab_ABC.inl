//*****************************************************************************
//
// $Created: JDY 03-08-28 $
// $Archive: /MVW_v10/Build/SDK/ADN2/src/ADN_Type_Tab_ABC.inl $
// $Author: Ape $
// $Modtime: 21/10/04 17:47 $
// $Revision: 1 $
// $Workfile: ADN_Type_Tab_ABC.inl $
//
//*****************************************************************************


//-----------------------------------------------------------------------------
// Name: ADN_Type_Line_ABC constructor
// Created: JDY 03-09-01
//-----------------------------------------------------------------------------
template <class T>
ADN_Type_Line_ABC<T>::ADN_Type_Line_ABC(int w)
:   ADN_Type_Vector_ABC< T >( false )
{
    for( int i=0;i<w;++i)
        AddItem(new T());
}


//-----------------------------------------------------------------------------
// Name: ADN_Type_Line_ABC destructor
// Created: JDY 03-09-01
//-----------------------------------------------------------------------------
template <class T>
ADN_Type_Line_ABC<T>::~ADN_Type_Line_ABC()
{
    mt_clear_owned_ptrs( *this );
}

// -----------------------------------------------------------------------------
// Name: ADN_Type_Tab_ABC::GetItemName
/** @return 
*/
// Created: AGN 2004-05-18
// -----------------------------------------------------------------------------
template <class T>
std::string ADN_Type_Line_ABC<T>::GetItemName()
{
    return std::string();
}

// =============================================================================
// 
// =============================================================================

//-----------------------------------------------------------------------------
// Name: ADN_Type_Tab_ABC constructor
// Created: JDY 03-09-01
//-----------------------------------------------------------------------------
template <class T>
ADN_Type_Tab_ABC<T>::ADN_Type_Tab_ABC(int w,int h)
:   ADN_Type_Vector_ABC< ADN_Type_Line_ABC<T> >( false )
{
    for( int i=0;i<h;++i)
        AddItem(new ADN_Type_Line_ABC<T>(w));
}


//-----------------------------------------------------------------------------
// Name: ADN_Type_Tab_ABC destructor
// Created: JDY 03-09-01
//-----------------------------------------------------------------------------
template <class T>
ADN_Type_Tab_ABC<T>::~ADN_Type_Tab_ABC()
{
    mt_clear_owned_ptrs( *this );
}


//-----------------------------------------------------------------------------
// Name: ADN_Type_Tab_ABC<T>::Get
// Created: JDY 03-09-01
//-----------------------------------------------------------------------------
template <class T>
T& ADN_Type_Tab_ABC<T>::Get(int i,int j)
{
    ADN_Type_Line_ABC<T>* pLine=operator [](i);
    return * ( pLine->operator[](j) );
}
