// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef MODULE_TESTER_IS_EMPTY_MODEL_VISITOR_H
#define MODULE_TESTER_IS_EMPTY_MODEL_VISITOR_H

#include <core/Model.h>
#include <core/ModelVisitor_ABC.h>
#include <stdexcept>

namespace sword
{
namespace test
{
// =============================================================================
/** @class  IsEmptyModelVisitor
    @brief  Model visitor to check whether it's empty or not
*/
// Created: MCO 2012-09-20
// =============================================================================
class IsEmptyModelVisitor : private core::ModelVisitor_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit IsEmptyModelVisitor( const core::Model& model )
        : empty_( true )
    {
        model.Accept( *this );
    }
    virtual ~IsEmptyModelVisitor() {}
    //@}

    //! @name Accessors
    //@{
    operator bool() const
    {
        return empty_;
    }
    //@}

private:
    //! @name Operations
    //@{
    virtual void Visit( int64_t /*value*/ )
    {
        empty_ = false;
    }
    virtual void Visit( uint64_t /*value*/ )
    {
        empty_ = false;
    }
    virtual void Visit( double /*value*/ )
    {
        empty_ = false;
    }
    virtual void Visit( const std::string& /*value*/ )
    {
        empty_ = false;
    }

    virtual void Visit( const std::string& /*key*/, const core::Model& /*child*/ )
    {
        empty_ = false;
    }
    virtual void Visit( std::size_t /*key*/, const core::Model& /*child*/ )
    {
        empty_ = false;
    }

    virtual void Visit( const core::Model& /*element*/ )
    {
        empty_ = false;
    }

    virtual void Visit( const boost::shared_ptr< core::UserData_ABC >& /*data*/ )
    {
        empty_ = false;
    }
    //@}

private:
    //! @name Member data
    //@{
    bool empty_;
    //@}
};

}
}

#endif // MODULE_TESTER_IS_EMPTY_MODEL_VISITOR_H
