// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_IntelligenceOrder.h"
#include "MIL_Fuseau.h"
#include "simulation_terrain/TER_Localisation.h"
#include "game_asn/generated/Intelligence.h"
#include "Tools/MIL_Tools.h"

// -----------------------------------------------------------------------------
// Name: MIL_IntelligenceOrder constructor
// Created: SBO 2007-10-30
// -----------------------------------------------------------------------------
MIL_IntelligenceOrder::MIL_IntelligenceOrder( const ASN1T_Intelligence& asn )
    : name_     ( asn.name )
    , nature_   ( asn.nature )
    , level_    ( asn.level )
    , embarked_ ( asn.embarked ? true : false )
    , position_ ( asn.location )
    , diplomacy_( asn.diplomacy )
    , formation_( asn.formation )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_IntelligenceOrder destructor
// Created: SBO 2007-10-30
// -----------------------------------------------------------------------------
MIL_IntelligenceOrder::~MIL_IntelligenceOrder()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_IntelligenceOrder::Serialize
// Created: SBO 2007-10-30
// -----------------------------------------------------------------------------
void MIL_IntelligenceOrder::Serialize( ASN1T_Intelligence& asn ) const
{
    asn.name      = name_.c_str();
    asn.nature    = nature_.c_str();
    asn.level     = level_;
    asn.embarked  = embarked_ ? 1 : 0;
    asn.location  = position_;
    asn.diplomacy = diplomacy_;
    asn.formation = formation_;
}

// -----------------------------------------------------------------------------
// Name: MIL_IntelligenceOrder::IsInside
// Created: SBO 2007-11-13
// -----------------------------------------------------------------------------
bool MIL_IntelligenceOrder::IsInside( const MIL_Fuseau& fuseau ) const
{
    MT_Vector2D pos;
    MIL_Tools::ConvertCoordMosToSim( position_, pos );
    return fuseau.IsInside( pos );
}

// -----------------------------------------------------------------------------
// Name: MIL_IntelligenceOrder::IsInside
// Created: SBO 2007-11-13
// -----------------------------------------------------------------------------
bool MIL_IntelligenceOrder::IsInside( const TER_Localisation& location ) const
{
    MT_Vector2D pos;
    MIL_Tools::ConvertCoordMosToSim( position_, pos );
    return location.IsInside( pos );
}

// -----------------------------------------------------------------------------
// Name: MIL_IntelligenceOrder::IsEmbarked
// Created: SBO 2007-11-13
// -----------------------------------------------------------------------------
bool MIL_IntelligenceOrder::IsEmbarked() const
{
    return embarked_;
}

// -----------------------------------------------------------------------------
// Name: MIL_IntelligenceOrder::GetLevel
// Created: SBO 2007-11-13
// -----------------------------------------------------------------------------
ASN1T_EnumNatureLevel MIL_IntelligenceOrder::GetLevel() const
{
    return level_;
}
