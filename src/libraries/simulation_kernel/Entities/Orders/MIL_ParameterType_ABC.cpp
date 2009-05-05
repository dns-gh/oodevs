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

#include "MIL_ParameterType_Bool.h"
#include "MIL_ParameterType_Enumeration.h"
#include "MIL_ParameterType_Point.h"
#include "MIL_ParameterType_PointList.h"
#include "MIL_ParameterType_Polygon.h"
#include "MIL_ParameterType_PolygonList.h"
#include "MIL_ParameterType_Location.h"
#include "MIL_ParameterType_LocationList.h"
#include "MIL_ParameterType_Path.h"
#include "MIL_ParameterType_PathList.h"
#include "MIL_ParameterType_Direction.h"
#include "MIL_ParameterType_NatureAtlas.h"
#include "MIL_ParameterType_Automat.h"
#include "MIL_ParameterType_AutomatList.h"
#include "MIL_ParameterType_Agent.h"
#include "MIL_ParameterType_AgentList.h"
#include "MIL_ParameterType_AgentKnowledge.h"
#include "MIL_ParameterType_AgentKnowledgeList.h"
#include "MIL_ParameterType_ObjectKnowledge.h"
#include "MIL_ParameterType_ObjectKnowledgeList.h"
#include "MIL_ParameterType_PopulationKnowledge.h"
#include "MIL_ParameterType_DotationType.h"
#include "MIL_ParameterType_EquipmentType.h"
#include "MIL_ParameterType_GDH.h"
#include "MIL_ParameterType_Numeric.h"
#include "MIL_ParameterType_GenObject.h"
#include "MIL_ParameterType_GenObjectList.h"
#include "MIL_ParameterType_MaintenancePriorities.h"
#include "MIL_ParameterType_MedicalPriorities.h"
#include "MIL_ParameterType_IndirectFire.h"
#include "MIL_ParameterType_String.h"
#include "MIL_ParameterType_ObjectiveList.h"
#include "MIL_OrderType_ABC.h"
#include "Decision/DEC_Tools.h"

// =============================================================================
// FACTORY
// =============================================================================

MIL_ParameterType_ABC::T_ParameterMap MIL_ParameterType_ABC::parameters_;

// -----------------------------------------------------------------------------
// Name: template< typename T > void MIL_ParameterType_ABC::RegisterParameterType
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
// Name: MIL_ParameterType_ABC::Initialize
// Created: NLD 2006-12-05
// -----------------------------------------------------------------------------
void MIL_ParameterType_ABC::Initialize()
{
    RegisterParameterType< MIL_ParameterType_Bool                  >();
    RegisterParameterType< MIL_ParameterType_Enumeration           >();
    RegisterParameterType< MIL_ParameterType_Point                 >();
    RegisterParameterType< MIL_ParameterType_PointList             >();
    RegisterParameterType< MIL_ParameterType_Polygon               >();
    RegisterParameterType< MIL_ParameterType_PolygonList           >();
    RegisterParameterType< MIL_ParameterType_Location              >();
    RegisterParameterType< MIL_ParameterType_LocationList          >();
    RegisterParameterType< MIL_ParameterType_Path                  >();
    RegisterParameterType< MIL_ParameterType_PathList              >();
    RegisterParameterType< MIL_ParameterType_Direction             >();
    RegisterParameterType< MIL_ParameterType_NatureAtlas           >();
    RegisterParameterType< MIL_ParameterType_Automat               >();
    RegisterParameterType< MIL_ParameterType_AutomatList           >();
    RegisterParameterType< MIL_ParameterType_Agent                 >();
    RegisterParameterType< MIL_ParameterType_AgentList             >();
    RegisterParameterType< MIL_ParameterType_AgentKnowledge        >();
    RegisterParameterType< MIL_ParameterType_AgentKnowledgeList    >();
    RegisterParameterType< MIL_ParameterType_ObjectKnowledge       >();
    RegisterParameterType< MIL_ParameterType_ObjectKnowledgeList   >();
    RegisterParameterType< MIL_ParameterType_PopulationKnowledge   >();
    RegisterParameterType< MIL_ParameterType_DotationType          >();
    RegisterParameterType< MIL_ParameterType_EquipmentType         >();
    RegisterParameterType< MIL_ParameterType_GDH                   >();
    RegisterParameterType< MIL_ParameterType_Numeric               >();
    RegisterParameterType< MIL_ParameterType_GenObject             >();
    RegisterParameterType< MIL_ParameterType_GenObjectList         >();
    RegisterParameterType< MIL_ParameterType_MaintenancePriorities >();
    RegisterParameterType< MIL_ParameterType_MedicalPriorities     >();
    RegisterParameterType< MIL_ParameterType_IndirectFire          >();
    RegisterParameterType< MIL_ParameterType_String                >();
    RegisterParameterType< MIL_ParameterType_ObjectiveList         >();
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
}
   
//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_ABC destructor
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
MIL_ParameterType_ABC::~MIL_ParameterType_ABC()
{
    // NOTHING
}

