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
const MIL_Army_ABC& MIL_Object_ABC::GetArmy() const
{
    assert( pArmy_ );
    return *pArmy_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Object_ABC::GetArmy
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
inline
MIL_Army_ABC& MIL_Object_ABC::GetArmy()
{
    assert( pArmy_ );
    return *pArmy_;
}

// -----------------------------------------------------------------------------
// Name: template< typename Extension > Extension& MIL_Object_ABC::GetAttribute
// Created: JCR 2008-06-04
// -----------------------------------------------------------------------------
template< typename Extension > 
const Extension& MIL_Object_ABC::GetAttribute() const 
{
    return tools::Extendable< ObjectAttribute_ABC >::Get< Extension >();
}

// -----------------------------------------------------------------------------
// Name: template< typename Extension > Extension& MIL_Object_ABC::RetrieveAttribute
// Created: LDC 2009-03-25
// -----------------------------------------------------------------------------
template< typename Extension > 
Extension* MIL_Object_ABC::RetrieveAttribute() 
{
    return tools::Extendable< ObjectAttribute_ABC >::Retrieve< Extension >();
}

// -----------------------------------------------------------------------------
// Name: template< typename T> void MIL_Object_ABC::ProcessAgentsInside
// Created: JCR 2008-08-28
// -----------------------------------------------------------------------------
template< typename Functor > 
void MIL_Object_ABC::ProcessAgentsInside( Functor& functor ) const
{
    interaction_.ProcessInteraction( functor );
}
