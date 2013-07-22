// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "SoundEvent.h"

using namespace gui;
// -----------------------------------------------------------------------------
// Name: SoundEvent constructor
// Created: NPT 2013-07-15
// -----------------------------------------------------------------------------
SoundEvent::SoundEvent( const kernel::Entity_ABC* entity, const std::string& soundType, bool stop )
    : entity_( entity )
    , soundType_( soundType )
    , stopSound_( stop )
{
        // NOTHING
}
// -----------------------------------------------------------------------------
// Name: SoundEvent destructor
// Created: NPT 2013-07-15
// -----------------------------------------------------------------------------
SoundEvent::~SoundEvent()
{
        // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SoundEvent::GetEntity
// Created: NPT 2013-07-15
// -----------------------------------------------------------------------------
const kernel::Entity_ABC* SoundEvent::GetEntity() const
{
    return entity_;
}

// -----------------------------------------------------------------------------
// Name: SoundEvent::GetSoundType
// Created: NPT 2013-07-15
// -----------------------------------------------------------------------------
const std::string& SoundEvent::GetSoundType() const
{
    return soundType_;
}

// -----------------------------------------------------------------------------
// Name: SoundEvent::StopSound
// Created: NPT 2013-07-22
// -----------------------------------------------------------------------------
bool SoundEvent::StopSound() const
{
    return stopSound_;
}
