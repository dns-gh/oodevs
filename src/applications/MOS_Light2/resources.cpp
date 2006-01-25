//****************************************************************************
//
// This file is part of MASA DIN library.
// 
// Refer to the included end-user license agreement (License.rtf) for
// restrictions.
//
// Copyright (c) 2002 Mathématiques Appliquées SA (MASA)
// All Rights Reserved. DIN is a trademark of MASA Corporation.
//
//****************************************************************************
//
// $Created:  NLD 2002-01-02 $
//
//*****************************************************************************

#include "MOS_Light2_pch.h"

#include "resources.h"

// Hack to encapsulate the xpm resources in a resources.cpp 
// instead of resources.h
#define static 

#include "res/mosicon.xpm"
#include "res/connexiongreen.xpm"
#include "res/connexionorange.xpm"
#include "res/connexionred.xpm"
#include "res/play.xpm"
#include "res/stop.xpm"
#include "res/textsmall.xpm"
#include "res/textbig.xpm"
#include "res/crossings.xpm"
#include "res/weather.xpm"
#include "res/path.xpm"
#include "res/oldpath.xpm"
#include "res/embraye.xpm"
#include "res/debraye.xpm"
#include "res/visionlines.xpm"
#include "res/visioncones.xpm"
#include "res/visionsurfaces.xpm"
#include "res/tickon.xpm"
#include "res/tickoff.xpm"
#include "res/tickred.xpm"
#include "res/msg.xpm"
#include "res/sim.xpm"
#include "res/simdbg.xpm"
#include "res/open.xpm"
#include "res/save.xpm"
#include "res/saveas.xpm"
#include "res/new.xpm"
#include "res/gas.xpm"
#include "res/ammo.xpm"
#include "res/nbc.xpm"
#include "res/construction.xpm"
#include "res/observe.xpm"
#include "res/tacticallines.xpm"
#include "res/flare.xpm"
#include "res/smoke.xpm"
#include "res/skull.xpm"
#include "res/agentmessage.xpm"
#include "res/messageoptions.xpm"
#include "res/conflict.xpm"
#include "res/threed.xpm"
#include "res/visible.xpm"
#include "res/select.xpm"
#include "res/point.xpm"
#include "res/line.xpm"
#include "res/circle.xpm"
#include "res/rectangle.xpm"
#include "res/polygon.xpm"
#include "res/text.xpm"
#include "res/image.xpm"
#include "res/empty.xpm"
#include "res/cross.xpm"
#include "res/saveall.xpm"
#include "res/missinglog.xpm"
#include "res/loglink.xpm"
#include "res/autolog.xpm"
#include "res/realtime.xpm"
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
#include "res/cadenas.xpm"
#include "res/cadenas_ouvert.xpm"
#include "res/talkie.xpm"
#include "res/talkie_interdit.xpm"
#include "res/brouillage.xpm"
#include "res/radars_on.xpm"
#include "res/aggregate.xpm"
#include "res/desaggregate.xpm"
#include "res/recrec.xpm"
#include "res/recplay.xpm"
#include "res/recstop.xpm"

QIconSet MakeExtendedIcon( const char* pEnabledXpm[], const char* pDisabledXpm[] )
{
    QPixmap enabledPixmap ( pEnabledXpm );
    QPixmap disabledPixmap( pDisabledXpm );

    QIconSet iconset  ( enabledPixmap );
    QIconSet iconset_x( disabledPixmap );

    iconset.setPixmap( iconset_x.pixmap(), QIconSet::Automatic, QIconSet::Disabled );

    return iconset;
}