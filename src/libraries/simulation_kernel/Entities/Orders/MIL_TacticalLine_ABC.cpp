// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_TacticalLine_ABC.h"

#include "Tools/MIL_IDManager.h"
#include "Tools/MIL_Tools.h"
#include "Network/NET_ASN_Tools.h"
#include "Network/NET_AsnException.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/MIL_Formation.h"
#include "Entities/MIL_EntityManager.h"
#include "MIL_AgentServer.h"

// -----------------------------------------------------------------------------
// Name: MIL_TacticalLine_ABC constructor
// Created: NLD 2006-11-16
// -----------------------------------------------------------------------------
MIL_TacticalLine_ABC::MIL_TacticalLine_ABC()
    : nID_                  ( 0 )
    , strName_              ()
    , pFormationBroadcasted_( 0 )
    , pAutomateBroadcasted_ ( 0 )
    , line_                 ()
{
}

// -----------------------------------------------------------------------------
// Name: MIL_TacticalLine_ABC constructor
// Created: NLD 2006-11-13
// -----------------------------------------------------------------------------
MIL_TacticalLine_ABC::MIL_TacticalLine_ABC( const ASN1T_TacticalLine& asn )
    : nID_                  ( MIL_IDManager::limits_.GetFreeSimID() )
    , strName_              ( asn.nom )
    , pFormationBroadcasted_( asn.diffusion.t == T_TacticalLinesDiffusion_formation ? MIL_AgentServer::GetWorkspace().GetEntityManager().FindFormation( asn.diffusion.u.formation ) : 0 )
    , pAutomateBroadcasted_ ( asn.diffusion.t == T_TacticalLinesDiffusion_automat   ? MIL_AgentServer::GetWorkspace().GetEntityManager().FindAutomate ( asn.diffusion.u.automat  ) : 0 )
    , line_                 ()
{
    if( !pFormationBroadcasted_ && !pAutomateBroadcasted_ )
        throw NET_AsnException< ASN1T_EnumInfoContextErrorCode >( EnumInfoContextErrorCode::error_invalid_diffusion );

    if( !NET_ASN_Tools::ReadLine( asn.geometrie, line_ ) )
        throw NET_AsnException< ASN1T_EnumInfoContextErrorCode >( EnumInfoContextErrorCode::error_invalid_geometry );
}

// -----------------------------------------------------------------------------
// Name: MIL_TacticalLine_ABC constructµor
// Created: NLD 2006-11-17
// -----------------------------------------------------------------------------
MIL_TacticalLine_ABC::MIL_TacticalLine_ABC( const MIL_Automate& automateBroadcasted, MIL_InputArchive& archive )
    : nID_                  ( MIL_IDManager::limits_.GetFreeSimID() )
    , strName_              ()
    , pFormationBroadcasted_( 0 )
    , pAutomateBroadcasted_ ( &automateBroadcasted )
    , line_                 ()
{
    Initialize( archive );
}

// -----------------------------------------------------------------------------
// Name: MIL_TacticalLine_ABC constructor
// Created: NLD 2006-11-17
// -----------------------------------------------------------------------------
MIL_TacticalLine_ABC::MIL_TacticalLine_ABC( const MIL_Formation& formationBroadcasted, MIL_InputArchive& archive )
    : nID_                  ( MIL_IDManager::limits_.GetFreeSimID() )
    , strName_              ()
    , pFormationBroadcasted_( &formationBroadcasted )
    , pAutomateBroadcasted_ ( 0 )
    , line_                 ()
{
    Initialize( archive );
}

// -----------------------------------------------------------------------------
// Name: MIL_TacticalLine_ABC destructor
// Created: NLD 2006-11-13
// -----------------------------------------------------------------------------
MIL_TacticalLine_ABC::~MIL_TacticalLine_ABC()
{
}

