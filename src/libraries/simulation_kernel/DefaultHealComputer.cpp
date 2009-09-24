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

using namespace human;

// -----------------------------------------------------------------------------
// Name: DefaultHealableComputer::DefaultHealableComputer
// Created: MGD 2009-09-21
// -----------------------------------------------------------------------------
DefaultHealComputer::DefaultHealComputer()
{

}

// -----------------------------------------------------------------------------
// Name: DefaultHealableComputer::~DefaultHealableComputer
// Created: MGD 2009-09-21
// -----------------------------------------------------------------------------
DefaultHealComputer::~DefaultHealComputer()
{

}

// -----------------------------------------------------------------------------
// Name: DefaultHealableComputer::Reset
// Created: MGD 2009-09-24
// -----------------------------------------------------------------------------
void DefaultHealComputer::Reset( const PHY_HumanRank& rank, unsigned int nNbrToChange )
{
  pRank_ = &rank;
  nNbrToChange_ = nNbrToChange;
  healablesComponents_.clear();
}

// -----------------------------------------------------------------------------
// Name: DefaultHealableComputer::ApplyOnComposante( PHY_ComposantePion& component ) = 0;
// Created: MGD 2009-09-24
// -----------------------------------------------------------------------------
void DefaultHealComputer::ApplyOnComposante( PHY_ComposantePion& component )
{
  healablesComponents_.push_back( &component );
}

// -----------------------------------------------------------------------------
// Name: DefaultHealableComputer::GetHealables
// Created: MGD 2009-09-24
// -----------------------------------------------------------------------------
void DefaultHealComputer::Heal() const
{
  PHY_ComposantePion::T_ComposantePionVector components = healablesComponents_;
  std::random_shuffle( components.begin(), components.end() );

  PHY_ComposantePion::IT_ComposantePionVector itCurrentComp = components.begin();
  unsigned int nNbrToChange = nNbrToChange_;
  while( nNbrToChange && itCurrentComp != components.end() )
  {
    unsigned int nNbrChanged = (*itCurrentComp)->HealHumans( *pRank_, 1 );
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