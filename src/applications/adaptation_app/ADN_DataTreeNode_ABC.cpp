// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGN 2004-05-12 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_DataTreeNode_ABC.cpp $
// $Author: Ape $
// $Modtime: 11/01/05 10:13 $
// $Revision: 2 $
// $Workfile: ADN_DataTreeNode_ABC.cpp $
//
// *****************************************************************************


#include "adaptation_app_pch.h"
#include "ADN_DataTreeNode_ABC.h"


// -----------------------------------------------------------------------------
// Name: ADN_DataTreeNode_ABC constructor
/** @param  pParent 
*/
// Created: AGN 2004-05-12
// -----------------------------------------------------------------------------
ADN_DataTreeNode_ABC::ADN_DataTreeNode_ABC( ADN_DataTreeNode_ABC* pParent )
: pParent_( pParent )
{
    
}

// -----------------------------------------------------------------------------
// Name: ADN_DataTreeNode_ABC destructor
// Created: AGN 2004-05-12
// -----------------------------------------------------------------------------
ADN_DataTreeNode_ABC::~ADN_DataTreeNode_ABC()
{
    
}

// -----------------------------------------------------------------------------
// Name: ADN_DataTreeNode_ABC::GetXmlLabel
/** @return 
*/
// Created: AGN 2004-05-12
// -----------------------------------------------------------------------------
std::string ADN_DataTreeNode_ABC::GetNodeName()
{
    return std::string();
}


// -----------------------------------------------------------------------------
// Name: ADN_DataTreeNode_ABC::GetParentNode
/** @return 
*/
// Created: AGN 2004-05-12
// -----------------------------------------------------------------------------
ADN_DataTreeNode_ABC* ADN_DataTreeNode_ABC::GetParentNode() const
{
    return pParent_;
}
