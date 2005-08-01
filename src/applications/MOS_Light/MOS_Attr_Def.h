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
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_Attr_Def.h $
// $Author: Nld $
// $Modtime: 30/11/04 18:33 $
// $Revision: 23 $
// $Workfile: MOS_Attr_Def.h $ 
//*****************************************************************************

#ifndef __MOS_Attr_Def_h_
#define __MOS_Attr_Def_h_

enum E_PostureType
{
    ePostureMouvement         = 0,
    ePostureMouvementDiscret  = 1,
    ePostureArret             = 2,
    ePosturePosteReflexe      = 3,
    ePosturePoste             = 4,
    ePosturePosteAmenage      = 5,
    ePosturePostePrepareGenie = 6,
    eNbrPosturesModifyingDetection = 7,
    ePostureTransporte        = 7,
    eNbrPostureType           = 8
};

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

enum E_TypeMunition
{
    eMunitionEau        = 0,
    eMunitionBalle      = 1,
    eMunitionChaise     = 2,
    eMunitionLampadaire = 3,
    eMunitionDetritus   = 4,
};


#endif // __MOS_Attr_Def_h_
