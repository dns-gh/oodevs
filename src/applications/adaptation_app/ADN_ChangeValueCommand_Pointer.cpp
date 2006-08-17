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
// $Archive: /MVW_v10/Build/SDK/ADN2/src/ADN_ChangeValueCommand_Pointer.cpp $
// $Author: Ape $
// $Modtime: 21/10/04 17:47 $
// $Revision: 1 $
// $Workfile: ADN_ChangeValueCommand_Pointer.cpp $
//
// *****************************************************************************

#include "adaptation_app_pch.h"

#include "ADN_ChangeValueCommand_Pointer.h"

// =============================================================================
// 
// =============================================================================

// -----------------------------------------------------------------------------
// Name: ADN_ChangeValueCommand_Pointer constructor
/** @param  modifiedData 
    @param  pOldValue 
    @param  pNewValue 
*/
// Created: AGN 2004-05-13
// -----------------------------------------------------------------------------
ADN_ChangeValueCommand_Pointer< ADN_Type_ABC< std::string > >::ADN_ChangeValueCommand_Pointer( ADN_TypePtr_InVector_ABC< ADN_Type_ABC< std::string > >& modifiedData, ADN_Type_ABC< std::string >* pOldValue, ADN_Type_ABC< std::string >* pNewValue )
: QtCommand( Command )
, modifiedData_( modifiedData )
, pOldValue_( pOldValue )
, pNewValue_( pNewValue )
{
    UpdateDescription();
}

// -----------------------------------------------------------------------------
// Name: ADN_ChangeValueCommand_Pointer destructor
// Created: AGN 2004-05-13
// -----------------------------------------------------------------------------
ADN_ChangeValueCommand_Pointer< ADN_Type_ABC< std::string > >::~ADN_ChangeValueCommand_Pointer()
{
    
}

// -----------------------------------------------------------------------------
// Name: ADN_ChangeValueCommand_Pointer::undo
// Created: AGN 2004-05-13
// -----------------------------------------------------------------------------
void ADN_ChangeValueCommand_Pointer< ADN_Type_ABC< std::string > >::undo()
{
    modifiedData_ = pOldValue_;
}

// -----------------------------------------------------------------------------
// Name: ADN_ChangeValueCommand_Pointer::UpdateDescription
// Created: AGN 2004-05-13
// -----------------------------------------------------------------------------
void ADN_ChangeValueCommand_Pointer< ADN_Type_ABC< std::string > >::UpdateDescription()
{
    QString strDescription( qApp->translate("ADN_ChangeValueCommand","Change ") );
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

    strDescription += qApp->translate("ADN_ChangeValueCommand"," from ");
    if( pOldValue_ != 0 )
        strDescription += pOldValue_->GetData().c_str();
    strDescription += qApp->translate("ADN_ChangeValueCommand"," to ");
    if( pNewValue_ != 0 )
        strDescription += pNewValue_->GetData().c_str();
    setDescription( strDescription );        
}