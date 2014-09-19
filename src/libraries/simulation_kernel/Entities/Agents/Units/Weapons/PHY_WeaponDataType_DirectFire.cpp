// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Weapons/PHY_WeaponDataType_DirectFire.cpp $
// $Author: Jvt $
// $Modtime: 2/05/05 18:28 $
// $Revision: 12 $
// $Workfile: PHY_WeaponDataType_DirectFire.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_WeaponDataType_DirectFire.h"
#include "PHY_WeaponType.h"
#include "OnComponentComputer_ABC.h"
#include "OnComponentFunctor_ABC.h"
#include "DefaultComponentFunctorComputer.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Actions/Loading/PHY_RoleAction_Loading.h"
#include "Entities/Agents/Roles/Decision/DEC_RolePion_Decision.h"
#include "Entities/Agents/Roles/HumanFactors/PHY_RoleInterface_HumanFactors.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Roles/Perception/PHY_RoleInterface_Perceiver.h"
#include "Entities/Agents/Roles/Posture/PHY_RoleInterface_Posture.h"
#include "Entities/Agents/Roles/Protection/PHY_RoleInterface_ActiveProtection.h"
#include "Entities/Agents/Roles/Urban/PHY_RoleInterface_UrbanLocation.h"
#include "Entities/Agents/Units/Categories/PHY_Protection.h"
#include "Entities/Agents/Units/Categories/PHY_RoePopulation.h"
#include "Entities/Agents/Units/Categories/PHY_Volume.h"
#include "Entities/Agents/Units/Composantes/PHY_Composante_ABC.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposanteType_ABC.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Agents/Units/Postures/PHY_Posture.h"
#include "Entities/Agents/Units/Dotations/PHY_AmmoDotationClass.h"
#include "Entities/Effects/MIL_Effect_DirectFirePion.h"
#include "Entities/Effects/MIL_Effect_DirectFirePopulation.h"
#include "Entities/Effects/MIL_Effect_ScatterPopulation.h"
#include "Entities/Effects/MIL_EffectManager.h"
#include "Entities/Populations/MIL_Population.h"
#include "Entities/Populations/MIL_PopulationConcentration.h"
#include "Entities/Populations/MIL_PopulationFlow.h"
#include "Entities/Populations/MIL_PopulationType.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionLevel.h"
#include "Entities/Agents/Units/Sensors/PHY_Sensor.h"
#include "Entities/Agents/Units/Sensors/PHY_SensorType.h"
#include "Entities/Agents/Units/Sensors/PHY_SensorTypeAgent.h"
#include "Entities/Objects/StructuralCapacity.h"
#include "MIL_Random.h"
#include "Urban/MIL_UrbanObject_ABC.h"
#include "simulation_terrain/TER_PopulationConcentration_ABC.h"
#include "simulation_terrain/TER_PopulationFlow_ABC.h"
#include "simulation_terrain/TER_PopulationManager.h"
#include "simulation_terrain/TER_World.h"
#include "Tools/MIL_Tools.h"

