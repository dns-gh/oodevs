// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __CommandPublisher_h_
#define __CommandPublisher_h_

#include "tools/ElementObserver_ABC.h"
#include "clients_kernel/SafePointer.h"

namespace kernel
{
    class Profile_ABC;
}

class Publisher_ABC;
class Services;

// =============================================================================
/** @class  CommandPublisher
    @brief  CommandPublisher
*/
// Created: SBO 2008-06-11
// =============================================================================
class CommandPublisher : public tools::Observer_ABC
                       , public tools::ElementObserver_ABC< kernel::Profile_ABC >
                       , public tools::ElementObserver_ABC< Services >
{

public:
    //! @name Constructors/Destructor
    //@{
             CommandPublisher( kernel::Controllers& controllers, Publisher_ABC& publisher );
             CommandPublisher( kernel::Controllers& controllers, Publisher_ABC& publisher, const kernel::Profile_ABC& profile );
    virtual ~CommandPublisher();
    //@}

    //! @name Operations
    //@{
    void Send( const std::string& target, const std::string& message );
    QString SelfProfile() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    CommandPublisher( const CommandPublisher& );            //!< Copy constructor
    CommandPublisher& operator=( const CommandPublisher& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const kernel::Profile_ABC& profile );
    virtual void NotifyUpdated( const Services& services );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    Publisher_ABC& publisher_;
    kernel::SafePointer< kernel::Profile_ABC > profile_;
    bool messenger_;
    //@}
};

#endif // __CommandPublisher_h_