// -----------------------------------------------------------------------------
// Name: MIL_TacticalLine_ABC::Initialize
// Created: NLD 2006-11-17
// -----------------------------------------------------------------------------
void MIL_TacticalLine_ABC::Initialize( MIL_InputArchive& archive )
{
    archive.ReadAttribute( "name", strName_ );
    T_PointVector points;
    while( archive.NextListElement() )
    {
        archive.Section( "point" );
        std::string strPoint;
        archive.Read( strPoint );
        MT_Vector2D vPosTmp;
        MIL_Tools::ConvertCoordMosToSim( strPoint, vPosTmp );
        points.push_back( vPosTmp );
        archive.EndSection(); // point
    }
    if( points.size() < 2 )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Invalid tactical line geometry" );
    line_.Reset( TER_Localisation::eLine, points );
}

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_TacticalLine_ABC::load
// Created: NLD 2006-11-16
// -----------------------------------------------------------------------------
void MIL_TacticalLine_ABC::load( MIL_CheckPointInArchive& file, const uint )
{
    file >> const_cast< uint& >( nID_ )
         >> strName_
         >> const_cast< MIL_Formation*& >( pFormationBroadcasted_ )
         >> const_cast< MIL_Automate*& >( pAutomateBroadcasted_ )
         >> line_;
}

// -----------------------------------------------------------------------------
// Name: MIL_TacticalLine_ABC::save
// Created: NLD 2006-11-16
// -----------------------------------------------------------------------------
void MIL_TacticalLine_ABC::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    file << nID_
         << strName_
         << pFormationBroadcasted_
         << pAutomateBroadcasted_
         << line_;
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_TacticalLine_ABC::Update
// Created: NLD 2006-11-13
// -----------------------------------------------------------------------------
void MIL_TacticalLine_ABC::Update( const ASN1T_TacticalLine& asn )
{
    pAutomateBroadcasted_  = 0;
    pFormationBroadcasted_ = 0;
    if( asn.diffusion.t == T_TacticalLinesDiffusion_formation )
        pFormationBroadcasted_ = MIL_AgentServer::GetWorkspace().GetEntityManager().FindFormation( asn.diffusion.u.formation );
    else if( asn.diffusion.t == T_TacticalLinesDiffusion_automat )
        pAutomateBroadcasted_ = MIL_AgentServer::GetWorkspace().GetEntityManager().FindAutomate( asn.diffusion.u.automat );

    if( !pFormationBroadcasted_ && !pAutomateBroadcasted_ )
        throw NET_AsnException< ASN1T_EnumInfoContextErrorCode >( EnumInfoContextErrorCode::error_invalid_diffusion );

    if( !NET_ASN_Tools::ReadLine( asn.geometrie, line_ ) )
        throw NET_AsnException< ASN1T_EnumInfoContextErrorCode >( EnumInfoContextErrorCode::error_invalid_geometry );
}

// =============================================================================
// TOOLS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_TacticalLine_ABC::Serialize
// Created: NLD 2006-11-17
// -----------------------------------------------------------------------------
void MIL_TacticalLine_ABC::Serialize( ASN1T_TacticalLine& asn ) const
{
    NET_ASN_Tools::WriteLine( line_, asn.geometrie );
    asn.nom = strName_.c_str();
    if( pFormationBroadcasted_ )
    {
        asn.diffusion.t           = T_TacticalLinesDiffusion_formation;
        asn.diffusion.u.formation = pFormationBroadcasted_->GetID();
    }
    else if( pAutomateBroadcasted_ )
    {
        asn.diffusion.t          = T_TacticalLinesDiffusion_automat;
        asn.diffusion.u.automat = pAutomateBroadcasted_->GetID();
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_TacticalLine_ABC::CleanAfterSerialization
// Created: NLD 2006-11-17
// -----------------------------------------------------------------------------
void MIL_TacticalLine_ABC::CleanAfterSerialization( ASN1T_TacticalLine& asn ) const
{
    NET_ASN_Tools::Delete( asn.geometrie );
}

