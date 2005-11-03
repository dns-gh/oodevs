// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2004-11-08 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Composantes/PHY_RoleHLA_Composantes.cpp $
// $Author: Age $
// $Modtime: 21/02/05 10:02 $
// $Revision: 12 $
// $Workfile: PHY_RoleHLA_Composantes.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"
#include "PHY_RoleHLA_Composantes.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposanteHLA.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposanteTypePion.h"
#include "Entities/Agents/Units/Sensors/PHY_SensorTypeAgent.h"
#include "Entities/Agents/Roles/hla/HLA_RoleInterface.h"

#include "Hla/HLA_Serialization.h"
#include "Hla/HLA_DirectFire.h"
#include "Hla/HLA_IndirectFire.h"
#include "Hla/HLA_InteractionManager_ABC.h"
#include "Hla/HLA_Neutralization.h"

#include "hla/AttributeIdentifier.h"

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Composantes constructor
// Created: AGE 2004-11-08
// -----------------------------------------------------------------------------
PHY_RoleHLA_Composantes::PHY_RoleHLA_Composantes( MT_RoleContainer& role, HLA_InteractionManager_ABC& interactionManager )
    : PHY_RoleInterface_Composantes( role )
    , interactionManager_( interactionManager )
    , bNeutralized_( false )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Composantes destructor
