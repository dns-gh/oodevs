// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Posture/PHY_RolePion_Posture.cpp $
// $Author: Jvt $
// $Modtime: 11/05/05 15:26 $
// $Revision: 12 $
// $Workfile: PHY_RolePion_Posture.cpp $
//
// *****************************************************************************

#include "MIL_Pch.h"

#include "PHY_RolePion_Posture.h"

#include "Entities/Agents/Units/Postures/PHY_Posture.h"
#include "Entities/Agents/Units/PHY_UnitType.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Transported/PHY_RolePion_Transported.h"
#include "Entities/Agents/Roles/Perception/PHY_RolePion_Perceiver.h"
#include "Entities/Agents/Roles/HumanFactors/PHY_RolePion_HumanFactors.h"
#include "Network/NET_ASN_Messages.h"
#include "Hla/HLA_UpdateFunctor.h"

MT_Random PHY_RolePion_Posture::random_;

BOOST_CLASS_EXPORT_GUID( PHY_RolePion_Posture, "PHY_RolePion_Posture" )

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Posture constructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RolePion_Posture::PHY_RolePion_Posture( MT_RoleContainer& role, const MIL_AgentPion& pion )
    : PHY_RoleInterface_Posture        ( role )
    , pPion_                           ( &pion )
    , pCurrentPosture_                 ( &PHY_Posture::arret_ )
    , pLastPosture_                    ( &PHY_Posture::arret_ )
    , rPostureCompletionPercentage_    ( 1. )
    , bDiscreteModeEnabled_            ( false )
    , bPosturesHasChanged_             ( true  )
    , bPercentageCrossed50_            ( false )
    , bStealthFactorHasChanged_        ( true  )
    , bPourcentageHasChanged_          ( true  )
    , rLastPostureCompletionPercentage_( 0. )
    , bIsStealth_                      ( false )
    , rTimingFactor_                   ( 1. )
    , rStealthFactor_                  ( 1. ) // Non furtif
    , rElongationFactor_               ( 1. )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Posture constructor
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
PHY_RolePion_Posture::PHY_RolePion_Posture()
    : PHY_RoleInterface_Posture        (  )
    , pPion_                           ( 0 )
    , pCurrentPosture_                 ( 0 )
    , pLastPosture_                    ( 0 )
    , rPostureCompletionPercentage_    ( 1. )
    , bDiscreteModeEnabled_            ( false )
    , bPosturesHasChanged_             ( true  )
    , bPercentageCrossed50_            ( false )
    , bStealthFactorHasChanged_        ( true  )
    , bPourcentageHasChanged_          ( true  )
    , rLastPostureCompletionPercentage_( 0. )
    , bIsStealth_                      ( false )
    , rTimingFactor_                   ( 1. )
    , rStealthFactor_                  ( 1. ) // Non furtif
    , rElongationFactor_               ( 1. )
{

}
  
// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Posture destructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RolePion_Posture::~PHY_RolePion_Posture()
{
}

// =============================================================================
// CHECKPOITNS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Posture::load
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
void PHY_RolePion_Posture::load( MIL_CheckPointInArchive& file, const uint )
{
    file >> boost::serialization::base_object< PHY_RoleInterface_Posture >( *this )
         >> const_cast< MIL_AgentPion*& >( pPion_ );
    
    uint nID;
    file >> nID;
    pCurrentPosture_ = PHY_Posture::FindPosture( nID );

    file >> nID;
    pLastPosture_ = PHY_Posture::FindPosture( nID );
    
    file >> rPostureCompletionPercentage_
         >> rElongationFactor_
         >> bDiscreteModeEnabled_
         >> rTimingFactor_
         >> rStealthFactor_
         >> bIsStealth_
         >> bPercentageCrossed50_
         >> rLastPostureCompletionPercentage_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Posture::save
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
void PHY_RolePion_Posture::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    file << boost::serialization::base_object< PHY_RoleInterface_Posture >( *this )
         << pPion_
         << pCurrentPosture_->GetID()
         << pLastPosture_->GetID()
         << rPostureCompletionPercentage_
         << rElongationFactor_
         << bDiscreteModeEnabled_
         << rTimingFactor_
         << rStealthFactor_
         << bIsStealth_
         << bPercentageCrossed50_
         << rLastPostureCompletionPercentage_;
}

