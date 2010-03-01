// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "LogisticPrototype.h"
#include "clients_kernel/Automat_ABC.h"
#include "protocol/Protocol.h"       

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: LogisticPrototype::LogisticPrototype
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
LogisticPrototype::LogisticPrototype( QWidget* parent, Controllers& controllers, MsgsClientToSim::MsgMagicActionCreateObject& msg )
    : LogisticPrototype_ABC( parent, controllers )
    , msg_ ( msg )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: LogisticPrototype::~LogisticPrototype
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
LogisticPrototype::~LogisticPrototype()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticPrototype::Commit
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
void LogisticPrototype::Commit()
{
    msg_.mutable_attributes()->mutable_logistic()->set_tc2( tc2s_->GetValue()->GetId());
}

// -----------------------------------------------------------------------------
// Name: LogisticPrototype::Clean
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
void LogisticPrototype::Clean()
{
    if( msg_.attributes().has_logistic() )
        msg_.mutable_attributes()->clear_logistic();
}
