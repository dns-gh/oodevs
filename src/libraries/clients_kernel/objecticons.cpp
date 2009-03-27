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
#include "ObjectType.h"

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
#include "res/objectTypeFire.xpm"
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
#include "res/objectTypeMedicalTreatment.xpm"

using namespace kernel;

namespace
{
    static std::map< std::string, const char** > sTypeIcons;
    void Init()
    {
        sTypeIcons[ "zone protegee" ] = xpm_objectTypeZoneProtegee;
        sTypeIcons[ "zone poser helicoptere" ] = xpm_objectTypeZonePoserHelicoptere;
        sTypeIcons[ "zone nbc" ] = xpm_objectTypeZoneNBC;
        sTypeIcons[ "zone mobilite amelioree" ] = xpm_objectTypeZoneMobiliteAmelioree;
        sTypeIcons[ "zone interdite tir" ] = xpm_objectTypeZoneInterditeTir;
        sTypeIcons[ "zone interdite mouvement" ] = xpm_objectTypeZoneInterditeMvt;
        sTypeIcons[ "zone implantation lrm" ] = xpm_objectTypeZoneImplantationLrm ;
        sTypeIcons[ "zone implantation cobra" ] = xpm_objectTypeZoneImplantationCobra;
        sTypeIcons[ "zone implantation mortier:" ] = xpm_objectTypeZoneImplantationCanon;
        sTypeIcons[ "zone implantation canon" ] = xpm_objectTypeZoneImplantationCanon;
        sTypeIcons[ "zone brouillage" ] = xpm_objectTypeZoneBrouillardBrod ;
        sTypeIcons[ "terrain largage" ] = xpm_objectTypeTerrainLargage;
        sTypeIcons[ "site franchissement" ] = xpm_objectTypeSiteFranchissement;
        sTypeIcons[ "site decontamination" ] = xpm_objectTypeSiteDecontamination;
        sTypeIcons[ "rota" ] = xpm_objectTypeRota ;
        sTypeIcons[ "poste tir" ] = xpm_objectTypePosteTir;
        sTypeIcons[ "poste controle" ] = xpm_objectTypePosteControle;
        sTypeIcons[ "pont flottant continu" ] = xpm_objectTypePontFlottant;
        sTypeIcons[ "pont flottant discontinu" ] = xpm_objectTypePontFlottant;
        sTypeIcons[ "plot ravitaillement" ] = xpm_objectTypePlotRavitaillement;
        sTypeIcons[ "plateforme" ] = xpm_objectTypePlateforme;
        sTypeIcons[ "piste" ] = xpm_objectTypePiste;
        sTypeIcons[ "nuage nbc" ] = xpm_objectTypeNuageNBC;
        sTypeIcons[ "fire" ] = xpm_objectTypeFire;
        //sTypeIcons[ "MedicalTreatment" ] = xpm_objectTypeMedicalTreatment;
        sTypeIcons[ "bouchon mines" ] = xpm_objectTypeMine;
        sTypeIcons[ "zone minee lineaire" ] = xpm_objectTypeMine;
        sTypeIcons[ "zone minee par dispersion" ] = xpm_objectTypeMine;
        sTypeIcons[ "itineraire logistique" ] = xpm_objectTypeItineraireLog;
        sTypeIcons[ "fosse anti char" ] = xpm_objectTypeFosseAC;
        sTypeIcons[ "eboulement" ] = xpm_objectTypeEboulement;
        sTypeIcons[ "destruction route" ] = xpm_objectTypeDestructionRoute;
        sTypeIcons[ "destruction pont" ] = xpm_objectTypeDestructionPont;
        sTypeIcons[ "camp refugies" ] = xpm_objectTypeCampRefugies;
        sTypeIcons[ "camp prisonniers" ] = xpm_objectTypeCampPrisonniers;
        sTypeIcons[ "barricade" ] = xpm_objectTypeBarricade;
        sTypeIcons[ "aire poser" ] = xpm_objectTypeAirePoser;
        sTypeIcons[ "aire logistique" ] = xpm_objectTypeAireLogistique;
        sTypeIcons[ "abattis" ] = xpm_objectTypeAbattis;
    }

// -----------------------------------------------------------------------------
// Name: TypeIcon
// Created: AGE 2008-03-14
// -----------------------------------------------------------------------------
const char** TypeIcon( const std::string& symbol, const std::string& type )
{
    if( sTypeIcons.empty() )
        Init();
    std::map< std::string, const char** >::const_iterator it = sTypeIcons.find( type );
    if( it != sTypeIcons.end() )
        return it->second;
    return 0;
}

}

// -----------------------------------------------------------------------------
// Name: objecticons::Draw
// Created: JCR 2008-06-10
// -----------------------------------------------------------------------------
void ObjectIcons::Draw( const ObjectType& type, const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools )
{
    if( viewport.IsVisible( where ) )
    {
        const std::string& symbol = type.GetSymbol();
        const std::string& name = type.GetName();
        tools.DrawIcon( TypeIcon( symbol, name ), where, 900.f );
    }
}
