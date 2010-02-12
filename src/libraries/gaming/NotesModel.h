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
 
#include <qstring.h>
#include "tools/Resolver.h"
#include "tools/Observer_ABC.h"
#include "tools/ElementObserver_ABC.h"
#include "game_asn/Messenger.h"

class Note;
class Services;

namespace kernel
{
    class Controller;
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
    void Create( const ASN1T_MsgNoteCreation& asn );
    void Update( const ASN1T_MsgNoteUpdate& asn );
    void Delete( const ASN1T_MsgNoteDestruction& asn );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    NotesModel( const NotesModel& );            //!< Copy constructor
    NotesModel& operator=( const NotesModel& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{ 
    kernel::Controller& controller_;
    //@}
};

#endif // __NotesModel_h_
