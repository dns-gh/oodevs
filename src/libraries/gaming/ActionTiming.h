// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionTiming_h_
#define __ActionTiming_h_

#include "clients_kernel/Extension_ABC.h"

namespace xml
{
    class xistream;
    class xostream;
}

namespace kernel
{
    class Controller;
}

class Action_ABC;
class Simulation;

// =============================================================================
/** @class  ActionTiming
    @brief  ActionTiming
*/
// Created: SBO 2007-06-19
// =============================================================================
class ActionTiming : public kernel::Extension_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ActionTiming( kernel::Controller& controller, unsigned long startTime, const Action_ABC& owner );
             ActionTiming( kernel::Controller& controller, const Simulation& simulation, const Action_ABC& owner );
             ActionTiming( xml::xistream& xis, kernel::Controller& controller, const Action_ABC& owner );
    virtual ~ActionTiming();
    //@}

    //! @name Operations
    //@{
    void ToggleEnabled();
    void Serialize( xml::xostream& xos ) const;
    void Shift( long shift );
    //@}

    //! @name Accessors
    //@{
    const Action_ABC& GetAction() const;
    bool IsEnabled() const;
    unsigned long GetTime() const; // $$$$ SBO 2007-07-05: should be a datetime
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionTiming( const ActionTiming& );            //!< Copy constructor
    ActionTiming& operator=( const ActionTiming& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const Action_ABC& owner_;
    bool enabled_;
    unsigned long time_;
    //@}
};

#endif // __ActionTiming_h_
