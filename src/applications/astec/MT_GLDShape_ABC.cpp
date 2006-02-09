// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-06-24 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MT_GLDShape_ABC.cpp $
// $Author: Ape $
// $Modtime: 2/09/04 11:10 $
// $Revision: 2 $
// $Workfile: MT_GLDShape_ABC.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "astec_pch.h"
#include "MT_GLDShape_ABC.h"

#include "MT_GLDShape.h"
#include "MT_GLDShapeGroup.h"

// -----------------------------------------------------------------------------
// Name: MT_GLDShape_ABC constructor
// Created: APE 2004-06-24
// -----------------------------------------------------------------------------
MT_GLDShape_ABC::MT_GLDShape_ABC( MT_GLDrawer& glDrawer )
    : QObject   ()
    , glDrawer_ ( glDrawer )
    , nState_   ( eCreation )
{
}


// -----------------------------------------------------------------------------
// Name: MT_GLDShape_ABC destructor
// Created: APE 2004-06-24
// -----------------------------------------------------------------------------
MT_GLDShape_ABC::~MT_GLDShape_ABC()
{
}


// -----------------------------------------------------------------------------
// Name: MT_GLDShape_ABC::ReadNewShape
/** @param  archive 
    @return 
*/
// Created: APE 2004-07-07
// -----------------------------------------------------------------------------
//static
MT_GLDShape_ABC* MT_GLDShape_ABC::ReadNewShape( MT_XXmlInputArchive& archive, MT_GLDrawer& glDrawer )
{
    std::string strSection = archive.GetCurrentElementName();
    
    if( strSection == "Shape" )
        return new MT_GLDShape( glDrawer, MT_GLDShape::ePoint );

    if( strSection == "ShapeGroup" )
        return new MT_GLDShapeGroup( glDrawer );

    throw;
}
