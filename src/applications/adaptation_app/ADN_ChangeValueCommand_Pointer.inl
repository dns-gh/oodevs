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
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_ChangeValueCommand_Pointer.inl $
// $Author: Ape $
// $Modtime: 1/12/04 15:19 $
// $Revision: 2 $
// $Workfile: ADN_ChangeValueCommand_Pointer.inl $
//
// *****************************************************************************

#include <qapp.h>

// -----------------------------------------------------------------------------
// Name: ADN_ChangeValueCommand_Pointer constructor
/** @param  modifiedData 
    @param  pOldValue 
    @param  pNewValue 
*/
// Created: AGN 2004-05-13
// -----------------------------------------------------------------------------
template< class PointedClass >
ADN_ChangeValueCommand_Pointer< PointedClass >::ADN_ChangeValueCommand_Pointer( ADN_TypePtr_InVector_ABC< PointedClass >& modifiedData, PointedClass* pOldValue, PointedClass* pNewValue )
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
template< class PointedClass >
ADN_ChangeValueCommand_Pointer< PointedClass >::~ADN_ChangeValueCommand_Pointer()
{

}

// -----------------------------------------------------------------------------
// Name: ADN_ChangeValueCommand_Pointer::undo
// Created: AGN 2004-05-13
// -----------------------------------------------------------------------------
template< class PointedClass >
void ADN_ChangeValueCommand_Pointer< PointedClass >::undo()
{
    modifiedData_ = pOldValue_;
}

// -----------------------------------------------------------------------------
// Name: ADN_ChangeValueCommand_Pointer::UpdateDescription
// Created: AGN 2004-05-13
// -----------------------------------------------------------------------------
template< class PointedClass >
void ADN_ChangeValueCommand_Pointer< PointedClass >::UpdateDescription()
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
        strDescription += pOldValue_->strName_.GetData().c_str();
    strDescription += qApp->translate("ADN_ChangeValueCommand"," to ");
    if( pNewValue_ != 0 )
        strDescription += pNewValue_->strName_.GetData().c_str();
    setDescription( strDescription );        
}
