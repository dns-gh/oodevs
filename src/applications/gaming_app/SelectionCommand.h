// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __SelectionCommand_h_
#define __SelectionCommand_h_

#include <tools/SelectionObserver_ABC.h>

namespace kernel
{
    class Controllers;
    class Entity_ABC;
}

class CommandPublisher;

// =============================================================================
/** @class  SelectionCommand
    @brief  SelectionCommand
*/
// Created: SBO 2008-07-07
// =============================================================================
class SelectionCommand : public QObject
                       , public tools::Observer_ABC
                       , public tools::SelectionObserver< kernel::Entity_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             SelectionCommand( QObject* parent, kernel::Controllers& controllers, CommandPublisher& publisher );
    virtual ~SelectionCommand();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifySelected( const kernel::Entity_ABC* element );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    CommandPublisher& publisher_;
    //@}
};

#endif // __SelectionCommand_h_
