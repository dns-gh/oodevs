// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_ParameterType_ABC.h"
#include "MT_Tools/MT_FormatString.h"
#include "MT_Tools/MT_ScipioException.h"

// =============================================================================
// FACTORY
// =============================================================================

MIL_ParameterType_ABC::T_ParameterMap MIL_ParameterType_ABC::parameters_;

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_ABC::RegisterParameterType
// Created: MGD 2010-11-17
// -----------------------------------------------------------------------------
void MIL_ParameterType_ABC::RegisterParameterType( const std::string& name, E_Type type )
{
    const MIL_ParameterType_ABC* pParameter = new MIL_ParameterType_ABC( name, type );
    const MIL_ParameterType_ABC*& pTmp = parameters_[ pParameter->GetName() ];
    if( pTmp )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "Parameter type %s already defined", pParameter->GetName() ) );
    pTmp = pParameter;
}

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_ABC::Initialize
// Created: NLD 2006-12-05
// -----------------------------------------------------------------------------
void MIL_ParameterType_ABC::Initialize()
{
    RegisterParameterType( std::string("bool"), eBool );
    RegisterParameterType( std::string("Boolean"), eBool );
    RegisterParameterType( std::string("Enumeration"), eEnumeration );
    RegisterParameterType( std::string("Point"), ePoint );
    RegisterParameterType( std::string("Polygon"), ePolygon );
    RegisterParameterType( std::string("Location" ), eLocation );
    RegisterParameterType( std::string("Path"), ePath );
    RegisterParameterType( std::string("Direction"), eDirection );
    RegisterParameterType( std::string("DirectionBM"), eDirection );
    RegisterParameterType( std::string("Heading"), eDirection );
    RegisterParameterType( std::string("NatureAtlas"), eNatureAtlas );
    RegisterParameterType( std::string("Automate"), eAutomat );
    RegisterParameterType( std::string("Automat"), eAutomat );
    RegisterParameterType( std::string("Agent"), eAgent );
    RegisterParameterType( std::string("AgentKnowledge"), eAgentKnowledge );
    RegisterParameterType( std::string("ObjectKnowledge"), eObjectKnowledge );
    RegisterParameterType( std::string("PopulationKnowledge"), eCrowdKnowledge );
    RegisterParameterType( std::string("CrowdKnowledge"), eCrowdKnowledge );
    RegisterParameterType( std::string("DotationType"), eResourceType );
    RegisterParameterType( std::string("ResourceType"), eResourceType );
    RegisterParameterType( std::string("EquipmentType"), eEquipmentType );
    RegisterParameterType( std::string("DateTime"), eDateTime );
    RegisterParameterType( std::string("Integer"), eInteger );
    RegisterParameterType( std::string("Numeric"), eNumeric );
    RegisterParameterType( std::string("GenObject"), ePlannedWork );
    RegisterParameterType( std::string("PlannedWork"), ePlannedWork );
    RegisterParameterType( std::string("MaintenancePriorities"), eMaintenancePriorities );
    RegisterParameterType( std::string("MedicalPriorities"), eMedicalPriorities );
    RegisterParameterType( std::string("IndirectFire"), eIndirectFire );
    RegisterParameterType( std::string("String"), eString );;
    RegisterParameterType( std::string("UrbanBlock"), eUrbanKnowledge );
    RegisterParameterType( std::string("UrbanKnowledge"), eUrbanKnowledge );
    RegisterParameterType( std::string("LocationComposite"), eLocationComposite );
    RegisterParameterType( std::string("PhaseLine"), ePhaseLine );
    RegisterParameterType( std::string("ResourceNetwork"), eResourceNetwork );
}

// =============================================================================
// INSTANCE
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_ABC constructor
// Created: NLD 2006-12-05
// -----------------------------------------------------------------------------
MIL_ParameterType_ABC::MIL_ParameterType_ABC( const std::string& strName, E_Type type  )
    : strName_( strName )
    , type_( type )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_ABC destructor
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
MIL_ParameterType_ABC::~MIL_ParameterType_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_ABC::Find
// Created: LDC 2010-09-14
// -----------------------------------------------------------------------------
const MIL_ParameterType_ABC* MIL_ParameterType_ABC::Find( const std::string& strName )
{
    CIT_ParameterMap it = parameters_.find( strName );
    if( it == parameters_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_ABC::GetName
// Created: NLD 2006-12-05
// -----------------------------------------------------------------------------
const std::string& MIL_ParameterType_ABC::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_ABC::GetType
// Created: MGD 2010-11-18
// -----------------------------------------------------------------------------
MIL_ParameterType_ABC::E_Type MIL_ParameterType_ABC::GetType() const
{
    return type_;
}

