// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Weapons/PHY_LauncherType.cpp $
// $Author: Age $
// $Modtime: 12/04/05 15:40 $
// $Revision: 4 $
// $Workfile: PHY_LauncherType.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "PHY_LauncherType.h"

#include "Entities/Agents/Units/Postures/PHY_Posture.h"
#include "Entities/Agents/Roles/Posture/PHY_RolePion_Posture.h"
#include "xeumeuleu/xml.h"

using namespace xml;

// =============================================================================
// MANAGER
// =============================================================================

PHY_LauncherType::T_LauncherTypeMap PHY_LauncherType::launcherTypes_;

struct PHY_LauncherType::LoadingWrapper
{
    void ReadLauncher( xml::xistream& xis )
    {
        PHY_LauncherType::ReadLauncher( xis );
    }
};

// -----------------------------------------------------------------------------
// Name: PHY_LauncherType::Initialize
// Created: NLD 2004-08-04
// -----------------------------------------------------------------------------
void PHY_LauncherType::Initialize( xml::xistream& xis )
{
    MT_LOG_INFO_MSG( "Initializing launcher types" );

    LoadingWrapper loader;

    xis >> start( "launchers" )
            >> list( "launcher", loader, &LoadingWrapper::ReadLauncher )
        >> end();
}

// -----------------------------------------------------------------------------
// Name: PHY_LauncherType::ReadLauncher
// Created: ABL 2007-07-20
// -----------------------------------------------------------------------------
void PHY_LauncherType::ReadLauncher( xml::xistream& xis )
{
        std::string strLauncherName;
        xis >> attribute( "name", strLauncherName );

        const PHY_LauncherType*& pLauncherType = launcherTypes_[ strLauncherName ];
        if ( pLauncherType )
            xis.error( "Launcher type already registered" );

        pLauncherType = new PHY_LauncherType( strLauncherName, xis );
}

// -----------------------------------------------------------------------------
// Name: PHY_LauncherType::Terminate
// Created: NLD 2004-08-04
// -----------------------------------------------------------------------------
void PHY_LauncherType::Terminate()
{
    for( CIT_LauncherTypeMap it = launcherTypes_.begin(); it != launcherTypes_.end(); ++it )
        delete it->second;
    launcherTypes_.clear();
}

// =============================================================================
// INITIALIZATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_LauncherType constructor
// Created: NLD 2004-08-04
// -----------------------------------------------------------------------------
PHY_LauncherType::PHY_LauncherType( const std::string& strName, xml::xistream& xis )
    : strName_       ( strName )
    , bDirectFire_   ( false )
    , bIndirectFire_ ( false )
    , phModificators_( PHY_Posture::GetPostures().size() )
{
    InitializeForDirectFire  ( xis );
    InitializeForIndirectFire( xis );
}

// -----------------------------------------------------------------------------
// Name: PHY_LauncherType destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
PHY_LauncherType::~PHY_LauncherType()
{
    phModificators_.clear();
}

