// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGN 2004-05-13 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_AddElementCommand.cpp $
// $Author: Ape $
// $Modtime: 8/04/05 11:43 $
// $Revision: 2 $
// $Workfile: ADN_AddElementCommand.cpp $
//
// *****************************************************************************


#include "ADN_pch.h"
#include "ADN_AddElementCommand.h"

// -----------------------------------------------------------------------------
// Name: ADN_AddElementCommand constructor
/** @param  modifiedData 
    @param  pAddedObject 
*/
// Created: AGN 2004-05-13
// -----------------------------------------------------------------------------
ADN_AddElementCommand< ADN_Type_ABC< std::string > >::ADN_AddElementCommand( ADN_Type_Vector_ABC< ADN_Type_ABC< std::string > >& modifiedData, ADN_Type_ABC< std::string >* pAddedObject )
: QtCommand( Command )
, modifiedData_( modifiedData )
, pAddedObject_( pAddedObject )
{
    UpdateDescription();
}

// -----------------------------------------------------------------------------
// Name: ADN_AddElementCommand destructor
// Created: AGN 2004-05-13
// -----------------------------------------------------------------------------
ADN_AddElementCommand< ADN_Type_ABC< std::string > >::~ADN_AddElementCommand()
{
    
}

// -----------------------------------------------------------------------------
// Name: ADN_AddElementCommand::undo
// Created: AGN 2004-05-13
// -----------------------------------------------------------------------------
void ADN_AddElementCommand< ADN_Type_ABC< std::string > >::undo()
{
    modifiedData_.RemItem( pAddedObject_ );
    delete pAddedObject_;
}

// -----------------------------------------------------------------------------
// Name: ADN_AddElementCommand::UpdateDescription
// Created: AGN 2004-05-13
// -----------------------------------------------------------------------------
void ADN_AddElementCommand< ADN_Type_ABC< std::string > >::UpdateDescription()
{
    QString strDescription( tr( "Add " ) );
    strDescription += modifiedData_.GetItemTypeName().c_str();
    strDescription += tr( " to ");
    strDescription += modifiedData_.GetNodeName().c_str();
    
    ADN_DataTreeNode_ABC* pParent = modifiedData_.GetParentNode();
    while( pParent != 0 )
    {
        std::string strNodeName = pParent->GetNodeName();
        if( ! strNodeName.empty() )
        {
            strDescription += " ";
            strDescription += strNodeName.c_str();
        }
        pParent = pParent->GetParentNode();
    }

    setDescription( strDescription );    
}

// =============================================================================
// 
// =============================================================================

// -----------------------------------------------------------------------------
// Name: ADN_AddElementCommand constructor
/** @param  modifiedData 
    @param  pAddedObject 
*/
// Created: AGN 2004-05-13
// -----------------------------------------------------------------------------
ADN_AddElementCommand< ADN_Type_ABC< double > >::ADN_AddElementCommand( ADN_Type_Vector_ABC< ADN_Type_ABC< double > >& modifiedData, ADN_Type_ABC< double >* pAddedObject )
: QtCommand( Command )
, modifiedData_( modifiedData )
, pAddedObject_( pAddedObject )
{
    UpdateDescription();
}

// -----------------------------------------------------------------------------
// Name: ADN_AddElementCommand destructor
// Created: AGN 2004-05-13
// -----------------------------------------------------------------------------
ADN_AddElementCommand< ADN_Type_ABC< double > >::~ADN_AddElementCommand()
{
    
}

// -----------------------------------------------------------------------------
// Name: ADN_AddElementCommand::undo
// Created: AGN 2004-05-13
// -----------------------------------------------------------------------------
void ADN_AddElementCommand< ADN_Type_ABC< double > >::undo()
{
    modifiedData_.RemItem( pAddedObject_ );
    delete pAddedObject_;
}

// -----------------------------------------------------------------------------
// Name: ADN_AddElementCommand::UpdateDescription
// Created: AGN 2004-05-13
// -----------------------------------------------------------------------------
void ADN_AddElementCommand< ADN_Type_ABC< double > >::UpdateDescription()
{
    QString strDescription( tr( "Add new " ) );
    strDescription += modifiedData_.GetItemTypeName().c_str();
    strDescription += tr( " to " );
    strDescription += modifiedData_.GetNodeName().c_str();
    
    ADN_DataTreeNode_ABC* pParent = modifiedData_.GetParentNode();
    while( pParent != 0 )
    {
        std::string strNodeName = pParent->GetNodeName();
        if( ! strNodeName.empty() )
        {
            strDescription += " ";
            strDescription += strNodeName.c_str();
        }
        pParent = pParent->GetParentNode();
    }

    setDescription( strDescription );    
}