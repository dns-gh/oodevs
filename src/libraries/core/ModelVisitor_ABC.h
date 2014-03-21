// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef CORE_MODEL_VISITOR_ABC_H
#define CORE_MODEL_VISITOR_ABC_H

#include "Types.h"
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <string>

namespace core
{
    class UserData_ABC;
    class Model;

// =============================================================================
/** @class  ModelVisitor_ABC
    @brief  Model visitor declaration
*/
// Created: SLI 2011-12-06
// =============================================================================
class ModelVisitor_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             ModelVisitor_ABC() {}
    virtual ~ModelVisitor_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Visit( int64_t value ) = 0;
    virtual void Visit( uint64_t value ) = 0;
    virtual void Visit( double value ) = 0;
    virtual void Visit( const std::string& value ) = 0;

    virtual void Visit( const std::string& key, const core::Model& child ) = 0;
    virtual void Visit( std::size_t key, const core::Model& child ) = 0;

    virtual void Visit( const core::Model& element ) = 0;

    virtual void Visit( const boost::shared_ptr< core::UserData_ABC >& data ) = 0;
    //@}
};

}

#endif // CORE_MODEL_VISITOR_ABC_H
