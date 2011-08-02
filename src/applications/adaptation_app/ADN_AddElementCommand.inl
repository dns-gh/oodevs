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
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_AddElementCommand.inl $
// $Author: Ape $
// $Modtime: 8/04/05 11:45 $
// $Revision: 2 $
// $Workfile: ADN_AddElementCommand.inl $
//
// *****************************************************************************

#include <QtGui/qapplication.h>

// -----------------------------------------------------------------------------
// Name: ADN_AddElementCommand constructor
/** @param  modifiedData 
    @param  pAddedObject 
*/
// Created: AGN 2004-05-13
// -----------------------------------------------------------------------------
template< class ObjectType >
ADN_AddElementCommand< ObjectType >::ADN_AddElementCommand( ADN_Type_Vector_ABC< ObjectType >& modifiedData, ObjectType* pAddedObject )
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
template< class ObjectType >
ADN_AddElementCommand< ObjectType >::~ADN_AddElementCommand()
{
    
}

// -----------------------------------------------------------------------------
// Name: ADN_AddElementCommand::undo
// Created: AGN 2004-05-13
// -----------------------------------------------------------------------------
template< class ObjectType >
void ADN_AddElementCommand< ObjectType >::undo()
{
    modifiedData_.RemItem( pAddedObject_ );
    delete pAddedObject_;
}

// -----------------------------------------------------------------------------
// Name: ADN_AddElementCommand::UpdateDescription
// Created: AGN 2004-05-13
// -----------------------------------------------------------------------------
template< class ObjectType >
void ADN_AddElementCommand< ObjectType >::UpdateDescription()
{
    QString strDescription( tr( "Add " ) );
    
    std::string strItemName = pAddedObject_->GetItemName();
    if( strItemName.empty() )
        strItemName = tr( "new " ).ascii() + modifiedData_.GetItemTypeName();
    else
        strItemName = modifiedData_.GetItemTypeName() + " " + strItemName;

    strDescription += strItemName.c_str();

    std::string strNodeName = modifiedData_.GetNodeName();

    if( ! strNodeName.empty() )
        strDescription += strNodeName.c_str();
    
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