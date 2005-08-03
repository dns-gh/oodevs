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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_Team.cpp $
// $Author: Ape $
// $Modtime: 16/02/05 15:54 $
// $Revision: 7 $
// $Workfile: MOS_Team.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "MOS_Light2_Pch.h"
#include "MOS_Team.h"

#include "MOS_App.h"
#include "MOS_Gtia.h"
#include "MOS_Tools.h"
#include "MOS_DynaObjectKnowledge.h"

#ifndef MOS_USE_INLINE
#   include "MOS_Team.inl"
#endif


// -----------------------------------------------------------------------------
// Name: MOS_Team constructor
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
MOS_Team::MOS_Team()
{
}

// -----------------------------------------------------------------------------
// Name: MOS_Team constructor
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
MOS_Team::MOS_Team( uint nID, DIN::DIN_Input& input )
: strName_              ()
, nID_                  ( nID  )
{
    input >> strName_;
}
    
// -----------------------------------------------------------------------------
// Name: MOS_Team destructor
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
MOS_Team::~MOS_Team()
{
    this->DestroyAllObjectKnowledges();
    this->DeleteAllGtias();

}


// -----------------------------------------------------------------------------
// Name: MOS_Team::ReadODB
// Created: APE 2004-09-06
// -----------------------------------------------------------------------------
void MOS_Team::ReadODB( MT_InputArchive_ABC& archive, bool bReadGtias )
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
        std::auto_ptr<MOS_Gtia> spGtia( new MOS_Gtia() );
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
        diplomaty.second = MOS_Tools::GetDiplomatyFromName( strRelationship );
        diplomaty_.push_back( diplomaty );

        archive.EndSection();
    }
    archive.EndList();

    archive.EndSection();

    MOS_App::GetApp().NotifyTeamCreated( *this );
}


// -----------------------------------------------------------------------------
// Name: MOS_Team::WriteODB
/** @param  archive 
*/
// Created: APE 2004-10-26
// -----------------------------------------------------------------------------
void MOS_Team::WriteODB( MT_OutputArchive_ABC& archive )
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
        archive.WriteAttribute( "relation", MOS_Tools::ConvertDiplomaty( (*itd).second ) );
        archive.EndSection();
    }
    archive.EndList();

    archive.EndSection();
}


// -----------------------------------------------------------------------------
// Name: MOS_Team::DestroyAllObjectKnowledges
// Created: NLD 2004-03-26
// -----------------------------------------------------------------------------
void MOS_Team::DestroyAllObjectKnowledges()
{
    for( CIT_ObjectKnowledgeMap it = objectKnowledges_.begin(); it != objectKnowledges_.end(); ++it )
        delete it->second;
    objectKnowledges_.clear();
}


// -----------------------------------------------------------------------------
// Name: MOS_Team::OnReceiveMsgObjectKnowledgeCreation
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void MOS_Team::OnReceiveMsgObjectKnowledgeCreation( const ASN1T_MsgObjectKnowledgeCreation& asnMsg )
{
    if( objectKnowledges_.find( asnMsg.oid_connaissance ) != objectKnowledges_.end() )
        return;

    MOS_DynaObjectKnowledge* pObjectKnowledge = new MOS_DynaObjectKnowledge( asnMsg, *this );
    objectKnowledges_.insert( std::make_pair( pObjectKnowledge->GetID(), pObjectKnowledge ) );

    MOS_App::GetApp().NotifyObjectKnowledgeCreated( *this, *pObjectKnowledge );
}


// -----------------------------------------------------------------------------
// Name: MOS_Team::OnReceiveMsgObjectKnowledgeUpdate
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void MOS_Team::OnReceiveMsgObjectKnowledgeUpdate( const ASN1T_MsgObjectKnowledgeUpdate& asnMsg )
{
    IT_ObjectKnowledgeMap itObjectKnowledge = objectKnowledges_.find( asnMsg.oid_connaissance );
    assert( itObjectKnowledge != objectKnowledges_.end() );

    itObjectKnowledge->second->Update( asnMsg );
    MOS_App::GetApp().NotifyObjectKnowledgeUpdated( *this, *(itObjectKnowledge->second) );
}


// -----------------------------------------------------------------------------
// Name: MOS_Team::OnReceiveMsgObjectKnowledgeDestruction
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void MOS_Team::OnReceiveMsgObjectKnowledgeDestruction( const ASN1T_MsgObjectKnowledgeDestruction& asnMsg )
{
    IT_ObjectKnowledgeMap itObjectKnowledge = objectKnowledges_.find( asnMsg.oid_connaissance );
    assert( itObjectKnowledge != objectKnowledges_.end() );

    MOS_App::GetApp().NotifyObjectKnowledgeDeleted( *this, *(itObjectKnowledge->second) );

    delete itObjectKnowledge->second;
    objectKnowledges_.erase( itObjectKnowledge );
}


// -----------------------------------------------------------------------------
// Name: MOS_Team::ChangeDiplomaty
/** @param  team 
    @param  nRelationship 
    */
// Created: APE 2004-10-29
// -----------------------------------------------------------------------------
void MOS_Team::ChangeDiplomaty( MOS_Team& team, ASN1T_EnumDiplomatie nRelationship )
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
