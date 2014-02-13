// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __actions_ActionTiming_h_
#define __actions_ActionTiming_h_

#include "clients_kernel/Displayable_ABC.h"
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Serializable_ABC.h"
#include <boost/noncopyable.hpp>
#pragma warning( push, 0 )
#include <QtCore/qdatetime.h>
#pragma warning( pop )

namespace xml
{
    class xostream;
}

namespace kernel
{
    class Controller;
    class Time_ABC;
}

namespace actions
{

// =============================================================================
/** @class  ActionTiming
    @brief  ActionTiming
*/
// Created: SBO 2007-06-19
// =============================================================================
class ActionTiming : public kernel::Extension_ABC
                   , public kernel::Serializable_ABC
                   , public kernel::Displayable_ABC
                   , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             ActionTiming( kernel::Controller& controller, const kernel::Time_ABC& simulation );
             ActionTiming( kernel::Controller& controller, const kernel::Time_ABC& simulation, const std::string& datetime );
    virtual ~ActionTiming();
    //@}

    //! @name Operations
    //@{
    void ToggleEnabled();
    virtual void SerializeAttributes( xml::xostream& ) const;
    void Shift( long secs );
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayInSummary( kernel::Displayer_ABC& displayer ) const;
    //@}

    //! @name Accessors
    //@{
    bool IsEnabled() const;
    QDateTime GetTime() const;
    void SetTime( const QDateTime& time );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const kernel::Time_ABC& simulation_;
    bool enabled_;
    QDateTime time_;
    //@}
};

}

#endif // __actions_ActionTiming_h_
