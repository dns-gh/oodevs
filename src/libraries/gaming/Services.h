// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Services_h_
#define __Services_h_

#include "clients_kernel/Services.h"

namespace kernel
{
    class Controller;
    class Logger_ABC;
}

namespace sword
{
    class ServicesDescription;
}

// =============================================================================
/** @class  Services
    @brief  Services
*/
// Created: AGE 2008-08-13
// =============================================================================
class Services : public boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             Services( kernel::Controller& controller, kernel::Logger_ABC& logger );
    virtual ~Services();
    //@}

    //! @name Read methods
    //@{
    bool HasService( sword::EnumService id ) const;
    bool RequireService( sword::EnumService id ) const;
    //@}

    //! @name Read methods
    //@{
    void Update( const sword::ServicesDescription& message );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    kernel::Logger_ABC& logger_;
    kernel::Services services_;
    //@}
};

#endif // __Services_h_
