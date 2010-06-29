// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __actions_ActionTiming_h_
#define __actions_ActionTiming_h_

#include "clients_kernel/Displayable_ABC.h"
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Serializable_ABC.h"
#include <qdatetime.h>

namespace xml
{
    class xistream;
    class xostream;
}

namespace kernel
{
    class Controller;
    class Time_ABC;
}

namespace actions
{
    class Action_ABC;

// =============================================================================
/** @class  ActionTiming
    @brief  ActionTiming
*/
// Created: SBO 2007-06-19
// =============================================================================
class ActionTiming : public kernel::Extension_ABC
                   , public kernel::Serializable_ABC
                   , public kernel::Displayable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ActionTiming( kernel::Controller& controller, const kernel::Time_ABC& simulation, const actions::Action_ABC& owner );
             ActionTiming( xml::xistream& xis, kernel::Controller& controller, const kernel::Time_ABC& simulation, const actions::Action_ABC& owner );
    virtual ~ActionTiming();
    //@}

    //! @name Operations
    //@{
    void ToggleEnabled();
    virtual void SerializeAttributes( xml::xostream& ) const;
    void Shift( long secs );
    virtual void DisplayInSummary( kernel::Displayer_ABC& displayer ) const;
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
    const kernel::Time_ABC& simulation_;
    const actions::Action_ABC& owner_;
    bool enabled_;
    QDateTime time_;
    //@}
};

}

#endif // __actions_ActionTiming_h_
