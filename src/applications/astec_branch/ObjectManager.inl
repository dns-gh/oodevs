//*****************************************************************************
//
// $Created: NLD 2003-01-29 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/ObjectManager.inl $
// $Author: Ape $
// $Modtime: 5/05/04 15:56 $
// $Revision: 2 $
// $Workfile: ObjectManager.inl $
//
//*****************************************************************************


//-----------------------------------------------------------------------------
// Name: ObjectManager::FindObject
// Created: NLD 2003-01-29
//-----------------------------------------------------------------------------
inline
Object_ABC* ObjectManager::FindObject( uint nID ) const
{
    CIT_ObjectMap itObject = ObjectMap_.find( nID );
    if( itObject == ObjectMap_.end() )
        return 0;
    return itObject->second;
}


// -----------------------------------------------------------------------------
// Name: ObjectManager::GetObjects
/** @return 
*/
// Created: APE 2004-05-05
// -----------------------------------------------------------------------------
inline
const ObjectManager::T_ObjectMap& ObjectManager::GetObjects() const
{
    return ObjectMap_;
}
