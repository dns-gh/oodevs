// *****************************************************************************
//
// $Created: NLD 2004-03-31 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Decision/Functions/DEC_ObjectFunctions.inl $
// $Author: Nld $
// $Modtime: 23/02/05 15:07 $
// $Revision: 3 $
// $Workfile: DEC_ObjectFunctions.inl $
//
// *****************************************************************************

#include "Entities/MIL_EntityManager.h"
#include "Entities/Objects/MIL_ObjectManipulator_ABC.h"

// -----------------------------------------------------------------------------
// Name: template< typename T > static void DEC_ObjectFunctions::MagicCreateObject
// Created: NLD 2005-01-19
// -----------------------------------------------------------------------------
template< typename T > 
int DEC_ObjectFunctions::MagicCreateObject( const T& caller, const std::string& type, const TER_Localisation* pLocalisation )
{
    if( !pLocalisation )
        return 0;
    return MagicCreateObject( caller.GetArmy(), type, *pLocalisation );
}

// -----------------------------------------------------------------------------
// Name: template< typename T > static int DEC_ObjectFunctions::MagicGetOrCreateObject
// Created: LDC 2013-02-11
// -----------------------------------------------------------------------------
template< typename T > 
int DEC_ObjectFunctions::MagicGetOrCreateObject( const T& caller, const std::string& type, const TER_Localisation* pLocalisation )
{
    if( !pLocalisation )
        return 0;
    return MagicGetOrCreateObject( caller.GetArmy(), type, *pLocalisation );
}
