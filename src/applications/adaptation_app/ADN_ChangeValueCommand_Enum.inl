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
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_ChangeValueCommand_Enum.inl $
// $Author: Ape $
// $Modtime: 17/02/05 15:41 $
// $Revision: 2 $
// $Workfile: ADN_ChangeValueCommand_Enum.inl $
//
// *****************************************************************************

#include <QtGui/qapplication.h>

// -----------------------------------------------------------------------------
// Name: ADN_ChangeValueCommand_Enum constructor
/** @param  EnumType 
    @param  modifiedData 
    @param  nOldValue 
    @param  nNewValue 
*/
// Created: AGN 2004-05-13
// -----------------------------------------------------------------------------
template< class EnumType, int nb >
ADN_ChangeValueCommand_Enum< EnumType, nb >::ADN_ChangeValueCommand_Enum( ADN_Type_Enum< EnumType, nb >& modifiedData, EnumType nOldValue, EnumType nNewValue )
: QtCommand( Command )
, modifiedData_( modifiedData )
, oldValue_( nOldValue )
, newValue_( nNewValue )
{
    UpdateDescription();
}

// -----------------------------------------------------------------------------
// Name: ADN_ChangeValueCommand_Enum destructor
// Created: AGN 2004-05-13
// -----------------------------------------------------------------------------
template< class EnumType, int nb >
ADN_ChangeValueCommand_Enum< EnumType, nb >::~ADN_ChangeValueCommand_Enum()
{
    
}

// -----------------------------------------------------------------------------
// Name: ADN_ChangeValueCommand_Enum::undo
// Created: AGN 2004-05-13
// -----------------------------------------------------------------------------
template< class EnumType, int nb >
void ADN_ChangeValueCommand_Enum< EnumType, nb >::undo()
{
    modifiedData_ = oldValue_;
}

// -----------------------------------------------------------------------------
// Name: ADN_ChangeValueCommand_Enum::UpdateDescription
// Created: AGN 2004-05-13
// -----------------------------------------------------------------------------
template< class EnumType, int nb >
void ADN_ChangeValueCommand_Enum< EnumType, nb >::UpdateDescription()
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

    strDescription += ADN_Type_Enum< EnumType, nb >::ConvertValue( oldValue_, ENT_Tr_ABC::eToTr ).c_str();
    strDescription += qApp->translate("ADN_ChangeValueCommand"," to ");
    strDescription += ADN_Type_Enum< EnumType, nb >::ConvertValue( newValue_, ENT_Tr_ABC::eToTr ).c_str();
    setDescription( strDescription );    
}