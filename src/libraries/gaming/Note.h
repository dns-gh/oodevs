// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __Note_h_
#define __Note_h_

#include "clients_kernel/Displayable_ABC.h"
#include "protocol/MessengerSenders.h"
#include <list>
#include <qlistview.h>

namespace kernel
{
    class Controller;
}

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
             Note( kernel::Controller& controller, const MsgsMessengerToClient::MsgNoteCreation&  );
    virtual ~Note();
    //@}

    //! @name Accessors
    //@{
    QString      GetName() const;
    QString      GetDesc() const;
    QString      GetNumber() const;
    unsigned int GetId() const;
    unsigned int GetParent() const;
    const std::string&  GetCreationTime() const;
    const std::string&  GetLastUpdateTime() const;

    //@}

    //! @name Operations
    //@{
    virtual void Display( QListViewItem* item ) const;
            void Update( const MsgsMessengerToClient::MsgNoteUpdate& message );
            void Delete();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Note( const Note& );            //!< Copy constructor
    Note& operator=( const Note& ); //!< Assignment operator
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
    std::string         creationTime_;
    std::string         lastUpdateTime_;
    //@}

};

#endif // __Note_h_
