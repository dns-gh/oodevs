// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_Formation.h"

#include "Entities/Agents/Units/Categories/PHY_NatureLevel.h"
#include "Entities/Automates/MIL_AutomateType.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Orders/MIL_TacticalLineManager.h"
#include "Entities/MIL_Army.h"
#include "Entities/MIL_EntityManager.h"
#include "Network/NET_ASN_Messages.h"
#include "MIL_AgentServer.h"

// -----------------------------------------------------------------------------
// Name: MIL_Formation constructor
// Created: NLD 2006-10-11
// -----------------------------------------------------------------------------
MIL_Formation::MIL_Formation( uint nID, MIL_Army& army, MIL_InputArchive& archive, MIL_Formation* pParent )
    : nID_       ( nID )
    , pArmy_     ( &army )
    , pParent_   ( pParent )
    , pLevel_    ( 0 )
    , strName_   ()
    , formations_()
    , automates_ ()
{
    std::string strLevel;
    archive.ReadAttribute( "name", strName_ );
    archive.ReadAttribute( "level", strLevel );    

    pLevel_ = PHY_NatureLevel::Find( strLevel );
    if( !pLevel_ )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unknown level", archive.GetContext() );

    if( pParent )
        pParent_->RegisterFormation( *this );
    else
        pArmy_->RegisterFormation( *this );

    InitializeSubordinates( archive );   
}

// -----------------------------------------------------------------------------
// Name: MIL_Formation constructor
// Created: NLD 2006-10-11
// -----------------------------------------------------------------------------
MIL_Formation::MIL_Formation()
    : nID_       ( 0 )
    , pArmy_     ( 0 )
    , pParent_   ( 0 )
    , pLevel_    ( 0 )
    , strName_   ()
    , formations_()
    , automates_ ()
{
}

// -----------------------------------------------------------------------------
// Name: MIL_Formation destructor
// Created: NLD 2006-10-11
// -----------------------------------------------------------------------------
MIL_Formation::~MIL_Formation()
{
    assert( pArmy_ );
    if( pParent_ )
        pParent_->UnregisterFormation( *this );
    else
        pArmy_->UnregisterFormation( *this );
}

// -----------------------------------------------------------------------------
// Name: MIL_Formation::InitializeSubordinates
// Created: NLD 2006-10-11
// -----------------------------------------------------------------------------
void MIL_Formation::InitializeSubordinates( MIL_InputArchive& archive )
{
    assert( pArmy_ );
    while( archive.NextListElement() )
    {
        std::string strElement = archive.GetCurrentElementName();

        if( strElement == "formation" )
        {
            archive.BeginList( "formation" );
        
            uint nID;
            archive.ReadAttribute( "id", nID );

            MIL_AgentServer::GetWorkspace().GetEntityManager().CreateFormation( nID, *pArmy_, archive, this ); // Auto-registration
            archive.EndList(); // formation
        }
        else if( strElement == "automat" )
        {
            archive.BeginList( "automat" );

            uint        nID;
            std::string strType;
            
            archive.ReadAttribute( "id"  , nID     );
            archive.ReadAttribute( "type", strType );

            const MIL_AutomateType* pAutomateType = MIL_AutomateType::FindAutomateType( strType );
            if( !pAutomateType )
                throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unknown automat type", archive.GetContext() );

            MIL_AgentServer::GetWorkspace().GetEntityManager().CreateAutomate( *pAutomateType, nID, *this, archive ); // Auto-registration
            archive.EndList(); // automat
        }
        else if( strElement == "limit" )
        {
            archive.BeginList( "limit" );
            MIL_AgentServer::GetWorkspace().GetTacticalLineManager().CreateLimit( *this, archive );
            archive.EndList(); // limit
        }
        else if( strElement == "lima" )
        {
            archive.BeginList( "lima" );
            MIL_AgentServer::GetWorkspace().GetTacticalLineManager().CreateLima( *this, archive );
            archive.EndList(); // lima
        }
    }
}

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_Formation::load
// Created: NLD 2006-10-18
// -----------------------------------------------------------------------------
void MIL_Formation::load( MIL_CheckPointInArchive& file, const uint )
{
   file >> const_cast< uint& >( nID_ )
        >> pArmy_
        >> pParent_;

   uint nLevel;
   file >> nLevel;
   pLevel_ = PHY_NatureLevel::Find( nLevel );
   assert( pLevel_ );

   file >> strName_
        >> formations_
        >> automates_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Formation::save
// Created: NLD 2006-10-18
// -----------------------------------------------------------------------------
void MIL_Formation::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    assert( pLevel_ );
    unsigned level = pLevel_->GetID();
    file << nID_
         << pArmy_
         << pParent_
         << level
         << strName_
         << formations_
         << automates_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Formation::WriteODB
// Created: NLD 2006-10-18
// -----------------------------------------------------------------------------
void MIL_Formation::WriteODB( MT_XXmlOutputArchive& archive ) const
{
    assert( pLevel_ );

    archive.Section( "formation" );
    archive.WriteAttribute( "id", nID_ );
    archive.WriteAttribute( "level", pLevel_->GetName() );
    archive.WriteAttribute( "name", strName_ );

    for( CIT_FormationSet it = formations_.begin(); it != formations_.end(); ++it )
        (**it).WriteODB( archive );

    for( CIT_AutomateSet it = automates_.begin(); it != automates_.end(); ++it )
        (**it).WriteODB( archive );

    archive.EndSection(); // formation
}

// -----------------------------------------------------------------------------
// Name: MIL_Formation::WriteLogisticLinksODB
// Created: NLD 2006-10-19
// -----------------------------------------------------------------------------
void MIL_Formation::WriteLogisticLinksODB( MT_XXmlOutputArchive& archive ) const
{
    for( CIT_FormationSet it = formations_.begin(); it != formations_.end(); ++it )
        (**it).WriteLogisticLinksODB( archive );

    for( CIT_AutomateSet it = automates_.begin(); it != automates_.end(); ++it )
        (**it).WriteLogisticLinksODB( archive );
}

// =============================================================================
// NETWORK
// ============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_Formation::SendCreation
// Created: NLD 2006-10-13
// -----------------------------------------------------------------------------
void MIL_Formation::SendCreation() const
{
    assert( pLevel_ );
    assert( pArmy_ );

    NET_ASN_MsgFormationCreation asn;
    asn().oid      = nID_;
    asn().oid_camp = pArmy_->GetID();
    asn().nom      = strName_.c_str();
    asn().niveau   = pLevel_->GetAsnID();
    if( pParent_ )
    {
        asn().m.oid_formation_parentePresent = 1;
        asn().oid_formation_parente = pParent_->GetID();
    }
    asn.Send();

    for( CIT_FormationSet it = formations_.begin(); it != formations_.end(); ++it )
        (**it).SendCreation();

    for( CIT_AutomateSet it = automates_.begin(); it != automates_.end(); ++it )
        (**it).SendCreation();
}

// -----------------------------------------------------------------------------
// Name: MIL_Formation::SendFullState
// Created: NLD 2006-10-13
// -----------------------------------------------------------------------------
void MIL_Formation::SendFullState() const
{
    for( CIT_FormationSet it = formations_.begin(); it != formations_.end(); ++it )
        (**it).SendFullState();

    for( CIT_AutomateSet it = automates_.begin(); it != automates_.end(); ++it )
        (**it).SendFullState();    
}
