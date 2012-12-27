// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Note.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/Tools.h"
#include <boost/date_time/posix_time/posix_time.hpp>

// -----------------------------------------------------------------------------
// Name: Note constructor
// Created: HBD 2010-01-14
// -----------------------------------------------------------------------------
Note::Note( kernel::Controller& controller, const sword::MarkerCreation& message )
    : controller_( controller )
    , parent_( message.definition().parent().id() )
    , name_( message.definition().name() )
    , noteText_( message.definition().description() )
    , noteId_( message.marker().id() )
    , noteNumber_( message.definition().number() )
    , creationTime_ ( message.date().data() )
    , lastUpdateTime_ ( creationTime_ )
{
     controller_.Create( *this );
}

// -----------------------------------------------------------------------------
// Name: Note destructor
// Created: HBD 2010-02-05
// -----------------------------------------------------------------------------
Note::~Note()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Note::Update
// Created: HBD 2010-01-14
// -----------------------------------------------------------------------------
void Note::Update( const sword::MarkerUpdate& message )
{
    if( message.has_name() )
        name_ = message.name();
    if( message.has_number() )
        noteNumber_ = message.number();
    if( message.has_description() )
        noteText_ = message.description();
    if( message.has_parent() )
        parent_ = message.parent().id();
    lastUpdateTime_ = message.date().data();
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Note::Delete
// Created: HBD 2010-02-05
// -----------------------------------------------------------------------------
void Note::Delete()
{
    controller_.Delete( *this );
}

// -----------------------------------------------------------------------------
// Name: Note::GetName
// Created: HBD 2010-02-05
// -----------------------------------------------------------------------------
QString Note::GetName() const
{
    return name_.c_str();
}

// -----------------------------------------------------------------------------
// Name: Note::GetId
// Created: HBD 2010-02-09
// -----------------------------------------------------------------------------
unsigned int Note::GetId() const
{
    return noteId_;
}

// -----------------------------------------------------------------------------
// Name: Note::GetStringCreationTime
// Created: NPT 2012-10-26
// -----------------------------------------------------------------------------
QString Note::GetStringCreationTime() const
{
    return boost::posix_time::to_simple_string( boost::posix_time::from_iso_string( creationTime_ ) ).c_str();
}

// -----------------------------------------------------------------------------
// Name: Note::GetStringLastUpdateTime
// Created: NPT 2012-10-26
// -----------------------------------------------------------------------------
QString Note::GetStringLastUpdateTime() const
{
    return boost::posix_time::to_simple_string( boost::posix_time::from_iso_string( lastUpdateTime_ ) ).c_str();
}

// -----------------------------------------------------------------------------
// Name: Note::GetParent
// Created: HBD 2010-02-09
// -----------------------------------------------------------------------------
unsigned int Note::GetParent() const
{
    return parent_;
}

// -----------------------------------------------------------------------------
// Name: Note::GetDesc
// Created: HBD 2010-02-10
// -----------------------------------------------------------------------------
QString Note::GetDesc() const
{
    return noteText_.c_str();;
}

// -----------------------------------------------------------------------------
// Name: Note::GetNumber
// Created: HBD 2010-02-10
// -----------------------------------------------------------------------------
QString Note::GetNumber() const
{
    return noteNumber_.c_str();
}
