// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_ToxicEffectManipulator.h"
#include "Entities/Agents/Units/Humans/PHY_HumanWound.h"
#include "Entities/Agents/Units/Humans/Human_ABC.h"
#include "MIL_NbcAgentType.h"

// -----------------------------------------------------------------------------
// Name: MIL_ToxicEffectManipulator constructor
// Created: JCR 2008-06-06
// -----------------------------------------------------------------------------
MIL_ToxicEffectManipulator::MIL_ToxicEffectManipulator( const T_NBCAgents& types, double quantity )
    : types_   ( types )
    , quantity_( quantity )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_ToxicEffectManipulator constructor
// Created: JCR 2008-06-06
// -----------------------------------------------------------------------------
MIL_ToxicEffectManipulator::MIL_ToxicEffectManipulator( const MIL_ToxicEffectManipulator& rhs )
    : types_   ( rhs.types_ )
    , quantity_( rhs.quantity_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_ToxicEffectManipulator destructor
// Created: JCR 2008-06-06
// -----------------------------------------------------------------------------
MIL_ToxicEffectManipulator::~MIL_ToxicEffectManipulator()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_ToxicEffectManipulator::GetQuantity
// Created: JCR 2008-06-06
// -----------------------------------------------------------------------------
double MIL_ToxicEffectManipulator::GetQuantity() const
{
    return quantity_;
}

// -----------------------------------------------------------------------------
// Name: MIL_ToxicEffectManipulator::GetType
// Created: JCR 2008-06-06
// -----------------------------------------------------------------------------
const MIL_NbcAgentType& MIL_ToxicEffectManipulator::GetType() const
{
    return *types_.front();
}

// -----------------------------------------------------------------------------
// Name: MIL_ToxicEffectManipulator::GetRandomWound
// Created: JCR 2008-08-28
// -----------------------------------------------------------------------------
const PHY_HumanWound& MIL_ToxicEffectManipulator::GetRandomWound( const MIL_NbcAgentType& type ) const
{
    return ( type.IsLiquidContaminating() ) ?
             type.GetLiquidRandomWound() :
             type.GetGasRandomWound(); // GetGasRandomWound
}

// -----------------------------------------------------------------------------
// Name: MIL_ToxicEffectManipulator::ApplyWound
// Created: LGY 2012-12-13
// -----------------------------------------------------------------------------
void MIL_ToxicEffectManipulator::ApplyWound( Human_ABC& human ) const
{
    for( auto it = types_.begin(); it != types_.end(); ++it )
        human.ApplyWound( GetRandomWound( **it ) );
}
