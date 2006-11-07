// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "Tools.h"

// -----------------------------------------------------------------------------
// Name: tools::ToString
// Created: AGE 2006-08-08
// -----------------------------------------------------------------------------
QString tools::ToString( E_FuncLimaType nType )
{
    static const QString limas[] = 
    {
        tools::translate( "E_FuncLimaType", "LD" ),
        tools::translate( "E_FuncLimaType", "LCA" ),
        tools::translate( "E_FuncLimaType", "LC" ),
        tools::translate( "E_FuncLimaType", "LI" ),
        tools::translate( "E_FuncLimaType", "LO" ),
        tools::translate( "E_FuncLimaType", "LCAR" ),
        tools::translate( "E_FuncLimaType", "LR" ),
        tools::translate( "E_FuncLimaType", "LDM" ),
        tools::translate( "E_FuncLimaType", "LFM" ),
        tools::translate( "E_FuncLimaType", "LIA" ),
    };
    if( nType >= 0 && nType < eLimaFuncNbr )
        return limas[ nType ];
    return Unknown();
}
