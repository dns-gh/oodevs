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
// =============================================================================
/** @class  NullModelVisitor
    @brief  Null model visitor
*/
// Created: MCO 2012-09-20
// =============================================================================
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

    virtual void Visit( const std::string& /*key*/, const core::Model& /*child*/ )
    {}
    virtual void Visit( std::size_t /*key*/, const core::Model& /*child*/ )
    {}

    virtual void Visit( const core::Model& /*element*/ )
    {}

    virtual void Visit( const boost::shared_ptr< core::UserData_ABC >& /*data*/ )
    {}
    //@}
};

}
}

#endif // MODULE_TESTER_NULL_MODEL_VISITOR_H
