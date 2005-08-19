//*****************************************************************************
//
// $Created: NLD 2003-01-29 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_DynaObjectManager.cpp $
// $Author: Ape $
// $Modtime: 28/10/04 16:00 $
// $Revision: 4 $
// $Workfile: MOS_DynaObjectManager.cpp $
//
//*****************************************************************************

#include "MOS_Light2_pch.h"
#include "MOS_DynaObjectManager.h"

#include "MOS_DynaObject.h"

#ifndef MOS_USE_INLINE
#   include "MOS_DynaObjectManager.inl"
#endif


//-----------------------------------------------------------------------------
// Name: MOS_DynaObjectManager constructor
// Created: NLD 2003-01-29
//-----------------------------------------------------------------------------
MOS_DynaObjectManager::MOS_DynaObjectManager()
{
}


//-----------------------------------------------------------------------------
// Name: MOS_DynaObjectManager destructor
// Created: NLD 2003-01-29
//-----------------------------------------------------------------------------
MOS_DynaObjectManager::~MOS_DynaObjectManager()
{
}


//-----------------------------------------------------------------------------
// Name: MOS_DynaObjectManager::RegisterDynaObject
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
void MOS_DynaObjectManager::RegisterDynaObject( MOS_DynaObject& object )
{
    bool bOut = dynaObjectMap_.insert( std::make_pair( object.GetID(), &object ) ).second;
    assert( bOut );
}


//-----------------------------------------------------------------------------
// Name: MOS_DynaObjectManager::UnregisterDynaObject
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
void MOS_DynaObjectManager::UnregisterDynaObject( MOS_DynaObject& object )
{
    uint nOut = dynaObjectMap_.erase( object.GetID() );
    assert( nOut == 1 );
}


//-----------------------------------------------------------------------------
// Name: MOS_DynaObjectManager::DeleteAllDynaObjects
// Created: NLD 2002-07-17
//-----------------------------------------------------------------------------
void MOS_DynaObjectManager::DeleteAllDynaObjects()
{
    for( IT_DynaObjectMap it = dynaObjectMap_.begin(); it != dynaObjectMap_.end(); ++it )
    {
        MOS_DynaObject& object = *it->second;
        delete &object;
    }

    dynaObjectMap_.clear();
}


// -----------------------------------------------------------------------------
// Name: MOS_DynaObjectManager::ReadODB
/** @param  archive 
    @param  bAutomate 
    */
// Created: APE 2004-08-30
// -----------------------------------------------------------------------------
void MOS_DynaObjectManager::ReadODB( MT_XXmlInputArchive& archive )
{
    archive.BeginList( "Objets" );
    while( archive.NextListElement())
    {
        std::auto_ptr<MOS_DynaObject> spDynaObject( new MOS_DynaObject() );
        spDynaObject->ReadODB( archive );
        this->RegisterDynaObject( *(spDynaObject.release()));
    }
    archive.EndList();
}


// -----------------------------------------------------------------------------
// Name: MOS_DynaObjectManager::WriteODB
/** @param  archive 
*/
// Created: APE 2004-08-30
// -----------------------------------------------------------------------------
void MOS_DynaObjectManager::WriteODB( MT_XXmlOutputArchive& archive )
{
    archive.BeginList( "Objets", dynaObjectMap_.size() );
    for( IT_DynaObjectMap it = dynaObjectMap_.begin(); it != dynaObjectMap_.end(); ++it )
        (*it).second->WriteODB( archive );
    archive.EndList();
}

