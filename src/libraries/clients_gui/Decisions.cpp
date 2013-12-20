// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "Decisions.h"

#include "clients_kernel/Controller.h"
#include "clients_kernel/DecisionalModel.h"
#include "clients_kernel/Entity_ABC.h"

#include <tools/Resolver.h>

using namespace gui;

// -----------------------------------------------------------------------------
// Name: Decisions constructor
// Created: ABR 2013-12-18
// -----------------------------------------------------------------------------
Decisions::Decisions( kernel::Controller& controller,
                      const kernel::Entity_ABC& entity,
                      const kernel::DecisionalModel& decisionalModel )
    : controller_( controller )
    , model_( &decisionalModel )
    , entity_( entity )
    , currentMission_( 0 )
    , brainDebug_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Decisions destructor
// Created: ABR 2013-12-18
// -----------------------------------------------------------------------------
Decisions::~Decisions()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Decisions::IsDebugActivated
// Created: ABR 2013-12-18
// -----------------------------------------------------------------------------
bool Decisions::IsDebugActivated() const
{
    return brainDebug_;
}

// -----------------------------------------------------------------------------
// Name: Decisions::GetMissions
// Created: ABR 2013-12-18
// -----------------------------------------------------------------------------
tools::Iterator< const kernel::Mission& > Decisions::GetMissions() const
{
    return model_->tools::Resolver< kernel::Mission >::CreateIterator();
}

// -----------------------------------------------------------------------------
// Name: Decisions::GetFragOrders
// Created: ABR 2013-12-18
// -----------------------------------------------------------------------------
tools::Iterator< const kernel::FragOrder& > Decisions::GetFragOrders() const
{
    return model_->tools::Resolver< kernel::FragOrder >::CreateIterator();
}

// -----------------------------------------------------------------------------
// Name: Decisions::GetCurrentMission
// Created: ABR 2013-12-18
// -----------------------------------------------------------------------------
const kernel::Mission* Decisions::GetCurrentMission() const
{
    return currentMission_;
}

// -----------------------------------------------------------------------------
// Name: Decisions::GetAgent
// Created: ABR 2013-12-18
// -----------------------------------------------------------------------------
const kernel::Entity_ABC& Decisions::GetAgent() const
{
    return entity_;
}

// -----------------------------------------------------------------------------
// Name: Decisions::GetModelName
// Created: ABR 2013-12-18
// -----------------------------------------------------------------------------
const std::string& Decisions::GetModelName() const
{
    return model_->GetName();
}
