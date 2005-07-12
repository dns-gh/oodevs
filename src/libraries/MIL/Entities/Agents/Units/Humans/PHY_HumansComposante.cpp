// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Humans/PHY_HumansComposante.cpp $
// $Author: Jvt $
// $Modtime: 1/04/05 11:26 $
// $Revision: 12 $
// $Workfile: PHY_HumansComposante.cpp $
//
// *****************************************************************************

#include "MIL_Pch.h"

#include "PHY_HumansComposante.h"

#include "PHY_Human.h"
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Entities/Agents/Actions/Firing/PHY_AgentFireResult.h"
#include "Entities/Agents/Units/Humans/PHY_HumanRank.h"

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante constructor
// Created: NLD 2004-08-16
// -----------------------------------------------------------------------------
PHY_HumansComposante::PHY_HumansComposante( PHY_ComposantePion& composante, uint nNbrMdr )
    : pComposante_( &composante )
    , humans_     ()
{
    while ( nNbrMdr-- )
        humans_.push_back( new PHY_Human( composante ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante constructor
// Created: JVT 2005-04-01
// -----------------------------------------------------------------------------
PHY_HumansComposante::PHY_HumansComposante()
    : pComposante_( 0 )
    , humans_     ()
{
}

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante destructor
// Created: NLD 2004-08-16
// -----------------------------------------------------------------------------
PHY_HumansComposante::~PHY_HumansComposante()
{
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante::ChangeHumanRank
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
bool PHY_HumansComposante::ChangeHumanRank( const PHY_HumanRank& oldRank, const PHY_HumanRank& newRank, const PHY_HumanWound& wound )
{
    if( oldRank == newRank )
        return false;

    for( CIT_HumanVector it = humans_.begin(); it != humans_.end(); ++it )
    {
        PHY_Human& human = **it;
        if( human.GetRank() == oldRank && human.GetWound() == wound )
        {
            human.ChangeRank( newRank );
            return true;
        }
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante::ChangeHumansWound
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
uint PHY_HumansComposante::ChangeHumansWound( const PHY_HumanRank& rank, const PHY_HumanWound& oldWound, const PHY_HumanWound& newWound, uint nNbrToChange )
{
    if( oldWound == newWound )
        return 0;

    uint nNbrChanged = 0;
    for( CIT_HumanVector it = humans_.begin(); it != humans_.end() && nNbrToChange ; ++it )
    {
        PHY_Human& human = **it;
        if( human.GetWound() == oldWound && human.GetRank() == rank )
        {
            human.ChangeWound( newWound );
            --nNbrToChange;
            ++nNbrChanged; 
        }
    }
    return nNbrChanged;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante::KillAllHumans
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
void PHY_HumansComposante::KillAllHumans()
{
    for( CIT_HumanVector it = humans_.begin(); it != humans_.end() ; ++it )
        (**it).ChangeWound( PHY_HumanWound::killed_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante::ChangeAllHumansWound
// Created: NLD 2004-10-07
// -----------------------------------------------------------------------------
void PHY_HumansComposante::KillAllHumans( PHY_AgentFireResult& fireResult )
{
    for( CIT_HumanVector it = humans_.begin(); it != humans_.end() ; ++it )
    {
        PHY_Human& human = **it;
        const PHY_HumanWound& oldWound = human.GetWound();
        if( human.ChangeWound( PHY_HumanWound::killed_ ) )
            fireResult.NotifyHumanWoundChanged( human, oldWound );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante::Resupply
// Created: NLD 2004-09-21
// -----------------------------------------------------------------------------
void PHY_HumansComposante::Resupply()
{
    for( CIT_HumanVector it = humans_.begin(); it != humans_.end(); ++it )
        (**it).Resupply();
}

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante::ApplyWounds
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
void PHY_HumansComposante::ApplyWounds( const PHY_ComposanteState& newComposanteState, PHY_AgentFireResult& fireResult )
{
    std::random_shuffle( humans_.begin(), humans_.end() );

    assert( pComposante_ );

    uint nNbrToDo = (uint)( humans_.size() * pComposante_->GetType().GetProtection().GetHumanWoundFactor( newComposanteState ) );
    for( CIT_HumanVector it = humans_.begin(); it != humans_.end(); ++it )
    {
        PHY_Human& human = **it;
        if( !human.IsUsable() )
            continue;

        if( nNbrToDo )
        {
            -- nNbrToDo;
            const PHY_HumanWound& oldWound = human.GetWound();
            if( human.ApplyWound() )
                fireResult.NotifyHumanWoundChanged( human, oldWound );
        }
        human.ApplyMentalDisease();
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante::ApplyWounds
// Created: NLD 2004-10-13
// -----------------------------------------------------------------------------
void PHY_HumansComposante::ApplyWounds( const MIL_NbcAgentType& nbcAgentType )
{
    for( CIT_HumanVector it = humans_.begin(); it != humans_.end(); ++it )
    {
        PHY_Human& human = **it;
        if( human.IsUsable() )
            human.ApplyWound( nbcAgentType );
    }
}

// =============================================================================
// COMPOSANTE MAINTENANCE
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante::NotifyComposanteHandledByMaintenance
// Created: NLD 2005-01-14
// -----------------------------------------------------------------------------
void PHY_HumansComposante::NotifyComposanteHandledByMaintenance()
{
    for( CIT_HumanVector it = humans_.begin(); it != humans_.end(); ++it )
        (**it).NotifyComposanteHandledByMaintenance();
}

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante::NotifyComposanteBackFromMaintenance
// Created: NLD 2005-01-14
// -----------------------------------------------------------------------------
void PHY_HumansComposante::NotifyComposanteBackFromMaintenance()
{
    for( CIT_HumanVector it = humans_.begin(); it != humans_.end(); ++it )
        (**it).NotifyComposanteBackFromMaintenance();
}

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante::NotifyComposanteTransfered
// Created: JVT 2005-01-17
// -----------------------------------------------------------------------------
void PHY_HumansComposante::NotifyComposanteTransfered( PHY_RolePion_Composantes& src, PHY_RolePion_Composantes& dest )
{
    for ( CIT_HumanVector it = humans_.begin(); it != humans_.end(); ++it )
    {
        (**it).CancelLogisticRequest();
        src.NotifyHumanRemoved( **it );
        dest.NotifyHumanAdded( **it );
    }
}
