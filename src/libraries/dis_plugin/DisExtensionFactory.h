// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DisExtensionFactory_h_
#define __DisExtensionFactory_h_

#include "dispatcher/ExtensionFactory_ABC.h"

namespace dispatcher
{
    class Agent;
}

namespace dis
{
    class UdpNetwork;
    class Time_ABC;

// =============================================================================
/** @class  DisExtensionFactory
    @brief  DisExtensionFactory
*/
// Created: AGE 2008-03-10
// =============================================================================
class DisExtensionFactory : public dispatcher::ExtensionFactory_ABC< dispatcher::Agent >
{

public:
    //! @name Constructors/Destructor
    //@{
             DisExtensionFactory( UdpNetwork& network, const Time_ABC& time );
    virtual ~DisExtensionFactory();
    //@}

    //! @name Operations
    //@{
    virtual void Create( dispatcher::Agent& entity );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    DisExtensionFactory( const DisExtensionFactory& );            //!< Copy constructor
    DisExtensionFactory& operator=( const DisExtensionFactory& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    UdpNetwork& network_;
    const Time_ABC& time_;
    unsigned short id_;
    //@}
};

}

#endif // __DisExtensionFactory_h_
