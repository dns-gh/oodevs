// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Units/Dotations/PHY_DotationCategory_IndirectMineFire.cpp $
// $Author: Nld $
// $Modtime: 17/03/05 14:53 $
// $Revision: 4 $
// $Workfile: PHY_DotationCategory_IndirectMineFire.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"

#include "PHY_DotationCategory_IndirectMineFire.h"
#include "MIL_AgentServer.h"
#include "Entities/MIL_EntityManager.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Objects/MIL_RealObjectType.h"
#include "Entities/Objects/MIL_ZoneMineeParDispersion.h"
#include "TER/TER_Localisation.h"

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory_IndirectFire::Create
// Created: NLD 2004-10-08
// -----------------------------------------------------------------------------
PHY_DotationCategory_IndirectFire_ABC& PHY_DotationCategory_IndirectMineFire::Create( const PHY_IndirectFireDotationClass& type, const PHY_DotationCategory& dotationCategory, MIL_InputArchive& archive )
{
    return *new PHY_DotationCategory_IndirectMineFire( type, dotationCategory, archive );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory_IndirectMineFire constructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
PHY_DotationCategory_IndirectMineFire::PHY_DotationCategory_IndirectMineFire( const PHY_IndirectFireDotationClass& type, const PHY_DotationCategory& dotationCategory, MIL_InputArchive& archive )
    : PHY_DotationCategory_IndirectFire_ABC( type, dotationCategory, archive )
{
    archive.ReadField( "NombreMines", nNbrObjects_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory_IndirectMineFire destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
PHY_DotationCategory_IndirectMineFire::~PHY_DotationCategory_IndirectMineFire()
{

}

// =============================================================================
// OPERATIONS
// =============================================================================
  
// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory_IndirectMineFire::Fire
// Created: NLD 2004-10-12
// -----------------------------------------------------------------------------
void PHY_DotationCategory_IndirectMineFire::Fire( const MIL_AgentPion& firer, const MT_Vector2D& vSourcePosition, const MT_Vector2D& vTargetPosition, MT_Float rInterventionTypeFired, PHY_FireResults_ABC& /*fireResult*/ ) const
{
    MT_Vector2D vFireDirection( 0., 1. );
    if( vTargetPosition != vSourcePosition )
        vFireDirection = ( vTargetPosition - vSourcePosition ).Normalize();
    MT_Vector2D vRotatedFireDirection = vFireDirection;
    vRotatedFireDirection.Rotate90();

    vFireDirection        *= ( rInterventionTypeFired * rDispersionX_ );
    vRotatedFireDirection *= ( rInterventionTypeFired * rDispersionY_ );

    const uint nNbrMines = (uint)( nNbrObjects_ * rInterventionTypeFired );

    T_PointVector points; points.reserve( 3 );
    points.push_back( vTargetPosition                         );
    points.push_back( vTargetPosition + vFireDirection        );
    points.push_back( vTargetPosition + vRotatedFireDirection );
    const TER_Localisation localisation( TER_Localisation::eEllipse, points );

    MIL_ZoneMineeParDispersion& zoneMinee = *new MIL_ZoneMineeParDispersion();
    zoneMinee.Initialize( firer.GetArmy(), localisation, nNbrMines );
    zoneMinee.Construct();
    MIL_AgentServer::GetWorkspace().GetEntityManager().RegisterObject( zoneMinee );
}
