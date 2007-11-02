// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Preparation_Types_h_
#define __Preparation_Types_h_

#include "clients_kernel/StrongType.h"

namespace kernel
{
    class Automat_ABC;
    class Karma;
}

#define DECLARE_SUBTYPE( name, base ) \
struct name##_{};\
typedef kernel::StrongType< base, name##_ > name;

DECLARE_SUBTYPE( TC2, const kernel::Automat_ABC* );
DECLARE_SUBTYPE( MaintenanceSuperior, const kernel::Automat_ABC* );
DECLARE_SUBTYPE( MedicalSuperior, const kernel::Automat_ABC* );
DECLARE_SUBTYPE( SupplySuperior, const kernel::Automat_ABC* );

DECLARE_SUBTYPE( TeamKarma, const kernel::Karma* );
DECLARE_SUBTYPE( IntelligenceKarma, const kernel::Karma* );

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
    eLimaFuncLIA    = 9, // Ligne d'identification acceuil
    eLimaFuncNbr         // Keep this at the end of the enum! No !
};

#endif // __Preparation_Types_h_
