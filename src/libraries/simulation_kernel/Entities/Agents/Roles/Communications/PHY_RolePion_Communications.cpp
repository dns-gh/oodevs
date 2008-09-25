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
#include "Entities/Objects/MIL_RealObject_ABC.h"
#include <xeumeuleu/xml.h>



MT_Float PHY_RolePion_Communications::rCoefSpeedModificator_         = 0.;
MT_Float PHY_RolePion_Communications::rCoefReloadingTimeModificator_ = 0.;

BOOST_CLASS_EXPORT_GUID( PHY_RolePion_Communications, "PHY_RolePion_Communications" )

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
PHY_RolePion_Communications::PHY_RolePion_Communications( MT_RoleContainer& role, MIL_AgentPion& pion )
    : PHY_RoleInterface_Communications( role )
    , pPion_                          ( &pion )
    , bHasChanged_                    ( true )
    , bBlackoutActivated_             ( false )
{
}
 
 
// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Communications constructor
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
PHY_RolePion_Communications::PHY_RolePion_Communications()
    : PHY_RoleInterface_Communications()
    , pPion_                          ( 0 )
{
}
 
// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Communications destructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RolePion_Communications::~PHY_RolePion_Communications()
{

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
                MIL_RealObject_ABC* pObject;
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
         & pPion_
         & jammers_
         & bBlackoutActivated_
         & bHasChanged_;
}
// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Communications::Jam
// Created: NLD 2004-11-04
// -----------------------------------------------------------------------------
void PHY_RolePion_Communications::Jam( const MIL_RealObject_ABC& jammer )
{
    // UAC ...
    if( pPion_->IsAutonomous() ) 
        return;
    bHasChanged_ = jammers_.insert( &jammer ).second;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Communications::Unjam
// Created: NLD 2004-11-04
// -----------------------------------------------------------------------------
void PHY_RolePion_Communications::Unjam( const MIL_RealObject_ABC& jammer )
{
    bHasChanged_ = ( jammers_.erase( &jammer ) == 1 );
}

// =============================================================================
// NETWORK
// =============================================================================

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
