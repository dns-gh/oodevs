// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// LTO
//
// *****************************************************************************

#ifndef __Note_h_
#define __Note_h_

#include "clients_kernel/Displayable_ABC.h"
#include "protocol/MessengerSenders.h"

#include <list>
namespace kernel
{
    class Controller;
}

typedef std::list< unsigned int >::iterator listIterator; 


// =============================================================================
/** @class  Note
    @brief  Note
*/
// Created: HBD 2010-01-14
// =============================================================================
class Note : public kernel::Displayable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Note(kernel::Controller& controller, unsigned int parent, unsigned int id, const std::string name, const std::string number, const std::string  desc);
    virtual ~Note();
    //@}

    //! @name Accessors
    //@{
    QString      GetName() const;
    QString      GetDesc() const;
    QString      GetNumber() const;
    unsigned int GetId() const;
    unsigned int GetParent() const;

    //@}

    //! @name Operations
    //@{
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
            void Update( const MsgsMessengerToClient::MsgNoteUpdate& message );
            void Delete();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Note( const Note& );            //!< Copy constructor
    Note& operator=( const Note& ); //!< Assignment operator
    //@}
    
    //! @name Helpers
    //@{
    //@}
private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    unsigned int        parent_;
    std::string         name_;
    std::string         noteText_;
    std::string         noteNumber_;
    unsigned int        noteId_;
    //@}

};

#endif // __Note_h_
