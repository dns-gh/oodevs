// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __Gaming_Types_h_
#define __Gaming_Types_h_

#include "ASN_Types.h"
#include "clients_kernel/Types.h"

enum E_FuncLimaType
{
    eLimaFuncLD     = 0, // Ligne de d�bouch�
    eLimaFuncLCA    = 1, // Ligne de changement d'attitude
    eLimaFuncLC     = 2, // Ligne de coordination
    eLimaFuncLI     = 3, // Ligne � interdire
    eLimaFuncLO     = 4, // Ligne d'objectif
    eLimaFuncLCAR   = 5, // Ligne de coup d'arr�t
    eLimaFuncLR     = 6, // Ligne de recueil
    eLimaFuncLDM    = 7, // Ligne de d�but de mission
    eLimaFuncLFM    = 8, // Ligne de fin de mission
    eLimaFuncLIA    = 9, // Ligne d'identification acceuil
    eLimaFuncNbr         // Keep this at the end of the enum! No !
};

#endif // __Gaming_Types_h_
