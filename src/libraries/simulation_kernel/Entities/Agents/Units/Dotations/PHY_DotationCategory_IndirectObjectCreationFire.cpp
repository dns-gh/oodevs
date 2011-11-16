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
#include "MIL_AgentServer.h"
#include "MIL_Singletons.h"
#include "Entities/MIL_EntityManager.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Entities/Objects/ConstructionAttribute.h"
#include "Entities/Objects/TimeLimitedAttribute.h"
#include "Entities/Objects/TimeLimitedCapacity.h"
#include "simulation_terrain/TER_Localisation.h"
#include "tools/xmlcodecs.h"
#include "MT_Tools/MT_Logger.h"
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory_IndirectObjectCreationFire::Create
// Created: LDC 2009-12-30
// -----------------------------------------------------------------------------
PHY_DotationCategory_IndirectFire_ABC& PHY_DotationCategory_IndirectObjectCreationFire::Create( const PHY_IndirectFireDotationClass& type, const PHY_DotationCategory& dotationCategory, xml::xistream& xis )
{
    return *new PHY_DotationCategory_IndirectObjectCreationFire( type, dotationCategory, xis );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory_IndirectObjectCreationFire constructor
// Created: LDC 2009-12-30
// -----------------------------------------------------------------------------
PHY_DotationCategory_IndirectObjectCreationFire::PHY_DotationCategory_IndirectObjectCreationFire( const PHY_IndirectFireDotationClass& type, const PHY_DotationCategory& dotationCategory, xml::xistream& xis )
    : PHY_DotationCategory_IndirectFire_ABC( type, dotationCategory, xis )
{
    std::string lifeTime;
    xis >> xml::attribute( "object-type", objectType_ )
        >> xml::attribute( "life-time", lifeTime );

    if( ! tools::DecodeTime( lifeTime, nLifeDuration_ ) || nLifeDuration_ < 0 )
      xis.error( "indirect-fire: life-time < 0" );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory_IndirectObjectCreationFire constructor
// Created: LDC 2009-12-30
// -----------------------------------------------------------------------------
PHY_DotationCategory_IndirectObjectCreationFire::PHY_DotationCategory_IndirectObjectCreationFire( const PHY_IndirectFireDotationClass& type, const PHY_DotationCategory& dotationCategory, xml::xistream& xis, const std::string& objectType )
    : PHY_DotationCategory_IndirectFire_ABC( type, dotationCategory, xis )
    , objectType_   ( objectType )
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
        MIL_Object_ABC* pObject = MIL_Singletons::GetEntityManager().CreateObject( objectType_, pFirer ? &pFirer->GetArmy() : 0, localisation );
        ConstructionAttribute* pAttribute = pObject->RetrieveAttribute< ConstructionAttribute >();
        if( pAttribute )
            pAttribute->Build( 1. );
        pObject->GetAttribute< TimeLimitedAttribute >() = TimeLimitedAttribute( nLifeDuration_ );
    }
    catch( std::runtime_error& )
    {
        MT_LOG_ERROR_MSG( "Création de l'objet de type" << objectType_ << "impossible" );
    }
}
