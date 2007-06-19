// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "ObjectType.h"
#include "GlTools_ABC.h"
#include "Viewport_ABC.h"
#include "objecticons.h"
#include "xeumeuleu/xml.h"

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: ObjectType constructor
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
ObjectType::ObjectType( xistream& xis, unsigned long id )
    : id_( id )
{
    xpm_ = TypeIcon();
    std::string name;
    xis >> attribute( "type", name )
        >> content( "PeutEtreObstacleDeManoeuvre", canBeReservedObstacle_ )
        >> content( "PeutEtreValorise", canBeValorized_ )
        >> content( "PeutEtreContourne", canBeBypassed_ );
    name_ = name.c_str();
}

// -----------------------------------------------------------------------------
// Name: ObjectType destructor
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
ObjectType::~ObjectType()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectType::GetName
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
QString ObjectType::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: ObjectType::CanBeReservedObstacle
// Created: AGE 2006-10-17
// -----------------------------------------------------------------------------
bool ObjectType::CanBeReservedObstacle() const
{
    return canBeReservedObstacle_;
}

// -----------------------------------------------------------------------------
// Name: ObjectType::CanBeValorized
// Created: AGE 2006-10-17
// -----------------------------------------------------------------------------
bool ObjectType::CanBeValorized() const
{
    return canBeValorized_;
}

// -----------------------------------------------------------------------------
// Name: ObjectType::CanBeBypassed
// Created: AGE 2006-10-17
// -----------------------------------------------------------------------------
bool ObjectType::CanBeBypassed() const
{
    return canBeBypassed_;
}

// -----------------------------------------------------------------------------
// Name: ObjectType::Draw
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
void ObjectType::Draw( const geometry::Point2f& where, const Viewport_ABC& viewport, const GlTools_ABC& tools ) const
{
    if( viewport.IsVisible( where ) )
        tools.DrawIcon( xpm_, where, 900.f );
}

// -----------------------------------------------------------------------------
// Name: ObjectType::TypeIcon
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
const char** ObjectType::TypeIcon()
{
    switch( id_ )
    {
        case eObjectType_ZoneProtegee           : return xpm_objectTypeZoneProtegee ;
        case eObjectType_ZonePoserHelicoptere   : return xpm_objectTypeZonePoserHelicoptere;
        case eObjectType_ZoneNbc                : return xpm_objectTypeZoneNBC;
        case eObjectType_ZoneMobiliteAmelioree  : return xpm_objectTypeZoneMobiliteAmelioree;
        case eObjectType_ZoneInterditeTir       : return xpm_objectTypeZoneInterditeTir;
        case eObjectType_ZoneInterditeMouvement : return xpm_objectTypeZoneInterditeMvt;
        case eObjectType_ZoneImplantationLrm    : return xpm_objectTypeZoneImplantationLrm ;
        case eObjectType_ZoneImplantationCobra  : return xpm_objectTypeZoneImplantationCobra;
        case eObjectType_ZoneImplantationCanon  : return xpm_objectTypeZoneImplantationCanon;
        case eObjectType_ZoneBrouillageBrod     : return xpm_objectTypeZoneBrouillardBrod ;
        case eObjectType_ZoneBrouillageBromure  : return xpm_objectTypeZoneBrouillageBromure;
        case eObjectType_TerrainLargage         : return xpm_objectTypeTerrainLargage;
        case eObjectType_SiteFranchissement     : return xpm_objectTypeSiteFranchissement;
        case eObjectType_SiteDecontamination    : return xpm_objectTypeSiteDecontamination;
        case eObjectType_Rota                   : return xpm_objectTypeRota ;
        case eObjectType_PosteTir               : return xpm_objectTypePosteTir;
        case eObjectType_PosteControle          : return xpm_objectTypePosteControle;
        case eObjectType_PontFlottantContinu    : 
        case eObjectType_PontFlottantDiscontinu : return xpm_objectTypePontFlottant;
        case eObjectType_PlotRavitaillement     : return xpm_objectTypePlotRavitaillement;
        case eObjectType_Plateforme             : return xpm_objectTypePlateforme;
        case eObjectType_Piste                  : return xpm_objectTypePiste;
        case eObjectType_NuageNbc               : return xpm_objectTypeNuageNBC;
        case eObjectType_BouchonMines           : 
        case eObjectType_ZoneMineeLineaire      : 
        case eObjectType_ZoneMineeParDispersion : return xpm_objectTypeMine;
        case eObjectType_ItineraireLogistique   : return xpm_objectTypeItineraireLog;
        case eObjectType_ZoneImplantationMortier: return xpm_objectTypeImplantationMortier;
        case eObjectType_FosseAntiChar          : return xpm_objectTypeFosseAC;
        case eObjectType_Eboulement             : return xpm_objectTypeEboulement;
        case eObjectType_DestructionRoute       : return xpm_objectTypeDestructionRoute;
        case eObjectType_DestructionPont        : return xpm_objectTypeDestructionPont;
        case eObjectType_CampRefugies           : return xpm_objectTypeCampRefugies;
        case eObjectType_CampPrisonniers        : return xpm_objectTypeCampPrisonniers;
        case eObjectType_Barricade              : return xpm_objectTypeBarricade;
        case eObjectType_AirePoser              : return xpm_objectTypeAirePoser;
        case eObjectType_AireLogistique         : return xpm_objectTypeAireLogistique;
        case eObjectType_Abattis                : return xpm_objectTypeAbattis;
        default: return 0;
    };
}
