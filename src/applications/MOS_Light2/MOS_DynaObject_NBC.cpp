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
#include "MOS_DynaObject_NBC.h"

#include "MOS_App.h"

// -----------------------------------------------------------------------------
// Name: MOS_DynaObject_NBC constructor
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
MOS_DynaObject_NBC::MOS_DynaObject_NBC()
    : MOS_DynaObject_ABC ()
    , nAgentNbcId_       ( (uint)-1 )
{
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: MOS_DynaObject_NBC constructor
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
MOS_DynaObject_NBC::MOS_DynaObject_NBC( const ASN1T_MsgObjectCreation& asnMsg )
    : MOS_DynaObject_ABC ( asnMsg )
    , nAgentNbcId_       ( 0 )
{
    if( GetType() == EnumObjectType::nuage_nbc )
        nAgentNbcId_ = asnMsg.attributs_specifiques.u.nuage_nbc->agent_nbc;
    else if( GetType() == EnumObjectType::zone_nbc )
        nAgentNbcId_ = asnMsg.attributs_specifiques.u.zone_nbc->agent_nbc;
}
// -----------------------------------------------------------------------------
// Name: MOS_DynaObject_NBC destructor
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
MOS_DynaObject_NBC::~MOS_DynaObject_NBC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MOS_DynaObject_NBC::ReadODB
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
void MOS_DynaObject_NBC::ReadODB( MT_XXmlInputArchive& archive )
{
    archive.Section( "Objet" );
    MOS_DynaObject_ABC::ReadODB( archive );

    archive.Section( "AgentNBC" );
    std::string strNBCAgent;
    archive.ReadAttribute( "type", strNBCAgent );
    nAgentNbcId_ = ( uint )-1;
    const T_MosId_String_Map& nbcMap = MOS_App::GetApp().GetNBCNames();
    for( CIT_MosId_String_Map it = nbcMap.begin(); it != nbcMap.end(); ++it )
        if( it->second == strNBCAgent )
            nAgentNbcId_ = it->first;
    assert( nAgentNbcId_ != (uint)-1 );
    archive.EndSection(); // AgentNBC

    archive.EndSection(); // Object
}

// -----------------------------------------------------------------------------
// Name: MOS_DynaObject_NBC::WriteODB
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
void MOS_DynaObject_NBC::WriteODB( MT_XXmlOutputArchive& archive ) const
{
    archive.Section( "Objet" );
    MOS_DynaObject_ABC::WriteODB( archive );

    archive.Section( "AgentNBC" );
    archive.WriteAttribute( "type", MOS_App::GetApp().GetNBCName( nAgentNbcId_ ) );
    archive.EndSection(); // AgentNBC

    archive.EndSection(); // Object
}
