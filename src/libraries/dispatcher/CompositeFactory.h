// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __CompositeFactory_h_
#define __CompositeFactory_h_

#include "ExtensionFactory_ABC.h"
#include "Factory_ABC.h"
#include <tools/Extendable.h>
#include <tools/SortedInterfaceContainer.h>

namespace dispatcher
{
// =============================================================================
/** @class  CompositeFactory
    @brief  CompositeFactory
*/
// Created: SBO 2008-02-13
// =============================================================================
class CompositeFactory : public tools::SortedInterfaceContainer< Factory_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             CompositeFactory();
    virtual ~CompositeFactory();
    //@}

    //! @name Operations
    //@{
    using tools::SortedInterfaceContainer< Factory_ABC >::Register;
    using tools::SortedInterfaceContainer< Factory_ABC >::Unregister;

    template< typename T >
    void Create( T& entity )
    {
        Apply( & ExtensionFactory_ABC< T >::Create, entity );
    }
    //@}

private:
    //! @name Copy/Assignment
    //@{
    CompositeFactory( const CompositeFactory& );            //!< Copy constructor
    CompositeFactory& operator=( const CompositeFactory& ); //!< Assignment operator
    //@}
};

}

#endif // __CompositeFactory_h_
