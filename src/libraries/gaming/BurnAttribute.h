// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __BurnAttribute_h_
#define __BurnAttribute_h_

#include "clients_kernel/ObjectExtensions.h"
#include <tools/Resolver_ABC.h>

namespace kernel
{
    class Controller;
}

// =============================================================================
/** @class  BurnAttribute
    @brief  BurnAttribute
*/
// Created: BCI 2010-12-13
// =============================================================================
class BurnAttribute : public kernel::BurnAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             BurnAttribute( kernel::Controller& controller );
    virtual ~BurnAttribute();
    //@}

    //! @name Overrides
    //@{
    virtual void Display         ( kernel::Displayer_ABC& ) const;
    virtual void DisplayInSummary( kernel::Displayer_ABC& ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    BurnAttribute( const BurnAttribute& );            //!< Copy constructor
    BurnAttribute& operator=( const BurnAttribute& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const sword::ObjectKnowledgeUpdate& );
    virtual void DoUpdate( const sword::ObjectUpdate& );

    template< typename T >
    void UpdateData( const T& message );
    //@}

public:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    int currentHeat_;
    int currentCombustionEnergy_;
    //@}
};

#endif // __BurnAttribute_h_
