// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "ObjectCreationPanel.h"
#include "moc_ObjectCreationPanel.cpp"
#include "ObjectPrototype.h"
#include "ASN_Messages.h"

// -----------------------------------------------------------------------------
// Name: ObjectCreationPanel constructor
// Created: SBO 2006-04-18
// -----------------------------------------------------------------------------
ObjectCreationPanel::ObjectCreationPanel( QWidget* parent, Controllers& controllers, const Model& model )
    : QVBox( parent )
    , created_( new ObjectPrototype( this, controllers, model ) )
{
    QPushButton* pOkButton = new QPushButton( tr( "Créer" ), this );
    connect( pOkButton, SIGNAL( clicked() ), this, SLOT( Commit() ) );
}
    
// -----------------------------------------------------------------------------
// Name: ObjectCreationPanel destructor
// Created: SBO 2006-04-18
// -----------------------------------------------------------------------------
ObjectCreationPanel::~ObjectCreationPanel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectCreationPanel::Commit
// Created: SBO 2006-04-18
// -----------------------------------------------------------------------------
void ObjectCreationPanel::Commit()
{
    if( !created_->CheckValidity() )
        throw std::exception( "Invalid object creation parameters" );

    ASN_MsgObjectMagicAction msg;
    msg.GetAsnMsg().oid_objet = 42; // $$$$ SBO 2006-04-19: IDManager

    ASN1T_MagicActionCreateObject action;
    msg.GetAsnMsg().action.t                 = T_MsgObjectMagicAction_action_create_object;
    msg.GetAsnMsg().action.u.create_object   = &action;
    created_->Serialize( msg );
    msg.Send( 912 );
}
