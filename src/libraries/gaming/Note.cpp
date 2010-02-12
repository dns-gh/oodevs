// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "gaming_pch.h"
#include "Note.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_gui/Tools.h"


// -----------------------------------------------------------------------------
// Name: Note constructor
// Created: HBD 2010-01-14
// -----------------------------------------------------------------------------
Note::Note(kernel::Controller& controller, unsigned int parent, unsigned int id, const std::string name, const std::string number, const std::string  desc)
:             controller_( controller)  
            , parent_( parent )
            , name_( name )
            , noteText_( desc )
            , noteId_(id)
            , noteNumber_(number)
{
    controller_.Create(*this);

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
void Note::Update(const ASN1T_MsgNoteUpdate& asn)
{
    if ( asn.m.namePresent )
        name_ = asn.name;
    if ( asn.m.numberPresent )
        noteNumber_ = asn.number;
    if ( asn.m.descriptionPresent )
        noteText_ = asn.description;
    if ( asn.m.parentPresent )
        parent_ = asn.parent;
    controller_.Update(*this);
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
// Name: Score::Display
/** @param  displayer 
*/
// Created: HBD 2010-01-19
// -----------------------------------------------------------------------------
void Note::Display( kernel::Displayer_ABC& displayer ) const
{
    displayer.Display( tools::translate( "Note", "Tree" ), name_ )
        .Display( tools::translate( "Note", "Value" ), this->noteNumber_ )
        .Display( tools::translate( "Note", "Text" ), this->noteText_ );
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
