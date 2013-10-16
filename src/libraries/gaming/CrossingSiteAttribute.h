// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __CrossingSiteAttribute_h_
#define __CrossingSiteAttribute_h_

#include "clients_kernel/ObjectExtensions.h"
#include "clients_kernel/OptionalValue.h"
#include <tools/Resolver_ABC.h>

namespace kernel
{
    class Controller;
    class Displayer_ABC;
}

// =============================================================================
/** @class  CrossingSiteAttribute
    @brief  CrossingSiteAttribute
*/
// Created: AGE 2006-02-14
// =============================================================================
class CrossingSiteAttribute : public kernel::CrossingSiteAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit CrossingSiteAttribute( kernel::Controller& controller );
    virtual ~CrossingSiteAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayInSummary( kernel::Displayer_ABC& displayer ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    CrossingSiteAttribute( const CrossingSiteAttribute& );            //!< Copy constructor
    CrossingSiteAttribute& operator=( const CrossingSiteAttribute& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const sword::ObjectKnowledgeUpdate& message );
    virtual void DoUpdate( const sword::ObjectUpdate& message );

    template< typename T >
    void UpdateData( const T& message );
    //@}

public:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    kernel::OptionalValue< unsigned int > width_;
    kernel::OptionalValue< unsigned int > depth_;
    kernel::OptionalValue< unsigned int > speed_;
    kernel::OptionalValue< bool         > needsConstruction_;
    //@}
};

#endif // __CrossingSiteAttribute_h_
