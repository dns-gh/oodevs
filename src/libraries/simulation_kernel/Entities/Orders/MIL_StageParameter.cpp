// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_StageParameter.h"
#include "protocol/Protocol.h"

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_StageParameter )

// -----------------------------------------------------------------------------
// Name: MIL_StageParameter constructor
// Created: ABR 2012-12-18
// -----------------------------------------------------------------------------
MIL_StageParameter::MIL_StageParameter()
    : MIL_StringParameter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_StageParameter constructor
// Created: ABR 2012-12-18
// -----------------------------------------------------------------------------
MIL_StageParameter::MIL_StageParameter( const std::string& message )
    : MIL_StringParameter( message )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_StageParameter destructor
// Created: ABR 2012-12-18
// -----------------------------------------------------------------------------
MIL_StageParameter::~MIL_StageParameter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_StageParameter::IsOfType
// Created: ABR 2012-12-18
// -----------------------------------------------------------------------------
bool MIL_StageParameter::IsOfType( MIL_ParameterType_ABC::E_Type type ) const
{
    return type == MIL_ParameterType_ABC::eStage;
}

// -----------------------------------------------------------------------------
// Name: MIL_StageParameter::ToElement
// Created: ABR 2013-02-20
// -----------------------------------------------------------------------------
bool MIL_StageParameter::ToElement( sword::MissionParameter_Value& elem ) const
{
    elem.set_stage( value_ );
    return true;
}
