//*****************************************************************************
//
// $Created: NLD 2003-01-29 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_ObjectManager.cpp $
// $Author: Ape $
// $Modtime: 28/10/04 16:00 $
// $Revision: 4 $
// $Workfile: MOS_ObjectManager.cpp $
//
//*****************************************************************************

#include "MOS_Light2_pch.h"
#include "MOS_ObjectManager.h"

#include "MOS_Object_ABC.h"
#include "MOS_Object_Factory.h"

#ifndef MOS_USE_INLINE
#   include "MOS_ObjectManager.inl"
#endif


//-----------------------------------------------------------------------------
// Name: MOS_ObjectManager constructor
// Created: NLD 2003-01-29
//-----------------------------------------------------------------------------
MOS_ObjectManager::MOS_ObjectManager()
{
}


//-----------------------------------------------------------------------------
// Name: MOS_ObjectManager destructor
// Created: NLD 2003-01-29
//-----------------------------------------------------------------------------
MOS_ObjectManager::~MOS_ObjectManager()
{
}


//-----------------------------------------------------------------------------
// Name: MOS_ObjectManager::RegisterObject
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
void MOS_ObjectManager::RegisterObject( MOS_Object_ABC& object )
{
    bool bOut = ObjectMap_.insert( std::make_pair( object.GetID(), &object ) ).second;
    assert( bOut );
}


//-----------------------------------------------------------------------------
// Name: MOS_ObjectManager::UnregisterObject
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
void MOS_ObjectManager::UnregisterObject( MOS_Object_ABC& object )
{
    uint nOut = ObjectMap_.erase( object.GetID() );
    assert( nOut == 1 );
}


//-----------------------------------------------------------------------------
// Name: MOS_ObjectManager::DeleteAllObjects
// Created: NLD 2002-07-17
//-----------------------------------------------------------------------------
void MOS_ObjectManager::DeleteAllObjects()
{
    for( IT_ObjectMap it = ObjectMap_.begin(); it != ObjectMap_.end(); ++it )
        delete it->second;
    ObjectMap_.clear();
}


// -----------------------------------------------------------------------------
// Name: MOS_ObjectManager::ReadODB
/** @param  archive 
    @param  bAutomate 
    */
// Created: APE 2004-08-30
// -----------------------------------------------------------------------------
void MOS_ObjectManager::ReadODB( MOS_InputArchive& archive )
{
    archive.BeginList( "Objets" );
    while( archive.NextListElement())
    {
        MOS_Object_ABC* pObject = MOS_Object_Factory::Create( archive );
        this->RegisterObject( *pObject );
    }
    archive.EndList();
}


// -----------------------------------------------------------------------------
// Name: MOS_ObjectManager::WriteODB
/** @param  archive 
*/
// Created: APE 2004-08-30
// -----------------------------------------------------------------------------
void MOS_ObjectManager::WriteODB( MT_XXmlOutputArchive& archive )
{
    archive.BeginList( "Objets", ObjectMap_.size() );
    for( IT_ObjectMap it = ObjectMap_.begin(); it != ObjectMap_.end(); ++it )
        (*it).second->WriteODB( archive );
    archive.EndList();
}

