// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_orders_pch.h"
#include "MIL_ParameterType_ABC.h"
#include "MIL_ParameterType_Agent.h"
#include "MIL_ParameterType_AgentKnowledge.h"
#include "MIL_ParameterType_Automat.h"
#include "MIL_ParameterType_Bool.h"
#include "MIL_ParameterType_Direction.h"
#include "MIL_ParameterType_DotationType.h"
#include "MIL_ParameterType_Enumeration.h"
#include "MIL_ParameterType_EquipmentType.h"
#include "MIL_ParameterType_GDH.h"
#include "MIL_ParameterType_GenObject.h"
#include "MIL_ParameterType_IndirectFire.h"
#include "MIL_ParameterType_Lima.h"
#include "MIL_ParameterType_LimaList.h"
#include "MIL_ParameterType_Location.h"
#include "MIL_ParameterType_LocationComposite.h"
#include "MIL_ParameterType_MaintenancePriorities.h"
#include "MIL_ParameterType_MedicalPriorities.h"
#include "MIL_ParameterType_NatureAtlas.h"
#include "MIL_ParameterType_Numeric.h"
#include "MIL_ParameterType_ObjectKnowledge.h"
#include "MIL_ParameterType_Path.h"
#include "MIL_ParameterType_Point.h"
#include "MIL_ParameterType_Polygon.h"
#include "MIL_ParameterType_PopulationKnowledge.h"
#include "MIL_ParameterType_String.h"
#include "MIL_ParameterType_UrbanBlock.h"
#include "MT_Tools/MT_FormatString.h"

// =============================================================================
// FACTORY
// =============================================================================

MIL_ParameterType_ABC::T_ParameterMap MIL_ParameterType_ABC::parameters_;

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_ABC::RegisterParameterType
// Created: NLD 2006-12-05
// -----------------------------------------------------------------------------
template< typename T > void MIL_ParameterType_ABC::RegisterParameterType()
{
    const MIL_ParameterType_ABC* pParameter = new T();
    const MIL_ParameterType_ABC*& pTmp = parameters_[ pParameter->GetName() ];
    if( pTmp )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "Parameter type %s already defined", pParameter->GetName() ) );
    pTmp = pParameter;
}

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_ABC::RegisterParameterType
// Created: MGD 2009-10-21
// -----------------------------------------------------------------------------
template< typename T > void MIL_ParameterType_ABC::RegisterParameterType( const std::string& name)
{
    const MIL_ParameterType_ABC* pParameter = new T( name );
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
    RegisterParameterType< MIL_ParameterType_Bool                  >();
    RegisterParameterType< MIL_ParameterType_Enumeration           >();
    RegisterParameterType< MIL_ParameterType_Point                 >( std::string("Point") );
    RegisterParameterType< MIL_ParameterType_Polygon               >( std::string("Polygon") );
    RegisterParameterType< MIL_ParameterType_Location              >();
    RegisterParameterType< MIL_ParameterType_Path                  >( std::string("Path") );
    RegisterParameterType< MIL_ParameterType_Direction             >( std::string("Direction") );
    RegisterParameterType< MIL_ParameterType_Direction             >( std::string("DirectionBM") );
    RegisterParameterType< MIL_ParameterType_Direction             >( std::string("Heading") );
    RegisterParameterType< MIL_ParameterType_NatureAtlas           >();
    RegisterParameterType< MIL_ParameterType_Automat               >( std::string("Automate") );
    RegisterParameterType< MIL_ParameterType_Agent                 >( std::string("Agent") );
    RegisterParameterType< MIL_ParameterType_AgentKnowledge        >( std::string("AgentKnowledge") );
    RegisterParameterType< MIL_ParameterType_ObjectKnowledge       >( std::string("ObjectKnowledge") );
    RegisterParameterType< MIL_ParameterType_PopulationKnowledge   >( std::string("PopulationKnowledge") );
    RegisterParameterType< MIL_ParameterType_DotationType          >();
    RegisterParameterType< MIL_ParameterType_EquipmentType         >();
    RegisterParameterType< MIL_ParameterType_GDH                   >();
    RegisterParameterType< MIL_ParameterType_Numeric               >();
    RegisterParameterType< MIL_ParameterType_GenObject             >( std::string("GenObject") );
    RegisterParameterType< MIL_ParameterType_MaintenancePriorities >();
    RegisterParameterType< MIL_ParameterType_MedicalPriorities     >();
    RegisterParameterType< MIL_ParameterType_IndirectFire          >();
    RegisterParameterType< MIL_ParameterType_String                >();
    RegisterParameterType< MIL_ParameterType_UrbanBlock            >( std::string("UrbanBlock") );
    RegisterParameterType< MIL_ParameterType_LocationComposite     >( std::string("LocationComposite") );
    RegisterParameterType< MIL_ParameterType_Lima                  >();
    RegisterParameterType< MIL_ParameterType_LimaList              >();
}

// =============================================================================
// INSTANCE
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_ABC constructor
// Created: NLD 2006-12-05
// -----------------------------------------------------------------------------
MIL_ParameterType_ABC::MIL_ParameterType_ABC( const std::string& strName )
    : strName_( strName )
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
