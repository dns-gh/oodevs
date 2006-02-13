// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: NLD 2004-03-18 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/Team.cpp $
// $Author: Ape $
// $Modtime: 16/02/05 15:54 $
// $Revision: 7 $
// $Workfile: Team.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "astec_pch.h"
#include "Team.h"

#include "App.h"
#include "Gtia.h"
#include "Tools.h"
#include "ObjectKnowledge.h"
#include "Object_ABC.h"
// -----------------------------------------------------------------------------
// Name: Team constructor
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
Team::Team()
{
}

// -----------------------------------------------------------------------------
// Name: Team constructor
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
Team::Team( uint nID, DIN::DIN_Input& input )
: strName_              ()
, nID_                  ( nID  )
{
    input >> strName_;
}
    
// -----------------------------------------------------------------------------
// Name: Team destructor
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
Team::~Team()
{
    this->DestroyAllObjectKnowledges();
//    this->DeleteAllGtias();

}


// -----------------------------------------------------------------------------
// Name: Team::ReadODB
// Created: APE 2004-09-06
// -----------------------------------------------------------------------------
void Team::ReadODB( InputArchive& archive, bool bReadGtias )
{
    archive.Section( "Armee" );

    archive.ReadAttribute( "id", nID_ );
    archive.ReadAttribute( "nom", strName_ );

    if( ! bReadGtias )
    {
        archive.EndSection();
        return;
    }

    archive.BeginList( "GroupesConnaissance" );
    while( archive.NextListElement() )
    {
        std::auto_ptr<Gtia> spGtia( new Gtia() );
        spGtia->ReadODB( archive );
        this->RegisterGtia( *(spGtia.release()) );
    }
    archive.EndList();

    archive.BeginList( "Diplomatie" );
    while( archive.NextListElement() )
    {
        archive.Section( "Armee" );

        T_Diplomaty diplomaty;
        archive.ReadAttribute( "nom", diplomaty.first );
        std::string strRelationship;
        archive.ReadAttribute( "relation", strRelationship );
        diplomaty.second = Tools::GetDiplomatyFromName( strRelationship );
        diplomaty_.push_back( diplomaty );

        archive.EndSection();
    }
    archive.EndList();

    archive.EndSection();

    App::GetApp().NotifyTeamCreated( *this );
}


// -----------------------------------------------------------------------------
// Name: Team::WriteODB
/** @param  archive 
*/
// Created: APE 2004-10-26
// -----------------------------------------------------------------------------
void Team::WriteODB( MT_OutputArchive_ABC& archive )
{
    archive.Section( "Armee" );

    archive.WriteAttribute( "id", nID_ );
    archive.WriteAttribute( "nom", strName_ );

    archive.BeginList( "GroupesConnaissance", gtias_.size() );
    for( CIT_GtiaMap it = gtias_.begin(); it != gtias_.end(); ++it )
        it->second->WriteODB( archive );
    archive.EndList();

    archive.BeginList( "Diplomatie", diplomaty_.size() );
    for( CIT_DiplomatyVector itd = diplomaty_.begin(); itd != diplomaty_.end(); ++itd )
    {
        archive.Section( "Armee" );
        archive.WriteAttribute( "nom", (*itd).first );
        archive.WriteAttribute( "relation", Tools::ConvertDiplomaty( (*itd).second ) );
        archive.EndSection();
    }
    archive.EndList();

    archive.EndSection();
}


// -----------------------------------------------------------------------------
// Name: Team::DestroyAllObjectKnowledges
// Created: NLD 2004-03-26
// -----------------------------------------------------------------------------
void Team::DestroyAllObjectKnowledges()
{
    for( CIT_ObjectKnowledgeMap it = objectKnowledges_.begin(); it != objectKnowledges_.end(); ++it )
        delete it->second;
    objectKnowledges_.clear();
}


// -----------------------------------------------------------------------------
// Name: Team::OnReceiveMsgObjectKnowledgeCreation
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
bool Team::OnReceiveMsgObjectKnowledgeCreation( const ASN1T_MsgObjectKnowledgeCreation& asnMsg )
{
    if( objectKnowledges_.find( asnMsg.oid_connaissance ) != objectKnowledges_.end() )
        return false;

    ObjectKnowledge* pObjectKnowledge = new ObjectKnowledge( asnMsg, *this );
    objectKnowledges_.insert( std::make_pair( pObjectKnowledge->GetID(), pObjectKnowledge ) );

    App::GetApp().NotifyObjectKnowledgeCreated( *this, *pObjectKnowledge );
    return true;
}

// -----------------------------------------------------------------------------
// Name: Team::CreateGtia
// Created: AGE 2005-09-21
// -----------------------------------------------------------------------------
Gtia* Team::CreateGtia( uint32 nId )
{
    if( FindGtia( nId ) )
        return FindGtia( nId );
    Gtia* gtia = new Gtia( nId );
    RegisterGtia( *gtia );
    return gtia;
}

// -----------------------------------------------------------------------------
// Name: Team::OnReceiveMsgObjectKnowledgeUpdate
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void Team::OnReceiveMsgObjectKnowledgeUpdate( const ASN1T_MsgObjectKnowledgeUpdate& asnMsg )
{
    IT_ObjectKnowledgeMap itObjectKnowledge = objectKnowledges_.find( asnMsg.oid_connaissance );
    assert( itObjectKnowledge != objectKnowledges_.end() );

    itObjectKnowledge->second->Update( asnMsg );
    App::GetApp().NotifyObjectKnowledgeUpdated( *this, *(itObjectKnowledge->second) );
}


// -----------------------------------------------------------------------------
// Name: Team::OnReceiveMsgObjectKnowledgeDestruction
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void Team::OnReceiveMsgObjectKnowledgeDestruction( const ASN1T_MsgObjectKnowledgeDestruction& asnMsg )
{
    IT_ObjectKnowledgeMap itObjectKnowledge = objectKnowledges_.find( asnMsg.oid_connaissance );
    assert( itObjectKnowledge != objectKnowledges_.end() );
    App::GetApp().NotifyObjectKnowledgeDeleted( *this, *(itObjectKnowledge->second) );
    Object_ABC::GetIDManagerForObjectType( itObjectKnowledge->second->GetObjectTypeID() ).ReleaseIdentifier( itObjectKnowledge->second->GetID() ) ;
    delete itObjectKnowledge->second;
    objectKnowledges_.erase( itObjectKnowledge );
}


// -----------------------------------------------------------------------------
// Name: Team::ChangeDiplomaty
/** @param  team 
    @param  nRelationship 
    */
// Created: APE 2004-10-29
// -----------------------------------------------------------------------------
void Team::ChangeDiplomaty( Team& team, ASN1T_EnumDiplomatie nRelationship )
{
    std::string strTeamName = team.GetName();
    for( IT_DiplomatyVector it = diplomaty_.begin(); it != diplomaty_.end(); ++it )
    {
        if( (*it).first == strTeamName )
        {
            (*it).second = nRelationship;
            return;
        }
    }
    T_Diplomaty diplomaty;
    diplomaty.first = strTeamName;
    diplomaty.second = nRelationship;
    diplomaty_.push_back( diplomaty );
}
