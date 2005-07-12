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
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_Order_Def.h $
// $Author: Nld $
// $Modtime: 19/01/05 10:05 $
// $Revision: 93 $
// $Workfile: MOS_Order_Def.h $ 
//*****************************************************************************

#ifndef __MOS_Order_Def_h_
#define __MOS_Order_Def_h_


enum E_FuncLimaType
{
    eLimaFuncLD     = 0, // Ligne de débouché
    eLimaFuncLCA    = 1, // Ligne de changement d'attitude
    eLimaFuncLC     = 2, // Ligne de coordination
    eLimaFuncLI     = 3, // Ligne à interdire
    eLimaFuncLO     = 4, // Ligne d'objectif
    eLimaFuncLCAR   = 5, // Ligne de coup d'arrêt
    eLimaFuncLR     = 6, // Ligne de recueil
    eLimaFuncLDM    = 7, // Ligne de début de mission
    eLimaFuncLFM    = 8, // Ligne de fin de mission
    eLimaFuncLIA    = 9  // Ligne d'identification et de recueil
};

enum E_FamilyID
{
    eFamilyOffensif,
    eFamilyDefensif,
    eFamilySurete,
    eFamilyMobilite,
    eFamilyContreMobilite,
    eFamilyFranchissement,
    eFamilyDeploiement,

};

#endif // __MOS_Order_Def_h_