// =============================================================================
// TOOLS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Posture::GetPostureTime
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
inline
MT_Float PHY_RolePion_Posture::GetPostureTime() const
{
    assert( pCurrentPosture_ );
    assert( pPion_ );
    assert( rTimingFactor_ > 0. );
    
    return GetRole< PHY_RolePion_HumanFactors >().ModifyPostureTime( pPion_->GetType().GetUnitType().GetPostureTime( *pCurrentPosture_ ) ) / rTimingFactor_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Posture::ChangePostureCompletionPercentage
// Created: NLD 2005-07-27
// -----------------------------------------------------------------------------
inline
void PHY_RolePion_Posture::ChangePostureCompletionPercentage( const MT_Float rNewPercentage )
{
    if( rPostureCompletionPercentage_ == rNewPercentage )
        return;

    // Network
    bPercentageCrossed50_ = ( 0.5 - rPostureCompletionPercentage_ ) * ( 0.5 - rNewPercentage ) <= 0;

    static const MT_Float rDeltaPercentageForNetwork = 0.05;
    if( fabs( rLastPostureCompletionPercentage_ - rNewPercentage ) > rDeltaPercentageForNetwork || rNewPercentage == 0. || rNewPercentage == 1. )
        bPourcentageHasChanged_ = true;

    rPostureCompletionPercentage_ = rNewPercentage;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Posture::ChangePosture
// Created: NLD 2005-07-27
// -----------------------------------------------------------------------------
inline
void PHY_RolePion_Posture::ChangePosture( const PHY_Posture& newPosture )
{
    if( pCurrentPosture_ == &newPosture )
        return;
    
    pLastPosture_                 = pCurrentPosture_;
    pCurrentPosture_              = &newPosture;

    ChangePostureCompletionPercentage( 0. );

    bPosturesHasChanged_          = true;
    bPourcentageHasChanged_       = true;
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Posture::Update
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void PHY_RolePion_Posture::Update( bool bIsDead )
{
    if( bIsDead )
    {
        ChangePosture                    ( PHY_Posture::arret_ );
        ChangePostureCompletionPercentage( 1. );
        bIsStealth_      = false;
        return;
    }

    // Mode furtif
    bIsStealth_ = !( random_.rand_oi( 0., 1. ) <= rStealthFactor_ );

    if( rPostureCompletionPercentage_ == 1. )
    {
        assert( pCurrentPosture_ );
        const PHY_Posture* pNextAutoPosture = pCurrentPosture_->GetNextAutoPosture();
        if( !pNextAutoPosture )
            return;
        ChangePosture( *pNextAutoPosture );
    }
    else
    {
        const MT_Float rPostureTime                = GetPostureTime();
              MT_Float rNewPostureCompetionPercentage = rPostureCompletionPercentage_;
        if( rPostureTime )
        {
            rNewPostureCompetionPercentage += ( 1. / rPostureTime );
            if( rNewPostureCompetionPercentage > 1. )
                rNewPostureCompetionPercentage = 1.;
        }
        else
            rNewPostureCompetionPercentage = 1.;

        ChangePostureCompletionPercentage( rNewPostureCompetionPercentage );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Posture::SetPostureMovement
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void PHY_RolePion_Posture::SetPostureMovement()
{
    if( bDiscreteModeEnabled_ )
        ChangePosture( PHY_Posture::mouvementDiscret_ );
    else
        ChangePosture( PHY_Posture::mouvement_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Posture::UnsetPostureMovement
// Created: JVT 2004-09-28
// -----------------------------------------------------------------------------
void PHY_RolePion_Posture::UnsetPostureMovement()
{
    if ( pCurrentPosture_ == &PHY_Posture::mouvement_ 
      || pCurrentPosture_ == &PHY_Posture::mouvementDiscret_ )
        ChangePosture( PHY_Posture::arret_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Posture::SetPosturePostePrepareGenie
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void PHY_RolePion_Posture::SetPosturePostePrepareGenie()
{
    ChangePosture( PHY_Posture::postePrepareGenie_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Posture::UnsetPosturePostePrepareGenie
// Created: JVT 2004-09-28
// -----------------------------------------------------------------------------
void PHY_RolePion_Posture::UnsetPosturePostePrepareGenie()
{
    if ( pCurrentPosture_ == &PHY_Posture::postePrepareGenie_ )
        ChangePosture( PHY_Posture::arret_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Posture::CanBePerceived
// Created: NLD 2004-11-22
// -----------------------------------------------------------------------------
bool PHY_RolePion_Posture::CanBePerceived( const MIL_AgentPion& perceiver ) const
{
    assert( pPion_ );
    return     ( !bIsStealth_ || perceiver.GetRole< PHY_RolePion_Perceiver >().WasPerceived( *pPion_ ) ) 
            && !GetRole< PHY_RolePion_Transported >().IsTransported();
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Posture::SendChangedState
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
void PHY_RolePion_Posture::SendChangedState( NET_ASN_MsgUnitAttributes& msg )
{
    if( bPosturesHasChanged_ )
    {
        msg.GetAsnMsg().m.posture_oldPresent = 1;
        msg.GetAsnMsg().m.posture_newPresent = 1;
        msg.GetAsnMsg().posture_old = pLastPosture_   ->GetAsnID();
        msg.GetAsnMsg().posture_new = pCurrentPosture_->GetAsnID();
    }

    if( bPourcentageHasChanged_ )
    {
        msg.GetAsnMsg().m.posture_pourcentagePresent = 1;
        msg.GetAsnMsg().posture_pourcentage          = (uint)( rPostureCompletionPercentage_ * 100. );
        rLastPostureCompletionPercentage_            = rPostureCompletionPercentage_;
    }

    if( bStealthFactorHasChanged_ )
    {
        msg.GetAsnMsg().m.mode_furtif_actifPresent = 1;
        msg.GetAsnMsg().mode_furtif_actif          = ( rStealthFactor_ < 1. );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Posture::SendFullState
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
void PHY_RolePion_Posture::SendFullState( NET_ASN_MsgUnitAttributes& msg )
{
    msg.GetAsnMsg().m.posture_oldPresent = 1;
    msg.GetAsnMsg().m.posture_newPresent = 1;
    msg.GetAsnMsg().posture_old = pLastPosture_   ->GetAsnID();
    msg.GetAsnMsg().posture_new = pCurrentPosture_->GetAsnID();

    msg.GetAsnMsg().m.posture_pourcentagePresent = 1;
    msg.GetAsnMsg().posture_pourcentage          = (uint)( rPostureCompletionPercentage_ * 100. );
    rLastPostureCompletionPercentage_            = rPostureCompletionPercentage_;

    msg.GetAsnMsg().m.mode_furtif_actifPresent = 1;
    msg.GetAsnMsg().mode_furtif_actif          = ( rStealthFactor_ < 1. );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Posture::Serialize
// Created: AGE 2004-11-10
// -----------------------------------------------------------------------------
void PHY_RolePion_Posture::Serialize( HLA_UpdateFunctor& functor ) const
{
    const PHY_Posture* pPosture = rPostureCompletionPercentage_ < 0.5 ? pLastPosture_ : pCurrentPosture_;
    functor.Serialize( "posture", HLAStatusHasChanged(), pPosture->GetName() );
}

