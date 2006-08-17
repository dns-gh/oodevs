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
// $Archive: /MVW_v10/Build/SDK/ADN2/src/ADN_RemoveElementCommand.cpp $
// $Author: Ape $
// $Modtime: 21/10/04 17:47 $
// $Revision: 1 $
// $Workfile: ADN_RemoveElementCommand.cpp $
//
// *****************************************************************************


#include "adaptation_app_pch.h"
#include "ADN_RemoveElementCommand.h"

// -----------------------------------------------------------------------------
// Name: ADN_RemoveElementCommand constructor
/** @param  modifiedData 
    @param  pRemovedObject 
*/
// Created: AGN 2004-05-13
// -----------------------------------------------------------------------------
ADN_RemoveElementCommand< ADN_Type_ABC< std::string > >::ADN_RemoveElementCommand( ADN_Type_Vector_ABC< ADN_Type_ABC< std::string > >& modifiedData, ADN_Type_ABC< std::string >* pRemovedObject )
: QtCommand( Command )
, modifiedData_( modifiedData )
, pRemovedObject_( pRemovedObject )
{
    UpdateDescription();
}

// -----------------------------------------------------------------------------
// Name: ADN_RemoveElementCommand destructor
// Created: AGN 2004-05-13
// -----------------------------------------------------------------------------
ADN_RemoveElementCommand< ADN_Type_ABC< std::string > >::~ADN_RemoveElementCommand()
{
    delete pRemovedObject_;
}

// -----------------------------------------------------------------------------
// Name: ADN_RemoveElementCommand::undo
// Created: AGN 2004-05-13
// -----------------------------------------------------------------------------
void ADN_RemoveElementCommand< ADN_Type_ABC< std::string > >::undo()
{
    modifiedData_.AddItem( pRemovedObject_ );
    pRemovedObject_ = 0;
}

// -----------------------------------------------------------------------------
// Name: ADN_RemoveElementCommand::UpdateDescription
// Created: AGN 2004-05-13
// -----------------------------------------------------------------------------
void ADN_RemoveElementCommand< ADN_Type_ABC< std::string > >::UpdateDescription()
{
    QString strDescription( qApp->translate("ADN_RemoveElementCommand","Remove ") );
    strDescription += modifiedData_.GetItemTypeName().c_str();
    strDescription += " ";
    strDescription += pRemovedObject_->GetData().c_str();
    strDescription += qApp->translate("ADN_RemoveElementCommand"," from ");
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
// Name: ADN_RemoveElementCommand constructor
/** @param  modifiedData 
    @param  pRemovedObject 
*/
// Created: AGN 2004-05-13
// -----------------------------------------------------------------------------
ADN_RemoveElementCommand< ADN_Type_ABC< double > >::ADN_RemoveElementCommand( ADN_Type_Vector_ABC< ADN_Type_ABC< double > >& modifiedData, ADN_Type_ABC< double >* pRemovedObject )
: QtCommand( Command )
, modifiedData_( modifiedData )
, pRemovedObject_( pRemovedObject )
{
    UpdateDescription();
}

// -----------------------------------------------------------------------------
// Name: ADN_RemoveElementCommand destructor
// Created: AGN 2004-05-13
// -----------------------------------------------------------------------------
ADN_RemoveElementCommand< ADN_Type_ABC< double > >::~ADN_RemoveElementCommand()
{
    delete pRemovedObject_;
}

// -----------------------------------------------------------------------------
// Name: ADN_RemoveElementCommand::undo
// Created: AGN 2004-05-13
// -----------------------------------------------------------------------------
void ADN_RemoveElementCommand< ADN_Type_ABC< double > >::undo()
{
    modifiedData_.AddItem( pRemovedObject_ );
    pRemovedObject_ = 0;
}

// -----------------------------------------------------------------------------
// Name: ADN_RemoveElementCommand::UpdateDescription
// Created: AGN 2004-05-13
// -----------------------------------------------------------------------------
void ADN_RemoveElementCommand< ADN_Type_ABC< double > >::UpdateDescription()
{
    QString strDescription( qApp->translate("ADN_RemoveElementCommand","Remove ") );
    strDescription += modifiedData_.GetItemTypeName().c_str();
    strDescription += qApp->translate("ADN_RemoveElementCommand"," from ");
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