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
// $Archive: /MVW_v10/Build/SDK/ADN2/src/ADN_RemoveElementCommand.inl $
// $Author: Ape $
// $Modtime: 21/10/04 17:47 $
// $Revision: 1 $
// $Workfile: ADN_RemoveElementCommand.inl $
//
// *****************************************************************************

#include <QtGui/qapplication.h>

// -----------------------------------------------------------------------------
// Name: ADN_RemoveElementCommand constructor
/** @param  modifiedData 
    @param  pRemovedObject 
*/
// Created: AGN 2004-05-13
// -----------------------------------------------------------------------------
template< class ObjectType >
ADN_RemoveElementCommand< ObjectType >::ADN_RemoveElementCommand( ADN_Type_Vector_ABC< ObjectType >& modifiedData, ObjectType* pRemovedObject )
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
template< class ObjectType >
ADN_RemoveElementCommand< ObjectType >::~ADN_RemoveElementCommand()
{
    delete pRemovedObject_;
}

// -----------------------------------------------------------------------------
// Name: ADN_RemoveElementCommand::undo
// Created: AGN 2004-05-13
// -----------------------------------------------------------------------------
template< class ObjectType >
void ADN_RemoveElementCommand< ObjectType >::undo()
{
    modifiedData_.AddItem( pRemovedObject_ );
    pRemovedObject_ = 0;
}

// -----------------------------------------------------------------------------
// Name: ADN_RemoveElementCommand::UpdateDescription
// Created: AGN 2004-05-13
// -----------------------------------------------------------------------------
template< class ObjectType >
void ADN_RemoveElementCommand< ObjectType >::UpdateDescription()
{
    QString strDescription( qApp->translate("ADN_RemoveElementCommand","Remove ") );
    
    std::string strItemName = pRemovedObject_->GetItemName();
    if( strItemName.empty() )
        strItemName = qApp->translate("ADN_RemoveElementCommand","new ").ascii() + modifiedData_.GetItemTypeName();
    else
        strItemName = modifiedData_.GetItemTypeName() + " " + strItemName;

    strDescription += strItemName.c_str();
    
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