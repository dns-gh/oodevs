// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "ObjectIcons.h"
#include "Viewport_ABC.h"
#include "GlTools_ABC.h"
#include "Types.h"

#define static 
#include "res/objectTypeZoneProtegee.xpm"
#include "res/objectTypeZonePoserHelicoptere.xpm"
#include "res/objectTypeZoneNBC.xpm"
#include "res/objectTypeZoneMobiliteAmelioree.xpm"
#include "res/objectTypeZoneInterditeTir.xpm"
#include "res/objectTypeZoneInterditeMvt.xpm"
#include "res/objectTypeZoneImplantationLrm.xpm"
#include "res/objectTypeZoneImplantationCobra.xpm"
#include "res/objectTypeZoneImplantationCanon.xpm"
#include "res/objectTypeZoneBrouillardBrod.xpm"
#include "res/objectTypeZoneBrouillageBromure.xpm"
#include "res/objectTypeTerrainLargage.xpm"
#include "res/objectTypeSiteFranchissement.xpm"
#include "res/objectTypeSiteDecontamination.xpm"
#include "res/objectTypeRota.xpm"
#include "res/objectTypePosteTir.xpm"
#include "res/objectTypePosteControle.xpm"
#include "res/objectTypePontFlottant.xpm"
#include "res/objectTypePlotRavitaillement.xpm"
#include "res/objectTypePlateForme.xpm"
#include "res/objectTypePiste.xpm"
#include "res/objectTypeNuageNBC.xpm"
#include "res/objectTypeMine.xpm"
#include "res/objectTypeItineraireLog.xpm"
#include "res/objectTypeImplantationMortier.xpm"
#include "res/objectTypeFosseAC.xpm"
#include "res/objectTypeEboulement.xpm"
#include "res/objectTypeDestructionRoute.xpm"
#include "res/objectTypeDestructionPont.xpm"
#include "res/objectTypeCampRefugies.xpm"
#include "res/objectTypeCampPrisonniers.xpm"
#include "res/objectTypeBarricade.xpm"
#include "res/objectTypeAirePoser.xpm"
#include "res/objectTypeAireLogistique.xpm"
#include "res/objectTypeAbattis.xpm"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ObjectIcons::Draw
// Created: AGE 2008-03-14
// -----------------------------------------------------------------------------
void ObjectIcons::Draw( unsigned long type, const geometry::Point2f& where, const Viewport_ABC& viewport, const GlTools_ABC& tools )
{
    if( viewport.IsVisible( where ) )
        tools.DrawIcon( TypeIcon( type ), where, 900.f );
}

// -----------------------------------------------------------------------------
// Name: ObjectIcons::TypeIcon
// Created: AGE 2008-03-14
// -----------------------------------------------------------------------------
const char** ObjectIcons::TypeIcon( unsigned long id )
{
    switch( id )
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
