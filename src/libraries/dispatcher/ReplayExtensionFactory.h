// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ReplayExtensionFactory_h_
#define __ReplayExtensionFactory_h_

#include "ExtensionFactory_ABC.h"

namespace kernel
{
    class Entity_ABC;
}

namespace dispatcher
{
    
    class ReplayModel_ABC;

// =============================================================================
/** @class  ReplayExtensionFactory
    @brief  ReplayExtensionFactory
*/
// Created: SBO 2008-02-14
// =============================================================================
class ReplayExtensionFactory : public ExtensionFactory_ABC< kernel::Entity_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ReplayExtensionFactory( const ReplayModel_ABC& model );
    virtual ~ReplayExtensionFactory();
    //@}

    //! @name Operations
    //@{
    virtual void Create( kernel::Entity_ABC& entity );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ReplayExtensionFactory( const ReplayExtensionFactory& );            //!< Copy constructor
    ReplayExtensionFactory& operator=( const ReplayExtensionFactory& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const ReplayModel_ABC& model_;
    //@}
};

}

#endif // __ReplayExtensionFactory_h_
