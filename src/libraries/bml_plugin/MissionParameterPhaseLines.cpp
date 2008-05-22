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

using namespace bml;

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
void MissionParameterPhaseLines::Serialize( ASN1T_MissionParameter& parameter ) const
{
    parameter.null_value = 0;
    parameter.value.t = T_MissionParameter_value_limasOrder;
    parameter.value.u.limasOrder = new ASN1T_LimasOrder();
    parameter.value.u.limasOrder->n = phaseLines_.size();
    if( parameter.value.u.limasOrder->n > 0 )
        parameter.value.u.limasOrder->elem = new ASN1T_LimaOrder[parameter.value.u.limasOrder->n];
    unsigned int i = 0;
    for( T_PhaseLines::const_iterator it = phaseLines_.begin(); it != phaseLines_.end(); ++it, ++i )
        (*it)->Serialize( parameter.value.u.limasOrder->elem[i] );
}

// -----------------------------------------------------------------------------
// Name: MissionParameterPhaseLines::Clean
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
void MissionParameterPhaseLines::Clean( ASN1T_MissionParameter& parameter ) const
{
    unsigned int i = 0;
    for( T_PhaseLines::const_iterator it = phaseLines_.begin(); it != phaseLines_.end(); ++it, ++i )
        (*it)->Clean( parameter.value.u.limasOrder->elem[i] );
    delete[] parameter.value.u.limasOrder->elem;
    delete parameter.value.u.limasOrder;
}

// -----------------------------------------------------------------------------
// Name: MissionParameterPhaseLines::AddLine
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
void MissionParameterPhaseLines::AddLine( MissionParameterPhaseLine& line )
{
    phaseLines_.push_back( &line );
}
