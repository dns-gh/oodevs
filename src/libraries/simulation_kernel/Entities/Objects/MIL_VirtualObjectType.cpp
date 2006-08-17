// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Objects/MIL_VirtualObjectType.cpp $
// $Author: Nld $
// $Modtime: 26/10/04 18:25 $
// $Revision: 1 $
// $Workfile: MIL_VirtualObjectType.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_VirtualObjectType.h"

#include "MIL_VirtualObjectType_ControlZone.h"

      MIL_VirtualObjectType::T_ObjectTypeMap MIL_VirtualObjectType::objectTypes_;
const MIL_VirtualObjectType_ControlZone&     MIL_VirtualObjectType::controlZone_ = *new MIL_VirtualObjectType_ControlZone( "controle de zone" );

// =============================================================================
// MANAGER
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_VirtualObjectType::Initialize
// Created: NLD 2004-08-09
// -----------------------------------------------------------------------------
void MIL_VirtualObjectType::Initialize( MIL_InputArchive& archive )
{
    MT_LOG_INFO_MSG( "Initializing virtual object types" );

    objectTypes_[ controlZone_.GetName() ] = &controlZone_;

    archive.Section  ( "Objets" );
    archive.BeginList( "ObjetsVirtuels" );

    while( archive.NextListElement() )
    {
        archive.Section( "Objet" );

        std::string strType;
        archive.ReadAttribute( "type", strType );

        const MIL_VirtualObjectType* pType = FindObjectType( strType );
        if( !pType )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unknown object type", archive.GetContext() );

        const_cast< MIL_VirtualObjectType* >( pType )->Read( archive );
        
        archive.EndSection(); // Objet
    }

    // Post check
    for( CIT_ObjectTypeMap itType = objectTypes_.begin(); itType != objectTypes_.end(); ++itType )
        if( !itType->second->IsInitialized() )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "Object type '%s' not initialized", itType->second->GetName().c_str() ), archive.GetContext() );

    archive.EndList   (); // ObjetsVirtuels
    archive.EndSection(); // Objets
}

// -----------------------------------------------------------------------------
// Name: MIL_VirtualObjectType::Terminate
// Created: NLD 2004-08-09
// -----------------------------------------------------------------------------
void MIL_VirtualObjectType::Terminate()
{
    objectTypes_.clear();
}

// =============================================================================
// INIT
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_VirtualObjectType constructor
// Created: NLD 2004-08-09
// -----------------------------------------------------------------------------
MIL_VirtualObjectType::MIL_VirtualObjectType( const std::string strName )
    : strName_     ( strName )
    , bInitialized_( false )
{
}

// -----------------------------------------------------------------------------
// Name: MIL_VirtualObjectType destructor
// Created: NLD 2004-08-09
// -----------------------------------------------------------------------------
MIL_VirtualObjectType::~MIL_VirtualObjectType()
{

}

