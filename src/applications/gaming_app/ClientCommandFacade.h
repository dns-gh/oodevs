// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ClientCommandFacade_h_
#define __ClientCommandFacade_h_

namespace kernel
{
    class Controllers;
}

class CommandPublisher;
class Publisher_ABC;

// =============================================================================
/** @class  ClientCommandFacade
    @brief  ClientCommandFacade
*/
// Created: SBO 2008-07-07
// =============================================================================
class ClientCommandFacade : public QObject
{
public:
    //! @name Constructors/Destructor
    //@{
             ClientCommandFacade( QObject* parent, kernel::Controllers& controllers, Publisher_ABC& publisher );
    virtual ~ClientCommandFacade();
    //@}

private:
    //! @name Member data
    //@{
    std::unique_ptr< CommandPublisher > publisher_;
    //@}
};

#endif // __ClientCommandFacade_h_
