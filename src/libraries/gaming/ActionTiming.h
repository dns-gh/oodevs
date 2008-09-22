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
#include "clients_kernel/Serializable_ABC.h"

namespace xml
{
    class xistream;
    class xostream;
}

namespace kernel
{
    class Controller;
}

namespace actions 
{
    class Action_ABC;
}

class Simulation;

// =============================================================================
/** @class  ActionTiming
    @brief  ActionTiming
*/
// Created: SBO 2007-06-19
// =============================================================================
class ActionTiming : public kernel::Extension_ABC
                   , public kernel::Serializable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ActionTiming( kernel::Controller& controller, const Simulation& simulation, const actions::Action_ABC& owner );
             ActionTiming( xml::xistream& xis, kernel::Controller& controller, const Simulation& simulation, const actions::Action_ABC& owner );
    virtual ~ActionTiming();
    //@}

    //! @name Operations
    //@{
    void ToggleEnabled();
    virtual void SerializeAttributes( xml::xostream& ) const;
    void Shift( long secs );
    //@}

    //! @name Accessors
    //@{
    const actions::Action_ABC& GetAction() const;
    bool IsEnabled() const;
    QDateTime GetTime() const;
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
    const Simulation& simulation_;
    const actions::Action_ABC& owner_;
    bool enabled_;
    QDateTime time_;
    //@}
};

#endif // __ActionTiming_h_
