//*****************************************************************************
// Created: JVT 02-10-24
//*****************************************************************************

#include "MOS_Object.h"

//-----------------------------------------------------------------------------
// Name: MOS_ObjectManager::RemoveObjects
// Created: FBD 02-11-20
//-----------------------------------------------------------------------------
inline
void MOS_ObjectManager::DestroyObjects()
{
    for( IT_ObjectPtrMap itObject = objectMap_.begin(); itObject != objectMap_.end(); ++itObject )
        delete itObject->second;
}


//-----------------------------------------------------------------------------
// Name: MOS_ObjectManager::GetObjectFromId
// Created: FBD 02-11-18
//-----------------------------------------------------------------------------
inline
MOS_Object& MOS_ObjectManager::GetObjectFromId( uint32 nObjectId )
{
    IT_ObjectPtrMap itObject = objectMap_.find( nObjectId );
    assert( itObject != objectMap_.end() );
    return *itObject->second;
}

//-----------------------------------------------------------------------------
// Name: MOS_ObjectManager::GetNodeFromIdx
// Created: FBD 02-11-18
//-----------------------------------------------------------------------------
inline
MOS_Node* MOS_ObjectManager::GetNodeFromIdx( uint nIdx )
{
    assert( nodeVector_.size() > nIdx );
    return &nodeVector_[ nIdx ];
}


//-----------------------------------------------------------------------------
// Name: MOS_ObjectManager::GetLineFromIdx
// Created: FBD 03-04-24
//-----------------------------------------------------------------------------
inline
MOS_Line* MOS_ObjectManager::GetLineFromIdx( uint nIdx )
{
    assert( lineVector_.size() > nIdx );
    return &lineVector_[ nIdx ];
}


//-----------------------------------------------------------------------------
// Name: MOS_ObjectManager::GetTriFromIdx
// Created: FBD 03-04-24
//-----------------------------------------------------------------------------
inline
MOS_Tri* MOS_ObjectManager::GetTriFromIdx( uint nIdx )
{
    assert( triVector_.size() > nIdx );
    return &triVector_[ nIdx ];
}

