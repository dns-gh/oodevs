// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_DotationCategory_IndirectObjectCreationFire.h"
#include "Entities/MIL_EntityManager.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Entities/Objects/ConstructionAttribute.h"
#include "Entities/Objects/TimeLimitedAttribute.h"
#include "Entities/Objects/TimeLimitedCapacity.h"
#include "simulation_terrain/TER_Localisation.h"
#include "tools/Codec.h"
#include "MT_Tools/MT_Logger.h"

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory_IndirectObjectCreationFire::Create
// Created: LDC 2009-12-30
// -----------------------------------------------------------------------------
PHY_DotationCategory_IndirectFire_ABC& PHY_DotationCategory_IndirectObjectCreationFire::Create( const PHY_IndirectFireDotationClass& type, const PHY_DotationCategory& dotationCategory, xml::xistream& xis,
                                                                                                unsigned int nInterventionType, double rDispersionX, double rDispersionY, double rDetectionRange )
{
    return *new PHY_DotationCategory_IndirectObjectCreationFire( type, dotationCategory, xis, nInterventionType, rDispersionX, rDispersionY, rDetectionRange );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory_IndirectObjectCreationFire constructor
// Created: LDC 2009-12-30
// -----------------------------------------------------------------------------
PHY_DotationCategory_IndirectObjectCreationFire::PHY_DotationCategory_IndirectObjectCreationFire( const PHY_IndirectFireDotationClass& type, const PHY_DotationCategory& dotationCategory, xml::xistream& xis,
                                                                                                  unsigned int nInterventionType, double rDispersionX, double rDispersionY, double rDetectionRange )
    : PHY_DotationCategory_IndirectFire_ABC( type, dotationCategory, nInterventionType, rDispersionX, rDispersionY, rDetectionRange )
{
    std::string lifeTime;
    xis >> xml::attribute( "object-type", objectType_ )
        >> xml::attribute( "life-time", lifeTime );

    if( ! tools::DecodeTime( lifeTime, nLifeDuration_ ) || nLifeDuration_ < 0 )
      throw MASA_EXCEPTION( xis.context() + "indirect-fire: life-time < 0" );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory_IndirectObjectCreationFire constructor
// Created: LDC 2009-12-30
// -----------------------------------------------------------------------------
PHY_DotationCategory_IndirectObjectCreationFire::PHY_DotationCategory_IndirectObjectCreationFire( const PHY_IndirectFireDotationClass& type, const PHY_DotationCategory& dotationCategory, const std::string& objectType,
                                                                                                  unsigned int nInterventionType, double rDispersionX, double rDispersionY, double rDetectionRange )
    : PHY_DotationCategory_IndirectFire_ABC( type, dotationCategory, nInterventionType, rDispersionX, rDispersionY, rDetectionRange )
    , objectType_( objectType )
    , nLifeDuration_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory_IndirectObjectCreationFire destructor
// Created: LDC 2009-12-30
// -----------------------------------------------------------------------------
PHY_DotationCategory_IndirectObjectCreationFire::~PHY_DotationCategory_IndirectObjectCreationFire()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory_IndirectObjectCreationFire::ApplyEffect
// Created: LDC 2009-12-30
// -----------------------------------------------------------------------------
void PHY_DotationCategory_IndirectObjectCreationFire::ApplyEffect( const MIL_Agent_ABC* pFirer, const MT_Vector2D& vSourcePosition, const MT_Vector2D& vTargetPosition, double rInterventionTypeFired, PHY_FireResults_ABC& /*fireResult*/ ) const
{
    MT_Vector2D vFireDirection( 0., 1. );
    if( vTargetPosition != vSourcePosition )
        vFireDirection = ( vTargetPosition - vSourcePosition ).Normalize();
    MT_Vector2D vRotatedFireDirection = vFireDirection;
    vRotatedFireDirection.Rotate90();

    vFireDirection        *= ( rInterventionTypeFired * rDispersionX_ );
    vRotatedFireDirection *= ( rInterventionTypeFired * rDispersionY_ );

    T_PointVector points; points.reserve( 3 );
    points.push_back( vTargetPosition                         );
    points.push_back( vTargetPosition + vFireDirection        );
    points.push_back( vTargetPosition + vRotatedFireDirection );
    const TER_Localisation localisation( TER_Localisation::eEllipse, points );

    try
    {
        MIL_Object_ABC* pObject = MIL_EntityManager_ABC::GetSingleton().CreateObject( objectType_, pFirer ? &pFirer->GetArmy() : 0, localisation ); // $$$$ _RC_ SLI 2012-12-07: only for TestScramblingAmmo
        ConstructionAttribute* pAttribute = pObject->RetrieveAttribute< ConstructionAttribute >();
        if( pAttribute )
            pAttribute->Build( 1. );
        pObject->GetAttribute< TimeLimitedAttribute >() = TimeLimitedAttribute( nLifeDuration_ );
    }
    catch( const std::exception& )
    {
        MT_LOG_ERROR_MSG( "Création de l'objet de type" << objectType_ << "impossible" );
    }
}
