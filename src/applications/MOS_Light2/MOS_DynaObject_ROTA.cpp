// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-09-02 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "MOS_Light2_pch.h"
#include "MOS_DynaObject_ROTA.h"

#include "MOS_App.h"

// -----------------------------------------------------------------------------
// Name: MOS_DynaObject_ROTA constructor
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
MOS_DynaObject_ROTA::MOS_DynaObject_ROTA()
    : MOS_DynaObject_ABC ()
    , nDanger_           ( 0 )
{
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: MOS_DynaObject_ROTA constructor
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
MOS_DynaObject_ROTA::MOS_DynaObject_ROTA( const ASN1T_MsgObjectCreation& asnMsg )
    : MOS_DynaObject_ABC ( asnMsg )
    , nDanger_           ( 0 )
{
    nDanger_ = asnMsg.attributs_specifiques.u.rota->niveau_danger;
    nbcAgents_.clear();
    for( uint i = 0; i < asnMsg.attributs_specifiques.u.rota->agents_nbc.n; ++i )
        nbcAgents_.push_back( asnMsg.attributs_specifiques.u.rota->agents_nbc.elem[i] );
}
// -----------------------------------------------------------------------------
// Name: MOS_DynaObject_ROTA destructor
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
MOS_DynaObject_ROTA::~MOS_DynaObject_ROTA()
{
    nbcAgents_.clear();
}

// -----------------------------------------------------------------------------
// Name: MOS_DynaObject_ROTA::ReadODB
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
void MOS_DynaObject_ROTA::ReadODB( MT_XXmlInputArchive& archive )
{
    archive.Section( "Objet" );
    MOS_DynaObject_ABC::ReadODB( archive );

    archive.ReadField( "Danger", nDanger_ );
    archive.BeginList( "AgentsNBC" );
    const T_MosId_String_Map& nbcMap = MOS_App::GetApp().GetNBCNames();
    while( archive.NextListElement() )
    {
        archive.Section( "AgentNBC" );
        std::string strNBCAgent;
        archive.ReadAttribute( "type", strNBCAgent );
        uint nAgentNbcId = ( uint )-1;
        for( CIT_MosId_String_Map it = nbcMap.begin(); it != nbcMap.end(); ++it )
            if( it->second == strNBCAgent )
            {
                nAgentNbcId = it->first;
                break;
            }
        assert( nAgentNbcId != ( uint )-1 );
        nbcAgents_.push_back( nAgentNbcId );
        archive.EndSection(); // AgentNBC
    }
    archive.EndList(); // AgentsNBC

    archive.EndSection(); // Object
}

// -----------------------------------------------------------------------------
// Name: MOS_DynaObject_ROTA::WriteODB
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
void MOS_DynaObject_ROTA::WriteODB( MT_XXmlOutputArchive& archive ) const
{
    archive.Section( "Objet" );
    MOS_DynaObject_ABC::WriteODB( archive );

    archive.WriteField( "Danger", nDanger_ );
    archive.BeginList( "AgentsNBC", nbcAgents_.size() );
    for( uint i = 0; i < nbcAgents_.size(); ++i )
    {
        archive.Section( "AgentNBC" );
        archive.WriteAttribute( "type", MOS_App::GetApp().GetNBCName( nbcAgents_[ i ] ) );
        archive.EndSection(); // AgentNBC
    }
    archive.EndList(); // AgentsNBC

    archive.EndSection(); // Object
}
