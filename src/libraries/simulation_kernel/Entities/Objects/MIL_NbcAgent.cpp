// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_NbcAgent.h"

#include "MIL_NbcAgentType.h"
#include "Entities/Agents/Units/Humans/PHY_HumanWound.h"

// -----------------------------------------------------------------------------
// Name: MIL_NbcAgent constructor
// Created: NLD 2006-10-27
// -----------------------------------------------------------------------------
MIL_NbcAgent::MIL_NbcAgent()
    : pType_( 0 )
    , nForm_( eLiquid )
{
}

// -----------------------------------------------------------------------------
// Name: MIL_NbcAgent constructor
// Created: NLD 2006-10-27
// -----------------------------------------------------------------------------
MIL_NbcAgent::MIL_NbcAgent( const MIL_NbcAgentType& type, E_Form nForm )
    : pType_( &type )
    , nForm_( nForm )
{
}

// -----------------------------------------------------------------------------
// Name: MIL_NbcAgent destructor
// Created: NLD 2006-10-27
// -----------------------------------------------------------------------------
MIL_NbcAgent::~MIL_NbcAgent()
{

}

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_NbcAgent::load
// Created: NLD 2006-10-27
// -----------------------------------------------------------------------------
void MIL_NbcAgent::load( MIL_CheckPointInArchive& file, const uint )
{
    uint nTypeID;
    file >> nTypeID;
    file >> const_cast< E_Form& >( nForm_ );

    pType_ = MIL_NbcAgentType::Find( nTypeID );
    assert( pType_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_NbcAgent::save
// Created: NLD 2006-10-27
// -----------------------------------------------------------------------------
void MIL_NbcAgent::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    assert( pType_ );
    unsigned id = pType_->GetID();
    file << id
         << nForm_;
}

// =============================================================================
// ACCESSORS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_NbcAgent::GetRandomWound
// Created: NLD 2006-10-27
// -----------------------------------------------------------------------------
const PHY_HumanWound& MIL_NbcAgent::GetRandomWound() const
{
    assert( pType_ );
    switch( nForm_ )
    {
        case eLiquid: return pType_->GetLiquidRandomWound();
        case eGas   : return pType_->GetGasRandomWound   ();
        default:
            assert( false );
            return PHY_HumanWound::notWounded_;
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_NbcAgent::GetLifeTime
// Created: NLD 2006-10-27
// -----------------------------------------------------------------------------
uint MIL_NbcAgent::GetLifeTime() const
{
    assert( pType_ );
    switch( nForm_ )
    {
        case eLiquid: return std::numeric_limits< uint >::max();
        case eGas   : return pType_->GetGasLifeTime();
        default:
            assert( false );
            return 0;
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_NbcAgent::GetPropagationAngle
// Created: NLD 2006-10-27
// -----------------------------------------------------------------------------
MT_Float MIL_NbcAgent::GetPropagationAngle() const
{
    assert( pType_ );
    switch( nForm_ )
    {
        case eLiquid: return std::numeric_limits< uint >::max();
        case eGas   : return pType_->GetGasPropagationAngle();
        default:
            assert( false );
            return 0;
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_NbcAgent::IsPoisonous
// Created: NLD 2006-10-30
// -----------------------------------------------------------------------------
bool MIL_NbcAgent::IsPoisonous() const
{
    assert( pType_ );
    switch( nForm_ )
    {
        case eLiquid: return pType_->IsLiquidPoisonous();
        case eGas   : return pType_->IsGasPoisonous   ();
        default:
            assert( false );
            return 0;
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_NbcAgent::IsContaminating
// Created: NLD 2006-10-30
// -----------------------------------------------------------------------------
bool MIL_NbcAgent::IsContaminating() const
{
    assert( pType_ );
    switch( nForm_ )
    {
        case eLiquid:  return pType_->IsLiquidContaminating();
        case eGas   :  return pType_->IsGasContaminating   ();
        default:
            assert( false );
            return 0;
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_NbcAgent::CanBeVaporized
// Created: NLD 2006-10-27
// -----------------------------------------------------------------------------
bool MIL_NbcAgent::CanBeVaporized() const
{
    assert( pType_ );
    return pType_->CanBeVaporized();
}

