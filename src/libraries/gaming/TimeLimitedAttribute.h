// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TimeLimitedAttribute_h_
#define __TimeLimitedAttribute_h_

#include "clients_kernel/ObjectExtensions.h"

namespace kernel
{
    class Controller;
    class Displayer_ABC;
}

// =============================================================================
/** @class  TimeLimitedAttribute
    @brief  TimeLimitedAttribute
*/
// Created: SBO 2007-02-08
// =============================================================================
class TimeLimitedAttribute : public kernel::TimeLimitedAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit TimeLimitedAttribute( kernel::Controller& controller );
    virtual ~TimeLimitedAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayInTooltip( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayInSummary( kernel::Displayer_ABC& displayer ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TimeLimitedAttribute( const TimeLimitedAttribute& );            //!< Copy constructor
    TimeLimitedAttribute& operator=( const TimeLimitedAttribute& ); //!< Assignment operator
    //@}

    //! @name Updatable_ABC
    //@{
    virtual void DoUpdate( const sword::ObjectUpdate& message );
    virtual void DoUpdate( const sword::ObjectKnowledgeUpdate& message );

    template< typename T >
    void UpdateData( const T& message );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    unsigned int activityTime_;
    //@}
};

#endif // __TimeLimitedAttribute_h_
