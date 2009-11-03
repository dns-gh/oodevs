// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_NullParameter.h"
#include "Network/NET_ASN_Tools.h"

// -----------------------------------------------------------------------------
// Name: MIL_NullParameter constructor
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
MIL_NullParameter::MIL_NullParameter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_NullParameter destructor
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
MIL_NullParameter::~MIL_NullParameter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_NullParameter::IsOfType
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
bool MIL_NullParameter::IsOfType( const MIL_ParameterType_ABC& ) const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_NullParameter::ToPointList
// Created: LDC 2009-05-25
// -----------------------------------------------------------------------------
bool MIL_NullParameter::ToPointList( ASN1T_PointList& asn ) const
{
    asn.n = 0;
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_NullParameter::ToPolygonList
// Created: LDC 2009-05-25
// -----------------------------------------------------------------------------
bool MIL_NullParameter::ToPolygonList( ASN1T_PolygonList& asn ) const
{
    asn.n = 0;
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_NullParameter::ToAgentKnowledgeList
// Created: LDC 2009-05-25
// -----------------------------------------------------------------------------
bool MIL_NullParameter::ToAgentKnowledgeList( ASN1T_UnitKnowledgeList& asn ) const
{
    asn.n = 0;
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_NullParameter::ToAgentList
// Created: LDC 2009-05-25
// -----------------------------------------------------------------------------
bool MIL_NullParameter::ToAgentList( ASN1T_UnitList& asn ) const
{
    asn.n = 0;
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_NullParameter::ToAutomatList
// Created: LDC 2009-05-25
// -----------------------------------------------------------------------------
bool MIL_NullParameter::ToAutomatList( ASN1T_AutomatList& asn ) const
{
    asn.n = 0;
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_NullParameter::ToGenObjectList
// Created: LDC 2009-05-25
// -----------------------------------------------------------------------------
bool MIL_NullParameter::ToGenObjectList( ASN1T_PlannedWorkList& asn ) const
{
    asn.n = 0;
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_NullParameter::ToLocationList
// Created: LDC 2009-05-25
// -----------------------------------------------------------------------------
bool MIL_NullParameter::ToLocationList( ASN1T_LocationList& asn ) const
{
    asn.n = 0;
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_NullParameter::ToObjectiveList
// Created: LDC 2009-05-25
// -----------------------------------------------------------------------------
bool MIL_NullParameter::ToObjectiveList( ASN1T_MissionObjectiveList& asn ) const
{
    asn.n = 0;
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_NullParameter::ToObjectKnowledgeList
// Created: LDC 2009-05-25
// -----------------------------------------------------------------------------
bool MIL_NullParameter::ToObjectKnowledgeList( ASN1T_ObjectKnowledgeList& asn ) const
{
    asn.n = 0;
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_NullParameter::ToPathList
// Created: LDC 2009-05-25
// -----------------------------------------------------------------------------
bool MIL_NullParameter::ToPathList( ASN1T_PathList& asn ) const
{
    asn.n = 0;
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_NullParameter::ToNatureAtlas
// Created: LDC 2009-11-03
// -----------------------------------------------------------------------------
bool MIL_NullParameter::ToNatureAtlas( ASN1T_AtlasNature& asn ) const
{
    return NET_ASN_Tools::CopyNatureAtlas( 0, asn );
}