// -----------------------------------------------------------------------------
// Name: PHY_LauncherType::InitializeForIndirectFire
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void PHY_LauncherType::InitializeForIndirectFire( xml::xistream& xis )
{
    bIndirectFire_ = false;
    xis >> optional()
        >> attribute( "indirect-fire", bIndirectFire_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_LauncherType::InitializeForDirectFire
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void PHY_LauncherType::InitializeForDirectFire( xml::xistream& xis )
{
    xis >> list( "ph-modifiers", *this, &PHY_LauncherType::ReadDirect );
}

// -----------------------------------------------------------------------------
// Name: PHY_LauncherType::ReadDirect
// Created: ABL 2007-07-20
// -----------------------------------------------------------------------------
void PHY_LauncherType::ReadDirect( xml::xistream& xis )
{

    bDirectFire_ = true;
    const PHY_Posture::T_PostureMap& postures = PHY_Posture::GetPostures();
    std::string postureType;

    xis >> attribute( "posture", postureType );
    PHY_Posture::CIT_PostureMap it = postures.find( postureType );
    const PHY_Posture& postureSource = *it->second;
    if( !postureSource.CanModifyPH() )
        return;

    xis >> list( "ph-modifier", *this, &PHY_LauncherType::ReadModifier, postureSource );
}

// -----------------------------------------------------------------------------
// Name: PHY_LauncherType::ReadModifier
// Created: ABL 2007-07-20
// -----------------------------------------------------------------------------
void PHY_LauncherType::ReadModifier( xml::xistream& xis, const PHY_Posture& postureSource )
{
    const PHY_Posture::T_PostureMap& postures = PHY_Posture::GetPostures();
    std::string targetType;
    xis >> attribute( "target-posture", targetType );
    PHY_Posture::CIT_PostureMap it = postures.find( targetType );
    const PHY_Posture& postureTarget = *it->second;
    if( !postureTarget.CanModifyPH() )
        return;

    MT_Float rModificatorValue;
    xis >> attribute( "value", rModificatorValue );

    if( rModificatorValue < 0 || rModificatorValue > 1 )
        xis.error( "target-posture: value not in [0..1]" );

    RegisterPHModificator( postureSource, postureTarget, rModificatorValue );
}

// -----------------------------------------------------------------------------
// Name: PHY_LauncherType::RegisterPHModificator
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void PHY_LauncherType::RegisterPHModificator( const PHY_Posture& postureSource, const PHY_Posture& postureTarget, MT_Float rModificatorValue )
{
    assert( rModificatorValue >= 0. && rModificatorValue <= 1. );
    assert( phModificators_.size() > postureSource.GetID() );

    T_PhModificatorValueVector& phModificatorValues = phModificators_[ postureSource.GetID() ];

    if( phModificatorValues.size() <= postureTarget.GetID() )
        phModificatorValues.resize( PHY_Posture::GetPostures().size() );

    assert( phModificatorValues.size() > postureTarget.GetID() );

    phModificatorValues[ postureTarget.GetID() ] = rModificatorValue;
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_LauncherType::GetPHModificator
// Created: NLD 2004-10-05
// -----------------------------------------------------------------------------
MT_Float PHY_LauncherType::GetPHModificator( const PHY_RolePion_Posture& firerPosture, const PHY_RoleInterface_Posture& targetPosture ) const
{
    const uint     nFirerCurrentPosture      = firerPosture.GetCurrentPosture              ().GetID();
    const uint     nFirerLastPosture         = firerPosture.GetLastPosture                 ().GetID();
    const MT_Float rFirerPostureCompletion   = firerPosture.GetPostureCompletionPercentage ();
    const uint     nTargetCurrentPosture     = targetPosture.GetCurrentPosture             ().GetID();
    const uint     nTargetLastPosture        = targetPosture.GetLastPosture                ().GetID();
    const MT_Float rTargetPostureCompletion  = targetPosture.GetPostureCompletionPercentage();
      
    const T_PhModificatorValueVector& firerCurrentModificator = phModificators_[ nFirerCurrentPosture ];
    const T_PhModificatorValueVector& firerLastModificator    = phModificators_[ nFirerLastPosture ];
    const MT_Float rInverseFirerPostureCompletion             = 1.f - rFirerPostureCompletion;

    return           rTargetPostureCompletion   * ( rFirerPostureCompletion * firerCurrentModificator[ nTargetCurrentPosture ] + rInverseFirerPostureCompletion * firerLastModificator[ nTargetCurrentPosture ] )
           + ( 1.f - rTargetPostureCompletion ) * ( rFirerPostureCompletion * firerCurrentModificator[ nTargetLastPosture ]    + rInverseFirerPostureCompletion * firerLastModificator[ nTargetLastPosture ] );
}

// -----------------------------------------------------------------------------
// Name: PHY_LauncherType::GetPHModificator
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
MT_Float PHY_LauncherType::GetPHModificator( const PHY_Posture& firerPosture, const PHY_Posture& targetPosture ) const
{
    return phModificators_[ firerPosture.GetID() ][ targetPosture.GetID() ];
}
