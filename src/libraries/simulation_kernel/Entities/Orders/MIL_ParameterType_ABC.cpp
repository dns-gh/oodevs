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
#include "MT_Tools/MT_String.h"
#include <boost/ptr_container/ptr_map.hpp>

namespace
{
    boost::ptr_map< std::string, MIL_ParameterType_ABC, sCaseInsensitiveLess > parameters;
}

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_ABC::RegisterParameterType
// Created: MGD 2010-11-17
// -----------------------------------------------------------------------------
void MIL_ParameterType_ABC::RegisterParameterType( std::string name, E_Type type )
{
    if( !parameters.insert( name, new MIL_ParameterType_ABC( name, type ) ).second )
        throw MASA_EXCEPTION( "Parameter type " + name + " already defined" );
}

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_ABC::Initialize
// Created: NLD 2006-12-05
// -----------------------------------------------------------------------------
void MIL_ParameterType_ABC::Initialize()
{
    RegisterParameterType( "Agent", eAgent );
    RegisterParameterType( "AgentKnowledge", eAgentKnowledge );
    RegisterParameterType( "Automat", eAutomat );
    RegisterParameterType( "Automate", eAutomat );
    RegisterParameterType( "bool", eBool );
    RegisterParameterType( "Boolean", eBool );
    RegisterParameterType( "CrowdKnowledge", eCrowdKnowledge );
    RegisterParameterType( "DateTime", eDateTime );
    RegisterParameterType( "Direction", eDirection );
    RegisterParameterType( "DirectionBM", eDirection );
    RegisterParameterType( "DotationType", eResourceType );
    RegisterParameterType( "Enumeration", eEnumeration );
    RegisterParameterType( "EquipmentType", eEquipmentType );
    RegisterParameterType( "GenObject", ePlannedWork );
    RegisterParameterType( "Heading", eDirection );
    RegisterParameterType( "IndirectFire", eIndirectFire );
    RegisterParameterType( "Integer", eInteger );
    RegisterParameterType( "LimaFunction", eLimaFunction );
    RegisterParameterType( "Location" , eLocation );
    RegisterParameterType( "LocationComposite", eLocationComposite );
    RegisterParameterType( "MaintenancePriorities", eMaintenancePriorities );
    RegisterParameterType( "MedicalPriorities", eMedicalPriorities );
    RegisterParameterType( "NatureAtlas", eNatureAtlas );
    RegisterParameterType( "Numeric", eNumeric );
    RegisterParameterType( "ObjectKnowledge", eObjectKnowledge );
    RegisterParameterType( "Path", ePath );
    RegisterParameterType( "PhaseLine", ePhaseLine );
    RegisterParameterType( "PlannedWork", ePlannedWork );
    RegisterParameterType( "Point", ePoint );
    RegisterParameterType( "Polygon", ePolygon );
    RegisterParameterType( "PopulationKnowledge", eCrowdKnowledge );
    RegisterParameterType( "ResourceNetworkNode", eResourceNetworkNode );
    RegisterParameterType( "ResourceNetworkType", eResourceNetworkType );
    RegisterParameterType( "ResourceType", eResourceType );
    RegisterParameterType( "Stage", eStage );
    RegisterParameterType( "String", eString );;
    RegisterParameterType( "UrbanBlock", eUrbanKnowledge );
    RegisterParameterType( "UrbanKnowledge", eUrbanKnowledge );
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
    auto it = parameters.find( strName );
    if( it == parameters.end() )
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
