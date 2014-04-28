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
        throw MASA_EXCEPTION( "Parameter type " + pParameter->GetName() + " already defined" );
    pTmp = pParameter;
}

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_ABC::Initialize
// Created: NLD 2006-12-05
// -----------------------------------------------------------------------------
void MIL_ParameterType_ABC::Initialize()
{
    RegisterParameterType( "bool", eBool );
    RegisterParameterType( "Boolean", eBool );
    RegisterParameterType( "Enumeration", eEnumeration );
    RegisterParameterType( "Point", ePoint );
    RegisterParameterType( "Polygon", ePolygon );
    RegisterParameterType( "Location" , eLocation );
    RegisterParameterType( "Path", ePath );
    RegisterParameterType( "Direction", eDirection );
    RegisterParameterType( "DirectionBM", eDirection );
    RegisterParameterType( "Heading", eDirection );
    RegisterParameterType( "NatureAtlas", eNatureAtlas );
    RegisterParameterType( "Automate", eAutomat );
    RegisterParameterType( "Automat", eAutomat );
    RegisterParameterType( "Agent", eAgent );
    RegisterParameterType( "AgentKnowledge", eAgentKnowledge );
    RegisterParameterType( "ObjectKnowledge", eObjectKnowledge );
    RegisterParameterType( "PopulationKnowledge", eCrowdKnowledge );
    RegisterParameterType( "CrowdKnowledge", eCrowdKnowledge );
    RegisterParameterType( "DotationType", eResourceType );
    RegisterParameterType( "ResourceType", eResourceType );
    RegisterParameterType( "EquipmentType", eEquipmentType );
    RegisterParameterType( "DateTime", eDateTime );
    RegisterParameterType( "Integer", eInteger );
    RegisterParameterType( "Numeric", eNumeric );
    RegisterParameterType( "GenObject", ePlannedWork );
    RegisterParameterType( "PlannedWork", ePlannedWork );
    RegisterParameterType( "MaintenancePriorities", eMaintenancePriorities );
    RegisterParameterType( "MedicalPriorities", eMedicalPriorities );
    RegisterParameterType( "IndirectFire", eIndirectFire );
    RegisterParameterType( "String", eString );;
    RegisterParameterType( "UrbanBlock", eUrbanKnowledge );
    RegisterParameterType( "UrbanKnowledge", eUrbanKnowledge );
    RegisterParameterType( "LocationComposite", eLocationComposite );
    RegisterParameterType( "PhaseLine", ePhaseLine );
    RegisterParameterType( "ResourceNetworkNode", eResourceNetworkNode );
    RegisterParameterType( "ResourceNetworkType", eResourceNetworkType );
    RegisterParameterType( "Stage", eStage );
    RegisterParameterType( "LimaFunction", eLimaFunction );
}

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
