// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __edxl_ExtensionFactory_h_
#define __edxl_ExtensionFactory_h_

#include "dispatcher/ExtensionFactory_ABC.h"

namespace dispatcher
{
    class Object;
    class Model_ABC;
}

namespace plugins
{
namespace edxl
{
    class ReportBuilder_ABC;

// =============================================================================
/** @class  ExtensionFactory
    @brief  ExtensionFactory
*/
// Created: SBO 2008-02-29
// =============================================================================
class ExtensionFactory : public dispatcher::ExtensionFactory_ABC< dispatcher::Object >
{

public:
    //! @name Constructors/Destructor
    //@{
             ExtensionFactory( ReportBuilder_ABC& builder, const dispatcher::Model_ABC& model );
    virtual ~ExtensionFactory();
    //@}

    //! @name Operations
    //@{
    virtual void Create( dispatcher::Object& object );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ExtensionFactory( const ExtensionFactory& );            //!< Copy constructor
    ExtensionFactory& operator=( const ExtensionFactory& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    ReportBuilder_ABC& builder_;
    const dispatcher::Model_ABC& model_;
    //@}
};

}
}

#endif // __ExtensionFactory_h_
