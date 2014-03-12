// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_Random.h"
#include "DefaultHealComputer.h"
#include "Entities/Agents/Units/Humans/PHY_HumanWound.h"

namespace human
{

// -----------------------------------------------------------------------------
// Name: DefaultHealableComputer::DefaultHealableComputer
// Created: MGD 2009-09-21
// -----------------------------------------------------------------------------
DefaultHealComputer::DefaultHealComputer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DefaultHealableComputer::~DefaultHealableComputer
// Created: MGD 2009-09-21
// -----------------------------------------------------------------------------
DefaultHealComputer::~DefaultHealComputer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DefaultHealableComputer::ApplyOnComponent
// Created: MGD 2009-09-24
// -----------------------------------------------------------------------------
void DefaultHealComputer::ApplyOnComponent( PHY_ComposantePion& component )
{
    components_.push_back( &component );
}

// -----------------------------------------------------------------------------
// Name: DefaultHealableComputer::Heal
// Created: MGD 2009-09-24
// -----------------------------------------------------------------------------
void DefaultHealComputer::Heal( const PHY_HumanRank& rank, unsigned int nNbrToChange ) const
{
    PHY_ComposantePion::T_ComposantePionVector components = components_;
    MIL_Random::random_shuffle( components, MIL_Random::eWounds );
    auto itCurrentComp = components.begin();
    while( nNbrToChange && itCurrentComp != components.end() )
    {
        unsigned int nNbrChanged = (*itCurrentComp)->HealHumans( rank, 1 );
        if( nNbrChanged == 0 )
            itCurrentComp = components.erase( itCurrentComp );
        else
        {
            nNbrToChange -= nNbrChanged;
            ++ itCurrentComp;
        }
        if( itCurrentComp == components.end() )
            itCurrentComp = components.begin();
    }
}

// -----------------------------------------------------------------------------
// Name: DefaultHealableComputer::Wound
// Created: MGD 2009-09-24
// -----------------------------------------------------------------------------
void DefaultHealComputer::Wound( const PHY_HumanRank& rank, unsigned int nNbrToChange  ) const
{
    PHY_ComposantePion::T_ComposantePionVector composantes = components_;
    MIL_Random::random_shuffle( composantes, MIL_Random::eWounds );

    auto itCurrentComp = composantes.begin();
    while( nNbrToChange && itCurrentComp != composantes.end() )
    {
        unsigned int nNbrChanged = (*itCurrentComp)->WoundHumans( rank, 1, PHY_HumanWound::killed_ );
        if( nNbrChanged == 0 )
            itCurrentComp = composantes.erase( itCurrentComp );
        else
        {
            nNbrToChange -= nNbrChanged;
            ++ itCurrentComp;
        }
        if( itCurrentComp == composantes.end() )
            itCurrentComp = composantes.begin();
    }
}

// -----------------------------------------------------------------------------
// Name: DefaultHealableComputer::HealAll
// Created: MGD 2009-09-24
// -----------------------------------------------------------------------------
void DefaultHealComputer::HealAll( bool withLog ) const
{
    for( auto it = components_.begin(); it != components_.end(); ++it )
        (**it).HealAllHumans( withLog );
}

// -----------------------------------------------------------------------------
// Name: DefaultHealableComputer::HealAll
// Created: MGD 2009-09-24
// -----------------------------------------------------------------------------
void DefaultHealComputer::EvacuateWoundedHumans( MIL_AutomateLOG& destinationTC2 ) const
{
    for( auto it = components_.begin(); it != components_.end(); ++it )
        (**it).EvacuateWoundedHumans( destinationTC2 );
}

// -----------------------------------------------------------------------------
// Name: DefaultHealableComputer::HealAll
// Created: MGD 2009-09-24
// -----------------------------------------------------------------------------
bool DefaultHealComputer::HasWoundedHumansToEvacuate() const
{
    for( auto it = components_.begin(); it != components_.end(); ++it )
        if( (**it).HasWoundedHumansToEvacuate() )
            return true;
    return false;
}

} // namespace human