// Created: AGE 2004-11-08
// -----------------------------------------------------------------------------
PHY_RoleHLA_Composantes::~PHY_RoleHLA_Composantes()
{
    for( CIT_Composantes it = composantes_.begin(); it != composantes_.end(); ++it )
        delete *it;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Composantes::GetSignificantVolume
// Created: AGE 2004-11-08
// -----------------------------------------------------------------------------
const PHY_Volume* PHY_RoleHLA_Composantes::GetSignificantVolume( const PHY_SensorTypeAgent& sensorType ) const
{
    const PHY_Volume* pSignificantVolume = 0;
    MT_Float rSignificantVolumeFactor = 0.;
    for( CIT_Composantes it = composantes_.begin(); it != composantes_.end(); ++it )
    {
        assert( *it );
        const PHY_ComposanteHLA& composante = **it;
        const PHY_Volume& compTypeVolume = composante.GetType().GetVolume();
        const MT_Float rVolumeFactor = sensorType.GetFactor( compTypeVolume );
        if( rVolumeFactor > rSignificantVolumeFactor )
        {
            pSignificantVolume = &compTypeVolume;
            rSignificantVolumeFactor =  rVolumeFactor;
        }
    }
    return pSignificantVolume;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Composantes::GetVisibleVolumes
// Created: AGE 2004-11-08
// -----------------------------------------------------------------------------
void PHY_RoleHLA_Composantes::GetVisibleVolumes( T_ComposanteVolumeSet& volumes ) const
{
    volumes.clear();
    for( CIT_Composantes it = composantes_.begin(); it != composantes_.end(); ++it )
    {
        assert( *it );
        const PHY_ComposanteHLA& composante = **it;
        const PHY_Volume& compTypeVolume = composante.GetType().GetVolume();
        volumes.insert( &compTypeVolume );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Composantes::BuildKnowledgeComposantes
// Created: AGE 2004-11-08
// -----------------------------------------------------------------------------
void PHY_RoleHLA_Composantes::BuildKnowledgeComposantes( T_KnowledgeComposanteVector& knowledge ) const
{
    for( CIT_Composantes it = composantes_.begin(); it != composantes_.end(); ++it )
    {
        assert( *it );
        const PHY_ComposanteHLA& composante = **it;
        knowledge.push_back( DEC_Knowledge_AgentComposante( composante ) );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Composantes::GetMajorComposante
// Created: AGE 2004-11-08
// -----------------------------------------------------------------------------
const PHY_Composante_ABC* PHY_RoleHLA_Composantes::GetMajorComposante() const
{
    // $$$$ AGE 2004-12-10: Sender-side algorithm to set the major composante...
    for( CIT_Composantes it = composantes_.begin(); it != composantes_.end(); ++it )
    {
        assert( *it );
        const PHY_ComposanteHLA& composante = **it;
        if( composante.IsActive() )
            return &composante;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Composantes::Deserialize
// Created: AGE 2004-11-10
// -----------------------------------------------------------------------------
void PHY_RoleHLA_Composantes::Deserialize( const AttributeIdentifier& attributeID, Deserializer deserializer )
{
    if( attributeID == "composantes" )
    {
        typedef std::pair< std::string, uint32 > T_Composante;
        std::vector< T_Composante > composantes;
        deserializer >> composantes;
        for( std::vector< T_Composante >::const_iterator it = composantes.begin(); it != composantes.end(); ++it )
        {
            const std::string& strComposanteName = it->first;
            uint nComposantes = it->second;
            UpdateComposante( strComposanteName, nComposantes );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Composantes::UpdateComposante
// Created: AGE 2004-11-22
// -----------------------------------------------------------------------------
void PHY_RoleHLA_Composantes::UpdateComposante( const std::string& strComposanteName, uint nComposantes )
{
    for( CIT_Composantes it = composantes_.begin(); it != composantes_.end(); ++it )
    {
        assert( *it );
        PHY_ComposanteHLA& composante = **it;
        if( composante.GetType().GetName() == strComposanteName )
        {
            composante.Update( nComposantes );
            return;
        }
    }
    const PHY_ComposanteType_ABC* pComposante = PHY_ComposanteTypePion::FindComposanteType( strComposanteName );
    if( pComposante )
        composantes_.push_back( new PHY_ComposanteHLA( *pComposante, nComposantes ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Composantes::Destroy
// Created: AGE 2004-11-10
// -----------------------------------------------------------------------------
void PHY_RoleHLA_Composantes::Destroy()
{
    for( CIT_Composantes it = composantes_.begin(); it != composantes_.end(); ++it )
    {
        assert( *it );
        PHY_ComposanteHLA& composante = **it;
        composante.Update( 0 );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Composantes::GetComposantesAbleToBeFired
// Created: AGE 2004-11-08
// -----------------------------------------------------------------------------
void PHY_RoleHLA_Composantes::GetComposantesAbleToBeFired( T_ComposanteVector& targets ) const
{
    targets.clear();
    for( CIT_Composantes it = composantes_.begin(); it != composantes_.end(); ++it )
    {
        assert( *it );
        PHY_ComposanteHLA& composante = **it;
        if( composante.IsActive() )
            targets.push_back( &composante );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Composantes::GetComposantesAbleToBeFired
// Created: AGE 2004-11-08
// -----------------------------------------------------------------------------
void PHY_RoleHLA_Composantes::GetComposantesAbleToBeFired( T_ComposanteVector& targets, uint nNbrFirer ) const
{
    targets.clear();
    T_ComposanteVector availableTargets;
    GetComposantesAbleToBeFired( availableTargets );
    if( availableTargets.empty() )
        return;
    while( targets.size() < nNbrFirer )
    {
        std::random_shuffle( availableTargets.begin(), availableTargets.end() );
        std::copy( availableTargets.begin(), availableTargets.end(), std::back_inserter( targets ) );
    }
    targets.resize( nNbrFirer );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Composantes::GetEtatOps
// Created: AGE 2004-12-13
// -----------------------------------------------------------------------------
float PHY_RoleHLA_Composantes::GetEtatOps() const
{
    unsigned int nCurrentElements = 0;
    unsigned int nMaxElements = 0;
    for( CIT_Composantes it = composantes_.begin(); it != composantes_.end(); ++it )
    {
        assert( *it );
        PHY_ComposanteHLA& composante = **it;
        nCurrentElements += composante.CurrentElements();
        nMaxElements     += composante.MaxElements();
    }
    if( nMaxElements > 0 )
        return static_cast< float >( nCurrentElements ) / static_cast< float >( nMaxElements );
    return 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Composantes::IsNeutralized
// Created: AGE 2004-11-22
// -----------------------------------------------------------------------------
bool PHY_RoleHLA_Composantes::IsNeutralized() const
{
    return bNeutralized_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Composantes::ChangeStatus
// Created: AGE 2004-12-02
// -----------------------------------------------------------------------------
void PHY_RoleHLA_Composantes::ChangeStatus( const std::vector< std::string >& statuses )
{
    bNeutralized_ = std::find( statuses.begin(), statuses.end(), "neutralise" ) != statuses.end();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Composantes::Neutralize
// Created: AGE 2004-11-08
// -----------------------------------------------------------------------------
void PHY_RoleHLA_Composantes::Neutralize()
{
    interactionManager_.Send( HLA_Neutralization( GetRole< HLA_RoleInterface >() ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Composantes::ApplyDirectFire
// Created: AGE 2004-11-08
// -----------------------------------------------------------------------------
void PHY_RoleHLA_Composantes::ApplyDirectFire( PHY_Composante_ABC& compTarget, const PHY_DotationCategory& dotationCategory, PHY_FireResults_ABC& /*result*/ )
{
    interactionManager_.Send( HLA_DirectFire( GetRole< HLA_RoleInterface >(), compTarget, dotationCategory ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Composantes::ApplyIndirectFire
// Created: AGE 2004-11-08
// -----------------------------------------------------------------------------
void PHY_RoleHLA_Composantes::ApplyIndirectFire( const PHY_DotationCategory& dotationCategory, PHY_FireResults_ABC& /*result*/ )
{
    interactionManager_.Send( HLA_IndirectFire( GetRole< HLA_RoleInterface >(), dotationCategory ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Composantes::ApplyPopulationFire
// Created: NLD 2005-11-03
// -----------------------------------------------------------------------------
void PHY_RoleHLA_Composantes::ApplyPopulationFire( PHY_Composante_ABC& compTarget, const MIL_PopulationType& populationType, const MIL_PopulationAttitude& populationAttitude, PHY_FireResults_ABC& result )
{
    // TODO $$$ HLA
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Composantes::ApplyExplosion
// Created: AGE 2004-11-08
// -----------------------------------------------------------------------------
void PHY_RoleHLA_Composantes::ApplyExplosion( const MIL_RealObjectType& /*objectType*/, PHY_FireResults_ABC& /*result*/ )
{
    // NOTHING
}
