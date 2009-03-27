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
#include "game_asn/SimulationSenders.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: LogisticPrototype::LogisticPrototype
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
LogisticPrototype::LogisticPrototype( QWidget* parent, Controllers& controllers, ASN1T_MagicActionCreateObject& msg )
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
    msg_.attributes.m.logisticPresent = 1;
    msg_.attributes.logistic.tc2 = tc2s_->GetValue()->GetId();
}

// -----------------------------------------------------------------------------
// Name: LogisticPrototype::Clean
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
void LogisticPrototype::Clean()
{
    msg_.attributes.m.logisticPresent = 0;
}
