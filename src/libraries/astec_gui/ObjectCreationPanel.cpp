// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "astec_gui_pch.h"
#include "ObjectCreationPanel.h"
#include "moc_ObjectCreationPanel.cpp"
#include "ObjectPrototype.h"
#include "astec_gaming/ASN_Messages.h"
#include "astec_kernel/ObjectType.h"
#include "astec_kernel/IDManager.h"

// -----------------------------------------------------------------------------
// Name: ObjectCreationPanel constructor
// Created: SBO 2006-04-18
// -----------------------------------------------------------------------------
ObjectCreationPanel::ObjectCreationPanel( QWidget* parent, Controllers& controllers, Publisher_ABC& publisher, const StaticModel& model, ParametersLayer& layer, const GlTools_ABC& tools )
    : QVBox( parent )
    , publisher_( publisher )
    , tools_( tools )
    , created_( new ObjectPrototype( this, controllers, model, layer ) )
{
    QPushButton* pOkButton = new QPushButton( tr( "Cr�er" ), this );
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
        return;

    ASN_MsgObjectMagicAction msg;
    msg.GetAsnMsg().oid_objet = created_->GetType().manager_.GetFreeIdentifier();

    ASN1T_MagicActionCreateObject action;
    msg.GetAsnMsg().action.t                 = T_MsgObjectMagicAction_action_create_object;
    msg.GetAsnMsg().action.u.create_object   = &action;
    created_->Serialize( msg );
    msg.Send( publisher_ );
    created_->Clean();
}

// -----------------------------------------------------------------------------
// Name: ObjectCreationPanel::Draw
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
void ObjectCreationPanel::Draw( const geometry::Rectangle2f& /*viewport*/ )
{
    created_->Draw( tools_ );
}
