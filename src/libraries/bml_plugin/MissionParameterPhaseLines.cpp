// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "bml_plugin_pch.h"
#include "MissionParameterPhaseLines.h"
#include "MissionParameterPhaseLine.h"

using namespace plugins::bml;

// -----------------------------------------------------------------------------
// Name: MissionParameterPhaseLines constructor
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
MissionParameterPhaseLines::MissionParameterPhaseLines( const kernel::OrderParameter& type )
    : MissionParameter_ABC( type )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameterPhaseLines destructor
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
MissionParameterPhaseLines::~MissionParameterPhaseLines()
{
    for( T_PhaseLines::iterator it = phaseLines_.begin(); it != phaseLines_.end(); ++it )
        delete *it;
}

// -----------------------------------------------------------------------------
// Name: MissionParameterPhaseLines::Serialize
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
void MissionParameterPhaseLines::Serialize( sword::MissionParameter& parameter ) const
{
    parameter.set_null_value( false );
    unsigned int i = 0;
    for( T_PhaseLines::const_iterator it = phaseLines_.begin(); it != phaseLines_.end(); ++it, ++i )
        (*it)->Serialize( *parameter.mutable_value()->Add()->mutable_phaseline()->mutable_elem( i ) );
}

// -----------------------------------------------------------------------------
// Name: MissionParameterPhaseLines::AddLine
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
void MissionParameterPhaseLines::AddLine( MissionParameterPhaseLine& line )
{
    phaseLines_.push_back( &line );
}
