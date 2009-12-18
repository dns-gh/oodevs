// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "simulation_kernel/DefaultHealComputer.h"
#include "simulation_kernel/Entities/Agents/MIL_Agent_ABC.h"

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
// Name: DefaultHealableComputer::ApplyOnComposante( PHY_ComposantePion& component ) = 0;
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
  std::random_shuffle( components.begin(), components.end() );

  PHY_ComposantePion::IT_ComposantePionVector itCurrentComp = components.begin();
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
    std::random_shuffle( composantes.begin(), composantes.end() );

    PHY_ComposantePion::IT_ComposantePionVector itCurrentComp = composantes.begin();
    while( nNbrToChange && itCurrentComp != composantes.end() )
    {
        uint nNbrChanged = (*itCurrentComp)->WoundHumans( rank, 1, PHY_HumanWound::killed_ );
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
void DefaultHealComputer::HealAll() const
{
    for( PHY_ComposantePion::CIT_ComposantePionVector it = components_.begin(); it != components_.end(); ++it )
        (**it).HealAllHumans();
}

// -----------------------------------------------------------------------------
// Name: DefaultHealableComputer::HealAll
// Created: MGD 2009-09-24
// -----------------------------------------------------------------------------
void DefaultHealComputer::EvacuateWoundedHumans( MIL_AutomateLOG& destinationTC2 ) const
{
    for( PHY_ComposantePion::CIT_ComposantePionVector it = components_.begin(); it != components_.end(); ++it )
        (**it).EvacuateWoundedHumans( destinationTC2 );
}
// -----------------------------------------------------------------------------
// Name: DefaultHealableComputer::HealAll
// Created: MGD 2009-09-24
// -----------------------------------------------------------------------------
bool DefaultHealComputer::HasWoundedHumansToEvacuate() const
{
    for( PHY_ComposantePion::CIT_ComposantePionVector it = components_.begin(); it != components_.end(); ++it )
    {
        if( (**it).HasWoundedHumansToEvacuate() )
            return true;
    }
    return false;
}

} // namespace human