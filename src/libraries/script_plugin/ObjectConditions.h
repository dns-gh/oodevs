// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ObjectConditions_h_
#define __ObjectConditions_h_

#include "dispatcher/Registrable_ABC.h"
#include <boost/shared_ptr.hpp>

namespace kernel
{
    class Controller;
}

namespace dispatcher
{
    class Object_ABC;
}

namespace plugins
{
namespace script
{
    class Condition_ABC;

// =============================================================================
/** @class  ObjectConditions
    @brief  Object conditions
*/
// Created: SBO 2010-07-13
// =============================================================================
class ObjectConditions : public dispatcher::Registrable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit ObjectConditions( kernel::Controller& controller );
    virtual ~ObjectConditions();
    //@}

    //! @name Operations
    //@{
    virtual void RegisterIn( directia::brain::Brain& brain );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ObjectConditions( const ObjectConditions& );            //!< Copy constructor
    ObjectConditions& operator=( const ObjectConditions& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    boost::shared_ptr< Condition_ABC > ObjectDestroyed();
    boost::shared_ptr< Condition_ABC > ObjectKnowledgeCreated();
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    //@}
};

}
}

#endif // __ObjectConditions_h_
