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
#include "xeumeuleu/xml.h"

using namespace xml;

      MIL_VirtualObjectType::T_ObjectTypeMap MIL_VirtualObjectType::objectTypes_;
const MIL_VirtualObjectType_ControlZone&     MIL_VirtualObjectType::controlZone_ = *new MIL_VirtualObjectType_ControlZone( "controle de zone" );

// =============================================================================
// MANAGER
// =============================================================================

struct MIL_VirtualObjectType::LoadingWrapper
{
    void ReadObject( xml::xistream& xis )
    {
        MIL_VirtualObjectType::ReadObject( xis );
    }
};

// -----------------------------------------------------------------------------
// Name: MIL_VirtualObjectType::Initialize
// Created: NLD 2004-08-09
// -----------------------------------------------------------------------------
void MIL_VirtualObjectType::Initialize( xml::xistream& xis )
{
    MT_LOG_INFO_MSG( "Initializing virtual object types" );

    objectTypes_[ controlZone_.GetName() ] = &controlZone_;
    LoadingWrapper loader;

    xis >> start( "objects" )
            >> start( "virtual-objects" )
                >> list( "object", loader, &LoadingWrapper::ReadObject )
            >> end()
        >> end();


    // Post check
    for( CIT_ObjectTypeMap itType = objectTypes_.begin(); itType != objectTypes_.end(); ++itType )
        if( !itType->second->IsInitialized() )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "Object type '%s' not initialized", itType->second->GetName().c_str() ) ); // $$$$ ABL 2007-07-19: error context
}

// -----------------------------------------------------------------------------
// Name: MIL_VirtualObjectType::ReadObject
// Created: ABL 2007-07-19
// -----------------------------------------------------------------------------
void MIL_VirtualObjectType::ReadObject( xml::xistream& xis )
{
        std::string strType;
        xis >> attribute( "type", strType );

        const MIL_VirtualObjectType* pType = Find( strType );
        if( !pType )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unknown object type" ); // $$$$ ABL 2007-07-19: error context

        const_cast< MIL_VirtualObjectType* >( pType )->Read( xis );
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