// -----------------------------------------------------------------------------
// Name: PHY_WeaponDataType_DirectFire constructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
PHY_WeaponDataType_DirectFire::PHY_WeaponDataType_DirectFire( const PHY_WeaponType& weaponType, xml::xistream& xis )
    : weaponType_( weaponType )
    , phs_       ( PHY_Volume::GetVolumes().size(), MT_InterpolatedFunction( 0., 0. ) )
{
    xis >> xml::list( "hit-probabilities", *this, &PHY_WeaponDataType_DirectFire::InitializePH );
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponDataType_DirectFire destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
PHY_WeaponDataType_DirectFire::~PHY_WeaponDataType_DirectFire()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponDataType_DirectFire::InitializePH
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void PHY_WeaponDataType_DirectFire::InitializePH( xml::xistream& xis )
{

    const PHY_Volume::T_VolumeMap& volumes = PHY_Volume::GetVolumes();

    std::string targetType;
    xis >> xml::attribute( "target", targetType );

    PHY_Volume::CIT_VolumeMap it = volumes.find( targetType );
    const PHY_Volume& volume = *it->second;

    assert( phs_.size() > volume.GetID() );

    MT_InterpolatedFunction& phFunction = phs_[ volume.GetID() ];

    xis >> xml::list( "hit-probability", *this, &PHY_WeaponDataType_DirectFire::ReadHitProbability, phFunction );
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponDataType_DirectFire::ReadHitProbability
// Created: ABL 2007-07-20
// -----------------------------------------------------------------------------
void PHY_WeaponDataType_DirectFire::ReadHitProbability( xml::xistream& xis, MT_InterpolatedFunction& phFunction )
{
    double rDistance;
    double rPH;

    xis >> xml::attribute( "distance", rDistance )
        >> xml::attribute( "percentage", rPH );

    if( rDistance < 0 )
        throw MASA_EXCEPTION( xis.context() + "hit-probability: distance <  0" );
    if( rPH < 0 || rPH > 1 )
        throw MASA_EXCEPTION( xis.context() + "hit-probability: percentage not in [0..1]" );

    phFunction.AddNewPoint( MIL_Tools::ConvertMeterToSim( rDistance ), rPH );
}

// =============================================================================
// TOOLS
// =============================================================================

//-----------------------------------------------------------------------------
// Name: PHY_WeaponDataType_DirectFire::GetMaxDistanceForPH
// Created: JVT 03-09-19
//-----------------------------------------------------------------------------
double PHY_WeaponDataType_DirectFire::GetMaxDistanceForPH( double rPH, const PHY_Volume& targetVolume ) const
{
    return weaponType_.GetPHModificator() * phs_[ targetVolume.GetID() ].GetMaxYForX( rPH );
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponDataType_DirectFire::GetMinDistanceForPH
// Created: JVT 2004-12-17
// -----------------------------------------------------------------------------
double PHY_WeaponDataType_DirectFire::GetMinDistanceForPH( double rPH, const PHY_Volume& targetVolume ) const
{
    return weaponType_.GetPHModificator() * phs_[ targetVolume.GetID() ].GetMinYForX( rPH );
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponDataType_DirectFire::GetMaxDistanceForPH
// Created: SBO 2006-01-25
// -----------------------------------------------------------------------------
double PHY_WeaponDataType_DirectFire::GetMaxDistanceForPH( double rPH, const PHY_RoleInterface_Posture& firerPosture, const PHY_RoleInterface_Posture& targetPosture, const PHY_Volume& targetVolume ) const
{
    return weaponType_.GetPHModificator( firerPosture, targetPosture ) * phs_[ targetVolume.GetID() ].GetMaxYForX( rPH );
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponDataType_DirectFire::GetMinDistanceForPH
// Created: SBO 2006-01-25
// -----------------------------------------------------------------------------
double PHY_WeaponDataType_DirectFire::GetMinDistanceForPH( double rPH, const PHY_RoleInterface_Posture& firerPosture, const PHY_RoleInterface_Posture& targetPosture, const PHY_Volume& targetVolume ) const
{
    return weaponType_.GetPHModificator( firerPosture, targetPosture ) * phs_[ targetVolume.GetID() ].GetMinYForX( rPH );
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponDataType_DirectFire::GetPH
// Created: NLD 2004-10-05
// Modified: JVT 2004-11-03
// -----------------------------------------------------------------------------
double PHY_WeaponDataType_DirectFire::GetPH( const MIL_Agent_ABC& firer, const MIL_Agent_ABC& target, const PHY_Volume& targetVolume, const MT_Vector3D& /*firerPosition*/, const MT_Vector3D& /*targetPosition*/ ) const
{
    const PHY_RoleInterface_Posture& firerPosture  = firer .GetRole< PHY_RoleInterface_Posture >();
    const PHY_RoleInterface_Posture& targetPosture = target.GetRole< PHY_RoleInterface_Posture >();
    double rDistance = firer.Distance( target );
    assert( phs_.size() > targetVolume.GetID() );

    const double rPHModificator = weaponType_.GetPHModificator( firerPosture, targetPosture );
    if( rPHModificator <= 0. )
        return 0.;
    rDistance /= rPHModificator;

    const double rPH = phs_[ targetVolume.GetID() ]( rDistance );
    const double protection = target.GetRole< PHY_RoleInterface_ActiveProtection >().GetPHModifier( weaponType_.GetDotationCategory() );
    return firer.GetRole< PHY_RoleInterface_HumanFactors >().ModifyPH( rPH * protection );
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponDataType_DirectFire::GetPH
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
double PHY_WeaponDataType_DirectFire::GetPH( const PHY_Volume& targetVolume, double rDistance ) const
{
    assert( phs_.size() > targetVolume.GetID() );

    const double rPHModificator = weaponType_.GetPHModificator();
    if( rPHModificator <= 0. )
        return 0.;
    rDistance /= rPHModificator;
    return phs_[ targetVolume.GetID() ]( rDistance );
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_WeaponDataType_DirectFire::GetMaxRangeToFireOn
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
double PHY_WeaponDataType_DirectFire::GetMaxRangeToFireOn( const PHY_ComposanteType_ABC& targetComposanteType, double rWantedPH ) const
{
    return GetMaxDistanceForPH( rWantedPH, targetComposanteType.GetVolume() );
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponDataType_DirectFire::GetMinRangeToFireOn
// Created: JVT 2004-12-17
// -----------------------------------------------------------------------------
double PHY_WeaponDataType_DirectFire::GetMinRangeToFireOn( const PHY_ComposanteType_ABC& targetComposanteType, double rWantedPH ) const
{
    return GetMinDistanceForPH( rWantedPH, targetComposanteType.GetVolume() );
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponDataType_DirectFire::GetMaxRangeToFireOnWithPosture
// Created: SBO 2006-01-10
// -----------------------------------------------------------------------------
double PHY_WeaponDataType_DirectFire::GetMaxRangeToFireOnWithPosture( const PHY_ComposanteType_ABC& targetComposanteType, const MIL_Agent_ABC& firer, const MIL_Agent_ABC& target, double rWantedPH ) const
{
    const PHY_RoleInterface_Posture&      firerPosture  = firer .GetRole< PHY_RoleInterface_Posture      >();
    const PHY_RoleInterface_Posture& targetPosture = target.GetRole< PHY_RoleInterface_Posture >();

    return GetMaxDistanceForPH( rWantedPH, firerPosture, targetPosture, targetComposanteType.GetVolume() );
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponDataType_DirectFire::GetMinRangeToFireOnWithPosture
// Created: SBO 2006-01-10
// -----------------------------------------------------------------------------
double PHY_WeaponDataType_DirectFire::GetMinRangeToFireOnWithPosture( const PHY_ComposanteType_ABC& targetComposanteType, const MIL_Agent_ABC& firer, const MIL_Agent_ABC& target, double rWantedPH ) const
{
    const PHY_RoleInterface_Posture&      firerPosture  = firer .GetRole< PHY_RoleInterface_Posture      >();
    const PHY_RoleInterface_Posture& targetPosture = target.GetRole< PHY_RoleInterface_Posture >();

    return GetMinDistanceForPH( rWantedPH, firerPosture, targetPosture, targetComposanteType.GetVolume() );
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponDataType_DirectFire::GetMaxRangeToFire
// Created: DDA 2010-05-03
// -----------------------------------------------------------------------------
double PHY_WeaponDataType_DirectFire::GetMaxRangeToFire( double rWantedPH ) const
{
    const PHY_Volume::T_VolumeMap& volumes = PHY_Volume::GetVolumes();

    double result = 0.;
    for( PHY_Volume::CIT_VolumeMap it = volumes.begin(); it != volumes.end(); ++it )
    {
        result = std::max( result, GetMaxDistanceForPH( rWantedPH, *it->second ) );
    }

    return result;
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponDataType_DirectFire::GetMaxRange
// Created: MCO 2012-11-23
// -----------------------------------------------------------------------------
double PHY_WeaponDataType_DirectFire::GetMaxRange() const
{
    double result = 0;
    for( auto it = phs_.begin(); it != phs_.end(); ++it )
        result = std::max( result, it->GetMaxYForX( 0 ) );
    return result;
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponDataType_DirectFire::GetDangerosity
// Created: NLD 2004-10-05
// -----------------------------------------------------------------------------
double PHY_WeaponDataType_DirectFire::GetDangerosity( const MIL_Agent_ABC& firer, const MIL_Agent_ABC& target, const PHY_ComposanteType_ABC& targetComposanteType, bool bUsePH ) const
{
    const PHY_Volume&     targetVolume      = targetComposanteType.GetVolume    ();
    const PHY_Protection& targetProtection  = targetComposanteType.GetProtection();

    const PHY_RoleInterface_Location& firerLocation = firer.GetRole< PHY_RoleInterface_Location >();
    const MT_Vector3D vFirerPosition( firerLocation.GetPosition().rX_, firerLocation.GetPosition().rY_, firerLocation.GetAltitude() );

    const PHY_RoleInterface_Location& targetLocation = target.GetRole< PHY_RoleInterface_Location >();
    const MT_Vector3D vTargetPosition( targetLocation.GetPosition().rX_, targetLocation.GetPosition().rY_, targetLocation.GetAltitude() );

    double rDangerosity  = bUsePH ? GetPH( firer, target, targetVolume, vFirerPosition, vTargetPosition ) : 1.;
             rDangerosity *= weaponType_.GetDotationCategory().GetAttritionScore( targetProtection );
    return rDangerosity;
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponDataType_DirectFire::GetDangerosity
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
double PHY_WeaponDataType_DirectFire::GetDangerosity( const PHY_ComposanteType_ABC& targetComposanteType, double rDistBtwFirerAndTarget, const PHY_MaterialCompositionType* material ) const
{
    const PHY_Volume&     targetVolume      = targetComposanteType.GetVolume    ();
    const PHY_Protection& targetProtection  = targetComposanteType.GetProtection();

    double rDangerosity  = GetPH( targetVolume, rDistBtwFirerAndTarget );
    rDangerosity *= weaponType_.GetDotationCategory().GetAttritionScore( targetProtection );
    if( material )
        rDangerosity *= weaponType_.GetDotationCategory().GetUrbanAttritionScore( *material );
    return rDangerosity;
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponDataType_DirectFire::Fire
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
void PHY_WeaponDataType_DirectFire::Fire( MIL_Agent_ABC& firer, MIL_Agent_ABC& target, PHY_Composante_ABC& compTarget, PHY_FireResults_ABC& fireResult, bool bUsePH ) const
{
    const PHY_RoleInterface_Location& firerLocation = firer.GetRole< PHY_RoleInterface_Location >();
    const PHY_RoleInterface_Location& targetLocation = target.GetRole< PHY_RoleInterface_Location >();
    NotifyFirerPerception( firer, target );

    const MIL_UrbanObject_ABC* urbanObject = target.GetRole< PHY_RoleInterface_UrbanLocation >().GetCurrentUrbanBlock();
    if( urbanObject )
        urbanObject->ApplyDirectFire();

    if( bUsePH )
    {
        const PHY_Volume& targetVolume = compTarget.GetType().GetVolume(); //$$$$ ENCAPSULER

        const MT_Vector3D firerPosition( firerLocation.GetPosition().rX_, firerLocation.GetPosition().rY_, firerLocation.GetAltitude() );
        const MT_Vector3D targetPosition( targetLocation.GetPosition().rX_, targetLocation.GetPosition().rY_, targetLocation.GetAltitude() );

        const double rPH = GetPH( firer, target, targetVolume, firerPosition, targetPosition );
        target.GetRole< PHY_RoleInterface_ActiveProtection >().UseAmmunition( weaponType_.GetDotationCategory() );
        if( 1. - MIL_Random::rand_io( MIL_Random::eFire ) > rPH )
        {
            if( urbanObject )
            {
                StructuralCapacity* capacity = const_cast< StructuralCapacity* >( urbanObject->Retrieve< StructuralCapacity >() );
                if( capacity )
                    capacity->ApplyDirectFire( *urbanObject, weaponType_.GetDotationCategory() );
            }
            return;
        }
    }

    MIL_Effect_DirectFirePion* pEffect = new MIL_Effect_DirectFirePion( weaponType_.GetDotationCategory(), target, compTarget, fireResult );
    MIL_EffectManager::GetEffectManager().Register( *pEffect );

    // handle direct-indirect fire on populations
    const MT_Vector2D firerPosition ( firerLocation.GetPosition().rX_, firerLocation.GetPosition().rY_ );
    const MT_Vector2D targetPosition( targetLocation.GetPosition().rX_, targetLocation.GetPosition().rY_ );

    TER_PopulationConcentration_ABC::T_PopulationConcentrationVector concentrations;
    TER_World::GetWorld().GetPopulationManager().GetConcentrationManager()
                         .GetListIntersectingLine( firerPosition, targetPosition, concentrations );
    for( TER_PopulationConcentration_ABC::CIT_PopulationConcentrationVector itConcentration = concentrations.begin();
        itConcentration != concentrations.end(); ++itConcentration )
    {
        MIL_PopulationConcentration* pElement = static_cast< MIL_PopulationConcentration* >( *itConcentration );
        MIL_Effect_DirectFirePopulation* pEffect = new MIL_Effect_DirectFirePopulation( *pElement, 1, fireResult );
        MIL_EffectManager::GetEffectManager().Register( *pEffect );
    }

    TER_PopulationFlow_ABC::T_PopulationFlowVector flows;
    TER_World::GetWorld().GetPopulationManager().GetFlowManager()
                         .GetListIntersectingLine( firerPosition, targetPosition, flows );
    for( TER_PopulationFlow_ABC::CIT_PopulationFlowVector itFlow = flows.begin(); itFlow != flows.end(); ++itFlow )
    {
        MIL_PopulationFlow* pElement = static_cast< MIL_PopulationFlow* >( *itFlow );
        MIL_Effect_DirectFirePopulation* pEffect = new MIL_Effect_DirectFirePopulation( *pElement, 1, fireResult );
        MIL_EffectManager::GetEffectManager().Register( *pEffect );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponDataType_DirectFire::Fire
// Created: NLD 2005-11-16
// -----------------------------------------------------------------------------
void PHY_WeaponDataType_DirectFire::Fire( MIL_Agent_ABC& firer, MIL_PopulationElement_ABC& target, unsigned int nNbrAmmoReserved, PHY_FireResults_ABC& fireResult, const PHY_AmmoDotationClass* dotationClass ) const
{
    const PHY_RoePopulation& roe = firer.GetRole< DEC_RolePion_Decision >().GetRoePopulation();
    const double rPH = target.GetPopulation().GetType().GetDamagePH( roe );

    unsigned int nHit = 0;
    for( unsigned int i = 1; i <= nNbrAmmoReserved; ++i )
        if( 1. - MIL_Random::rand_io() <= rPH )
            ++nHit;

    MIL_Effect_ABC* pEffect = 0;
    if( &PHY_AmmoDotationClass::alr_ == dotationClass )
        pEffect = new MIL_Effect_ScatterPopulation( target, nHit, fireResult );
    else
        pEffect = new MIL_Effect_DirectFirePopulation( target, nHit, fireResult );
    MIL_EffectManager::GetEffectManager().Register( *pEffect );
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponDataType_DirectFire::NotifyFirerPerception
// Created: SLG 2010-05-20
// -----------------------------------------------------------------------------
void PHY_WeaponDataType_DirectFire::NotifyFirerPerception( MIL_Agent_ABC& firer, MIL_Agent_ABC& target ) const
{
    if( IsFirerInsideRecognitionDistance( firer, target ) )
    {
        PHY_RoleInterface_Perceiver& role = target.GetRole< PHY_RoleInterface_Perceiver >();
        role.NotifyExternalPerception( firer, PHY_PerceptionLevel::recognized_ );
    }
}

namespace
{
    class SensorFunctor : private boost::noncopyable
    {
    public:
        SensorFunctor(const MIL_Agent_ABC& perceiver, const MIL_Agent_ABC& target )
            : perceiver_( perceiver ), target_( target ), isInside_( false )
        {}
        ~SensorFunctor()
        {}

        void operator() ( const PHY_Sensor& sensor )
        {
            const PHY_SensorTypeAgent* sensorTypeAgent = sensor.GetType().GetTypeAgent();
            if( sensorTypeAgent )
            {
                MT_Vector2D targetPosition = target_.GetRole< PHY_RoleInterface_Location >().GetPosition();
                MT_Vector2D perceiverPosition = perceiver_.GetRole< PHY_RoleInterface_Location >().GetPosition();
                isInside_ = sensorTypeAgent->CanDetectFirer( targetPosition.Distance( perceiverPosition ) );
            }
        }
        bool IsInside(){ return isInside_; }
    private:
        const MIL_Agent_ABC& perceiver_;
        const MIL_Agent_ABC& target_;
        bool                 isInside_;
    };

    class Functor : public OnComponentFunctor_ABC
    {
    public:
        Functor( const MIL_Agent_ABC& perceiver, const MIL_Agent_ABC& target )
            : perceiver_( perceiver )
            , transport_( perceiver.RetrieveRole< transport::PHY_RoleAction_Loading >() )
            , target_( target )
            , isInside_( false )
        {}
        ~Functor()
        {}

        void operator() ( PHY_ComposantePion& composante )
        {
            if( !composante.CanPerceive( transport_ ) )
                return;
            SensorFunctor dataFunctor( perceiver_, target_ );
            composante.ApplyOnSensors( dataFunctor );
            if( !isInside_)
                isInside_ = dataFunctor.IsInside();

        }
        bool IsInside(){ return isInside_; }

    private:
        const MIL_Agent_ABC& perceiver_;
        const transport::PHY_RoleAction_Loading* transport_;
        const MIL_Agent_ABC& target_;
        bool isInside_;
    };
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponDataType_DirectFire::IsFirerInsideRecognitionDistance
// Created: SLG 2010-05-20
// -----------------------------------------------------------------------------
bool PHY_WeaponDataType_DirectFire::IsFirerInsideRecognitionDistance( MIL_Agent_ABC& firer, MIL_Agent_ABC& target ) const
{
    Functor dataFunctor( target, firer );
    DefaultComponentFunctorComputer dataComputer( dataFunctor );
    target.Execute< OnComponentComputer_ABC >( dataComputer );
    return dataFunctor.IsInside();
}
