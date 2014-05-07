//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Objects/MIL_Object_ABC.inl $
// $Author: Nld $
// $Modtime: 26/11/04 17:15 $
// $Revision: 3 $
// $Workfile: MIL_Object_ABC.inl $
//
//*****************************************************************************

// -----------------------------------------------------------------------------
// Name: MIL_Object_ABC::IsMarkedForDestruction
// Created: NLD 2003-10-10
// -----------------------------------------------------------------------------
inline
bool MIL_Object_ABC::IsMarkedForDestruction() const
{
    return bMarkedForDestruction_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Object_ABC::IsReadyForDeletion
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
inline
bool MIL_Object_ABC::IsReadyForDeletion() const
{
    return bReadyForDeletion_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Object_ABC::GetArmy
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
inline
const MIL_Army_ABC* MIL_Object_ABC::GetArmy() const
{
    return pArmy_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Object_ABC::GetArmy
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
inline
MIL_Army_ABC* MIL_Object_ABC::GetArmy()
{
    return pArmy_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Object_ABC::GetAttribute
// Created: JCR 2008-06-18
// -----------------------------------------------------------------------------
template< typename T > T& MIL_Object_ABC::GetAttribute()
{
    return GetAttribute< T, T >();
}

// -----------------------------------------------------------------------------
// Name: MIL_Object_ABC::GetAttribute
// Created: JCR 2008-06-18
// -----------------------------------------------------------------------------
template< typename T > const T& MIL_Object_ABC::GetAttribute() const
{
    return tools::Extendable< ObjectAttribute_ABC >::Get< T >();
}

// -----------------------------------------------------------------------------
// Name: MIL_Object_ABC::GetAttribute
// Created: JCR 2008-05-26
// -----------------------------------------------------------------------------
template< typename T, typename I > T& MIL_Object_ABC::GetAttribute()
{
    I* attribute = tools::Extendable< ObjectAttribute_ABC >::Retrieve< I >();
    if( !attribute )
    {
        attribute = new T();
        tools::Extendable< ObjectAttribute_ABC >::Attach< I >( *attribute );
        Register( attribute );
    }
    return *static_cast< T* >( attribute );
}
// -----------------------------------------------------------------------------
// Name: MIL_Object_ABC::RetrieveAttribute
// Created: JCR 2008-06-06
// -----------------------------------------------------------------------------
template< typename T >
T* MIL_Object_ABC::RetrieveAttribute()
{
    return tools::Extendable< ObjectAttribute_ABC >::Retrieve< T >();
}

// -----------------------------------------------------------------------------
// Name: MIL_Object_ABC::RetrieveAttribute
// Created: LDC 2009-03-25
// -----------------------------------------------------------------------------
template< typename T >
const T* MIL_Object_ABC::RetrieveAttribute() const
{
    return tools::Extendable< ObjectAttribute_ABC >::Retrieve< T >();
}

// -----------------------------------------------------------------------------
// Name: MIL_Object_ABC::AddCapacity
// Created: JCR 2008-05-26
// -----------------------------------------------------------------------------
template< typename T >
void MIL_Object_ABC::AddCapacity( T* capacity )
{
    tools::Extendable< ObjectCapacity_ABC >::Attach( *capacity );
    Register( static_cast< ObjectCapacity_ABC *>( capacity ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_Object_ABC::ProcessAgentsInside
// Created: JCR 2008-08-28
// -----------------------------------------------------------------------------
template< typename Functor >
void MIL_Object_ABC::ProcessAgentsInside( Functor functor ) const
{
    interaction_.ProcessInteraction( functor );
}

// -----------------------------------------------------------------------------
// Name: MIL_Object_ABC::SetAttribute
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
template< typename T, typename I >
void MIL_Object_ABC::SetAttribute( const T& attribute )
{
    GetAttribute< T, I >() = attribute;
}

// -----------------------------------------------------------------------------
// Name: MIL_Object_ABC::GetID
// Created: BCI 2011-01-12
// -----------------------------------------------------------------------------
inline
unsigned int MIL_Object_ABC::GetID() const
{
    return id_;
}
