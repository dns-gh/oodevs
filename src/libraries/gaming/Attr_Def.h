//*****************************************************************************
//
// This file is part of MASA DIN library.
//
// Refer to the included end-user license agreement (License.rtf) for
// restrictions.
//
// Copyright (c) 2002 Mathématiques Appliquées SA (MASA)
// All Rights Reserved. DIN is a trademark of MASA Corporation.
//
//*****************************************************************************
//
// $Created: NLD 2003-01-13 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/Attr_Def.h $
// $Author: Age $
// $Modtime: 31/03/05 16:33 $
// $Revision: 8 $
// $Workfile: Attr_Def.h $
//*****************************************************************************

#ifndef __Attr_Def_h_
#define __Attr_Def_h_

enum E_UnitVisType
{
    eVisTypeInvisible  = 0,
    eVisTypeDetected   = 1,
    eVisTypeRecognized = 2,
    eVisTypeIdentified = 3
};

enum E_PerceptionState // Utilisé pour les objets
{
    ePerceived,
    ePerceptionInProgress,
    eNotPerceived
};

enum E_NatureAtlasType
{
    eNatureAtlas_PC,
    eNatureAtlas_Armored,
    eNatureAtlas_ASS,
    eNatureAtlas_Log,
    eNatureAtlas_Mortar,
    eNatureAtlas_RocketLauncher,
    eNatureAtlas_None,
    eNatureAtlas_Vehicule,
    eNatureAtlas_PersonnelAPied,
    eNatureAtlas_Helicoptere,
    eNatureAtlas_ASA,
    eNatureAtlas_Indefini,
    eNbrNatureAtlas
};

#endif // __Attr_Def_h_
