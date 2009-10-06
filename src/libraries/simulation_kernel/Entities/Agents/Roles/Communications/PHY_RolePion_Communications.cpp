// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Communications/PHY_RolePion_Communications.cpp $
// $Author: Jvt $
// $Modtime: 6/04/05 18:12 $
// $Revision: 4 $
// $Workfile: PHY_RolePion_Communications.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_RolePion_Communications.h"
#include "Network/NET_ASN_Messages.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Objects/MIL_Object_ABC.h"

#include "simulation_kernel/NetworkNotificationHandler_ABC.h"
#include "simulation_kernel/SpeedComputer_ABC.h"

#include <xeumeuleu/xml.h>

MT_Float PHY_RolePion_Communications::rCoefSpeedModificator_         = 0.;
MT_Float PHY_RolePion_Communications::rCoefReloadingTimeModificator_ = 0.;

BOOST_CLASS_EXPORT_GUID( PHY_RolePion_Communications, "PHY_RolePion_Communications" )

template< typename Archive >
void save_construct_data( Archive& archive, const PHY_RolePion_Communications* role, const unsigned int /*version*/ )
{
    MIL_AgentPion* const pion = &role->pion_;
    archive << pion;
}

template< typename Archive >
void load_construct_data( Archive& archive, PHY_RolePion_Communications* role, const unsigned int /*version*/ )
{
	MIL_AgentPion* pion;
	archive >> pion;
	::new( role )PHY_RolePion_Communications( *pion );
}


// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Communications::Initialize
// Created: NLD 2004-11-08
// -----------------------------------------------------------------------------
void PHY_RolePion_Communications::Initialize( xml::xistream& xis )
{
    xis >> xml::start( "communications" )
            >> xml::start( "communication-breakdown" )
                >> xml::attribute( "speed-modifier", rCoefSpeedModificator_ )
                >> xml::attribute( "reloading-time-modifier", rCoefReloadingTimeModificator_ )
            >> xml::end()
        >> xml::end();

    if( rCoefSpeedModificator_ < 0 )
        xis.error( "communication-breakdown: speed-modifier < 0" );
    if( rCoefReloadingTimeModificator_ <= 0 )
        xis.error( "communication-breakdown: reloading-time-modifier <= 0" );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Communications constructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RolePion_Communications::PHY_RolePion_Communications( MIL_AgentPion& pion )
    : pion_                           ( pion )
    , bHasChanged_                    ( true )
    , bBlackoutActivated_             ( false )
{
    // NOTHING
}
 
// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Communications destructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RolePion_Communications::~PHY_RolePion_Communications()
{
    // NOTHING
}

// =============================================================================
// CHECKPOINTS
// =============================================================================
namespace boost
{
    namespace serialization
    {        
        template< typename Archive >
        void save( Archive& file, const PHY_RolePion_Communications::T_JammerSet& set, const uint )
        {
            unsigned size = set.size();
            file << size;
            for ( PHY_RolePion_Communications::CIT_JammerSet it = set.begin(); it != set.end(); ++it )
                file << *it;
        }
        
        template< typename Archive >
        void load( Archive& file, PHY_RolePion_Communications::T_JammerSet& set, const uint )
        {
            uint nNbr;
            file >> nNbr;
            while ( nNbr-- )
            {
                MIL_Object_ABC* pObject;
                file >> pObject;
                set.insert( pObject );
            }
        }

        template< typename Archive >
        inline void serialize( Archive& file, PHY_RolePion_Communications::T_JammerSet& set, const uint nVersion )
        {
            split_free( file, set, nVersion );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Communications::serialize
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
template< typename Archive >
inline
void PHY_RolePion_Communications::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< PHY_RoleInterface_Communications >( *this )
         & jammers_
         & bBlackoutActivated_
         & bHasChanged_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Communications::Jam
// Created: NLD 2004-11-04
// -----------------------------------------------------------------------------
void PHY_RolePion_Communications::Jam( const MIL_Object_ABC& jammer )
{
    // UAC ...
    if( pion_.IsAutonomous() ) 
        return;
    bHasChanged_ = jammers_.insert( &jammer ).second;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Communications::Unjam
// Created: NLD 2004-11-04
// -----------------------------------------------------------------------------
void PHY_RolePion_Communications::Unjam( const MIL_Object_ABC& jammer )
{
    bHasChanged_ = ( jammers_.erase( &jammer ) == 1 );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Communications::SendFullState
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
void PHY_RolePion_Communications::SendFullState( NET_ASN_MsgUnitAttributes& msg ) const
{
    msg().m.communications_brouilleesPresent = 1;
    msg().communications_brouillees = !jammers_.empty();

    msg().m.silence_radioPresent  = 1;
    msg().silence_radio           = bBlackoutActivated_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Communications::SendChangedState
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
void PHY_RolePion_Communications::SendChangedState( NET_ASN_MsgUnitAttributes& msg ) const
{
    if( bHasChanged_ )
        SendFullState( msg );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Communications::Update
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void PHY_RolePion_Communications::Update( bool /*bIsDead*/ )
{
    if( HasChanged() )
    {
        pion_.Apply( &network::NetworkNotificationHandler_ABC::NotifyDataHasChanged );
    } 
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Communications::Clean
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
void PHY_RolePion_Communications::Clean()
{
    bHasChanged_ = false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Communications::HasChanged
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
bool PHY_RolePion_Communications::HasChanged() const
{
    return bHasChanged_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Communications::ActivateBlackout
// Created: NLD 2004-11-08
// -----------------------------------------------------------------------------
void PHY_RolePion_Communications::ActivateBlackout()
{
    if( bBlackoutActivated_ )
        return;
    bBlackoutActivated_ = true;
    bHasChanged_        = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Communications::DeactivateBlackout
// Created: NLD 2004-11-08
// -----------------------------------------------------------------------------
void PHY_RolePion_Communications::DeactivateBlackout()
{
    if( !bBlackoutActivated_ )
        return;
    bBlackoutActivated_ = false;
    bHasChanged_        = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Communications::CanCommunicate
// Created: NLD 2004-11-08
// -----------------------------------------------------------------------------
bool PHY_RolePion_Communications::CanCommunicate() const
{
    return jammers_.empty() && !bBlackoutActivated_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Communications::ModifySpeed
// Created: NLD 2004-09-23
// -----------------------------------------------------------------------------
void PHY_RolePion_Communications::Execute(moving::SpeedComputer_ABC& algorithm) const
{
    if( !jammers_.empty() )
    	algorithm.AddModifier(rCoefSpeedModificator_, false);
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Communications::ModifyReloadingDuration
// Created: NLD 2004-10-07
// -----------------------------------------------------------------------------
MT_Float PHY_RolePion_Communications::ModifyReloadingDuration( MT_Float rDuration ) const
{
    if( jammers_.empty() )
        return rDuration;
    return rDuration *= rCoefReloadingTimeModificator_;
}
