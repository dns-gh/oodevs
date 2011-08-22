// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Weapons/PHY_UrbanAttritionData.cpp $
// $Author: Jvt $
// $Modtime: 19/10/04 18:30 $
// $Revision: 2 $
// $Workfile: PHY_UrbanAttritionData.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_UrbanAttritionData.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposanteState.h"
#include "PHY_MaterialCompositionType.h"
#include <xeumeuleu/xml.hpp>

PHY_UrbanAttritionData::T_UrbanAttritionVector PHY_UrbanAttritionData::globalAttritionFactors_;

// -----------------------------------------------------------------------------
// Name: PHY_UrbanAttritionData constructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
PHY_UrbanAttritionData::PHY_UrbanAttritionData( xml::xistream& xis )
    : attritionFactors_ ( PHY_MaterialCompositionType::Count(), 0. )
{
    xis >> xml::list( "urban-modifier", *this, &PHY_UrbanAttritionData::ReadModifier );
}

// -----------------------------------------------------------------------------
// Name: PHY_UrbanAttritionData constructor
// Created: JCR 2011-08-12
// -----------------------------------------------------------------------------
PHY_UrbanAttritionData::PHY_UrbanAttritionData( const PHY_UrbanAttritionData& data )
    : attritionFactors_ ( data.attritionFactors_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_UrbanAttritionData destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
PHY_UrbanAttritionData::~PHY_UrbanAttritionData()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_UrbanAttritionData::Init
// Created: JCR 2011-08-12
// -----------------------------------------------------------------------------
// static
void PHY_UrbanAttritionData::Init()
{
    if( globalAttritionFactors_.size() != PHY_MaterialCompositionType::Count() )
    {
        T_UrbanAttritionVector vector( PHY_MaterialCompositionType::Count(), 0. );
        globalAttritionFactors_.swap( vector );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_UrbanAttritionData::ReadModifier
// Created: JCR 2011-08-12
// -----------------------------------------------------------------------------
void PHY_UrbanAttritionData::ReadModifier( xml::xistream& xis )
{
    double rFactor;
    std::string materialType;
    xis >> xml::attribute( "material-type", materialType )
        >> xml::attribute( "value", rFactor );

    if( rFactor < 0 || rFactor > 1 )
        xis.error( "urbanBlock-modifier: value not in [0..1]" );
    const PHY_MaterialCompositionType* material = PHY_MaterialCompositionType::Find( materialType );
    if( !material || static_cast< int >( attritionFactors_.size() ) < material->GetId() )
        throw std::runtime_error( "Error when loading material type: " + materialType );
    attritionFactors_[ material->GetId() ] = rFactor;
}

// -----------------------------------------------------------------------------
// Name: PHY_UrbanAttritionData::UpdateGlobal
// Created: JCR 2011-08-12
// -----------------------------------------------------------------------------
void PHY_UrbanAttritionData::UpdateGlobalScore()
{
    PHY_UrbanAttritionData::Init();
    for ( unsigned int i = 0; i < attritionFactors_.size(); ++i )
        globalAttritionFactors_[ i ] = std::max( globalAttritionFactors_[ i ], attritionFactors_[ i ] );
}

// -----------------------------------------------------------------------------
// Name: PHY_UrbanAttritionData::GetScore
// Created: JCR 2011-08-12
// -----------------------------------------------------------------------------
double PHY_UrbanAttritionData::GetScore( const PHY_MaterialCompositionType& material ) const
{
    if( static_cast< int >( attritionFactors_.size() ) < material.GetId() )
        throw std::runtime_error( "Unregistered material type: " + material.GetName() );
    return attritionFactors_[ material.GetId() ];
}

// -----------------------------------------------------------------------------
// Name: PHY_UrbanAttritionData::GetGlobalScore
// Created: JCR 2011-08-12
// -----------------------------------------------------------------------------
double PHY_UrbanAttritionData::GetGlobalScore( const PHY_MaterialCompositionType& material )
{
    if( static_cast< int >( globalAttritionFactors_.size() ) < material.GetId() )
        throw std::runtime_error( "Unregistered material type: " + material.GetName() );
    return globalAttritionFactors_[ material.GetId() ];
}
