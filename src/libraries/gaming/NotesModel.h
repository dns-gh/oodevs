// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __NotesModel_h_
#define __NotesModel_h_

#include "protocol/MessengerSenders.h"
#include <tools/Resolver.h>
#include <tools/Observer_ABC.h>
#include <tools/ElementObserver_ABC.h>

class Note;
class Services;

namespace kernel
{
    class Controller;
}

namespace sword
{
    class NoteCreation;
    class NoteUpdate;
    class NoteDestruction;
}

// =============================================================================
/** @class  NotesModel
    @brief  NotesModel
*/
// Created: HBD 2010-01-14
// =============================================================================
class NotesModel : public tools::Resolver< Note, unsigned int >
                 , public tools::Observer_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             NotesModel( kernel::Controller& controller );
    virtual ~NotesModel();
    //@}

    //! @name Operations
    //@{
    void Create( const sword::MarkerCreation&    message );
    void Update( const sword::MarkerUpdate&      message );
    void Delete( const sword::MarkerDestruction& message );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    NotesModel( const NotesModel& );            //!< Copy constructor
    NotesModel& operator=( const NotesModel& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    //@}
};

#endif // __NotesModel_h_
