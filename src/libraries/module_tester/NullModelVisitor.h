// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef MODULE_TESTER_NULL_MODEL_VISITOR_H
#define MODULE_TESTER_NULL_MODEL_VISITOR_H

#include <core/ModelVisitor_ABC.h>
#include <stdexcept>

namespace sword
{
namespace test
{
class NullModelVisitor : public core::ModelVisitor_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             NullModelVisitor() {}
    virtual ~NullModelVisitor() {}
    //@}

    //! @name Operations
    //@{
    virtual void Visit( int64_t /*value*/ )
    {}
    virtual void Visit( uint64_t /*value*/ )
    {}
    virtual void Visit( double /*value*/ )
    {}
    virtual void Visit( const std::string& /*value*/ )
    {}

    virtual void Visit( const std::string& /*key*/, const core::Visitable_ABC& /*child*/ )
    {}
    virtual void Visit( unsigned int /*key*/, const core::Visitable_ABC& /*child*/ )
    {}

    virtual void Visit( const core::Visitable_ABC& /*element*/ )
    {}

    virtual void Visit( boost::shared_ptr< core::UserData_ABC > /*data*/ )
    {}
    //@}
};

}
}

#endif // MODULE_TESTER_NULL_MODEL_VISITOR_H
