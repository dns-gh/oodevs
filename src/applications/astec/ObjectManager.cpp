//*****************************************************************************
//
// $Created: NLD 2003-01-29 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/ObjectManager.cpp $
// $Author: Ape $
// $Modtime: 28/10/04 16:00 $
// $Revision: 4 $
// $Workfile: ObjectManager.cpp $
//
//*****************************************************************************

#include "astec_pch.h"
#include "ObjectManager.h"
#include "App.h"
#include "Object_ABC.h"
#include "Object_Factory.h"

//-----------------------------------------------------------------------------
// Name: ObjectManager constructor
// Created: NLD 2003-01-29
//-----------------------------------------------------------------------------
ObjectManager::ObjectManager()
{
}


//-----------------------------------------------------------------------------
// Name: ObjectManager destructor
// Created: NLD 2003-01-29
//-----------------------------------------------------------------------------
ObjectManager::~ObjectManager()
{
}

// -----------------------------------------------------------------------------
// Name: ObjectManager::CreateObject
// Created: AGE 2005-09-21
// -----------------------------------------------------------------------------
void ObjectManager::CreateObject( const ASN1T_MsgObjectCreation& asnMsg )
{
    if( FindObject( asnMsg.oid ) )
        return;
    Object_ABC* pObject = Object_Factory::Create( asnMsg );
    RegisterObject( *pObject );
    App::GetApp().NotifyObjectCreated( *pObject );
}

//-----------------------------------------------------------------------------
// Name: ObjectManager::RegisterObject
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
void ObjectManager::RegisterObject( Object_ABC& object )
{
    if( ! ObjectMap_.insert( std::make_pair( object.GetID(), &object ) ).second )
        RUNTIME_ERROR;
}


//-----------------------------------------------------------------------------
// Name: ObjectManager::UnregisterObject
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
void ObjectManager::UnregisterObject( Object_ABC& object )
{
    if( ObjectMap_.erase( object.GetID() ) != 1 )
        RUNTIME_ERROR;
}


//-----------------------------------------------------------------------------
// Name: ObjectManager::DeleteAllObjects
// Created: NLD 2002-07-17
//-----------------------------------------------------------------------------
void ObjectManager::DeleteAllObjects()
{
    for( IT_ObjectMap it = ObjectMap_.begin(); it != ObjectMap_.end(); ++it )
        delete it->second;
    ObjectMap_.clear();
}


// -----------------------------------------------------------------------------
// Name: ObjectManager::ReadODB
/** @param  archive 
    @param  bAutomate 
    */
// Created: APE 2004-08-30
// -----------------------------------------------------------------------------
void ObjectManager::ReadODB( InputArchive& archive )
{
    archive.BeginList( "Objets" );
    while( archive.NextListElement())
    {
        Object_ABC* pObject = Object_Factory::Create( archive );
        this->RegisterObject( *pObject );
    }
    archive.EndList();
}


// -----------------------------------------------------------------------------
// Name: ObjectManager::WriteODB
/** @param  archive 
*/
// Created: APE 2004-08-30
// -----------------------------------------------------------------------------
void ObjectManager::WriteODB( MT_XXmlOutputArchive& archive )
{
    archive.BeginList( "Objets", ObjectMap_.size() );
    for( IT_ObjectMap it = ObjectMap_.begin(); it != ObjectMap_.end(); ++it )
        (*it).second->WriteODB( archive );
    archive.EndList();
}

