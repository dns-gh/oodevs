// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "edxlhave_plugin_pch.h"
#include "ObjectExtension.h"
#include "Publisher_ABC.h"
#include "HospitalUpdateReport.h"
#include "dispatcher/Model_ABC.h"
#include "dispatcher/Object_ABC.h"
#include "ReportBuilder_ABC.h"

using namespace plugins::edxl;

// -----------------------------------------------------------------------------
// Name: ObjectExtension constructor
// Created: JCR 2010-05-31
// -----------------------------------------------------------------------------
ObjectExtension::ObjectExtension( ReportBuilder_ABC& builder, const dispatcher::Model_ABC& model )
    : builder_ ( builder )
    , model_ ( model )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectExtension destructor
// Created: JCR 2010-05-31
// -----------------------------------------------------------------------------
ObjectExtension::~ObjectExtension()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectExtension::DoUpdate
// Created: JCR 2010-05-31
// -----------------------------------------------------------------------------
void ObjectExtension::DoUpdate( const MsgsSimToClient::MsgObjectCreation& message )
{
    if( message.attributes().has_medical_treatment() )
    {
        const dispatcher::Object_ABC* object = model_.Objects().Find( message.object().id() );
        if( object )
            builder_.CreateReport( message.object().id(), std::string( object->GetName().ascii() ), message.attributes().medical_treatment() );
    }
}

// -----------------------------------------------------------------------------
// Name: ObjectExtension::DoUpdate
// Created: JCR 2010-05-31
// -----------------------------------------------------------------------------
void ObjectExtension::DoUpdate( const MsgsSimToClient::MsgObjectUpdate& message )
{
    if( message.attributes().has_medical_treatment() )
    {
        const dispatcher::Object_ABC* object = model_.Objects().Find( message.object().id() );
        if( object )
            builder_.CreateReport( message.object().id(), std::string( object->GetName().ascii() ), message.attributes().medical_treatment() );
    }
}
