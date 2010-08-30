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
#include "protocol/protocol.h"

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
bool MIL_NullParameter::ToPointList( Common::MsgPointList& /*asn*/ ) const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_NullParameter::ToPolygonList
// Created: LDC 2009-05-25
// -----------------------------------------------------------------------------
bool MIL_NullParameter::ToPolygonList( Common::MsgPolygonList& /*asn*/ ) const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_NullParameter::ToAgentKnowledgeList
// Created: LDC 2009-05-25
// -----------------------------------------------------------------------------
bool MIL_NullParameter::ToAgentKnowledgeList( Common::UnitKnowledgeIdList& /*asn*/ ) const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_NullParameter::ToAgentList
// Created: LDC 2009-05-25
// -----------------------------------------------------------------------------
bool MIL_NullParameter::ToAgentList( Common::UnitIdList& /*asn*/ ) const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_NullParameter::ToAutomatList
// Created: LDC 2009-05-25
// -----------------------------------------------------------------------------
bool MIL_NullParameter::ToAutomatList( Common::AutomatIdList& /*asn*/ ) const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_NullParameter::ToGenObjectList
// Created: LDC 2009-05-25
// -----------------------------------------------------------------------------
bool MIL_NullParameter::ToGenObjectList( Common::MsgPlannedWorkList& /*asn*/ ) const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_NullParameter::ToLocationList
// Created: LDC 2009-05-25
// -----------------------------------------------------------------------------
bool MIL_NullParameter::ToLocationList( Common::MsgLocationList& /*asn*/ ) const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_NullParameter::ToObjectiveList
// Created: LDC 2009-05-25
// -----------------------------------------------------------------------------
bool MIL_NullParameter::ToObjectiveList( Common::MsgMissionObjectiveList& /*asn*/ ) const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_NullParameter::ToObjectKnowledgeList
// Created: LDC 2009-05-25
// -----------------------------------------------------------------------------
bool MIL_NullParameter::ToObjectKnowledgeList( Common::ObjectKnowledgeIdList& /*asn*/ ) const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_NullParameter::ToPathList
// Created: LDC 2009-05-25
// -----------------------------------------------------------------------------
bool MIL_NullParameter::ToPathList( Common::MsgPathList& /*asn*/ ) const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_NullParameter::ToNatureAtlas
// Created: LDC 2009-11-03
// -----------------------------------------------------------------------------
bool MIL_NullParameter::ToNatureAtlas( Common::MsgAtlasNature& asn ) const
{
    return NET_ASN_Tools::CopyNatureAtlas( 0, asn );
}