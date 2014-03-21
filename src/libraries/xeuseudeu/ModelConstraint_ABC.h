// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef MODULE_TESTER_MODEL_CONSTRAINT_ABC_H
#define MODULE_TESTER_MODEL_CONSTRAINT_ABC_H

#include <boost/noncopyable.hpp>
#include <iosfwd>

namespace core
{
    class Model;
}

namespace sword
{
namespace test
{
    class Model;

// =============================================================================
/** @class  ModelConstraint_ABC
    @brief  Model constraint
*/
// Created: MCO 2012-09-20
// =============================================================================
class ModelConstraint_ABC : boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             ModelConstraint_ABC() {}
    virtual ~ModelConstraint_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual bool Check( const Model& expected, const core::Model& actual ) = 0;

    virtual void Serialize( const Model& expected, std::ostream& os, std::size_t indent ) const = 0;
    //@}
};

}
}

#endif // MODULE_TESTER_MODEL_CONSTRAINT_ABC_H
