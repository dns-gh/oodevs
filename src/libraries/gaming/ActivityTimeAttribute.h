// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActivityTimeAttribute_h_
#define __ActivityTimeAttribute_h_

#include "clients_kernel/ObjectExtensions.h"
#include "clients_kernel/Drawable_ABC.h"

namespace kernel
{
    class Controller;
    class Displayer_ABC;
}

// =============================================================================
/** @class  ActivityTimeAttribute
    @brief  ActivityTimeAttribute
*/
// Created: SBO 2007-02-08
// =============================================================================
class ActivityTimeAttribute : public kernel::ActivityTimeAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit ActivityTimeAttribute( kernel::Controller& controller );
    virtual ~ActivityTimeAttribute();
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
    ActivityTimeAttribute( const ActivityTimeAttribute& );            //!< Copy constructor
    ActivityTimeAttribute& operator=( const ActivityTimeAttribute& ); //!< Assignment operator
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

#endif // __ActivityTimeAttribute_h_
