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
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_ChangeValueCommand_String.cpp $
// $Author: Ape $
// $Modtime: 28/01/05 14:27 $
// $Revision: 2 $
// $Workfile: ADN_ChangeValueCommand_String.cpp $
//
// *****************************************************************************


#include "adaptation_app_pch.h"
#include "ADN_ChangeValueCommand_String.h"
#include "moc_ADN_ChangeValueCommand_String.cpp"

#include "ADN_Types.h"
#include <QtGui/qapplication.h>

// -----------------------------------------------------------------------------
// Name: ADN_ChangeValueCommand_String constructor
/** @param  modifiedData
    @param  nOldValue
    @param  nNewValue
*/
// Created: AGN 2004-05-13
// -----------------------------------------------------------------------------
ADN_ChangeValueCommand_String::ADN_ChangeValueCommand_String( ADN_Type_String& modifiedData, const std::string& nOldValue, const std::string& nNewValue )
: QtCommand()
, modifiedData_( modifiedData )
, oldValue_( nOldValue )
, newValue_( nNewValue )
{
    UpdateDescription();
}

// -----------------------------------------------------------------------------
// Name: ADN_ChangeValueCommand_String destructor
// Created: AGN 2004-05-13
// -----------------------------------------------------------------------------
ADN_ChangeValueCommand_String::~ADN_ChangeValueCommand_String()
{

}

// -----------------------------------------------------------------------------
// Name: ADN_ChangeValueCommand_String::undo
// Created: AGN 2004-05-13
// -----------------------------------------------------------------------------
void ADN_ChangeValueCommand_String::undo()
{
    modifiedData_ = oldValue_;
}

// -----------------------------------------------------------------------------
// Name: ADN_ChangeValueCommand_String::mergeMeWith
/** @param  other
    @return
*/
// Created: AGN 2004-05-13
// -----------------------------------------------------------------------------
bool ADN_ChangeValueCommand_String::mergeMeWith( QtCommand* other )
{
    ADN_ChangeValueCommand_String* pCastCommand = static_cast< ADN_ChangeValueCommand_String* >( other );
    if( & pCastCommand->modifiedData_ == & modifiedData_ )
    {
        newValue_ = pCastCommand->newValue_;
        UpdateDescription();
        return true;
    }
    else
        return false;
}

// -----------------------------------------------------------------------------
// Name: ADN_ChangeValueCommand_String::UpdateDescription
// Created: AGN 2004-05-13
// -----------------------------------------------------------------------------
void ADN_ChangeValueCommand_String::UpdateDescription()
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
    strDescription += oldValue_.c_str();
    strDescription += qApp->translate("ADN_ChangeValueCommand"," to ");
    strDescription += newValue_.c_str();
    setDescription( strDescription );
}