// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef WRAPPER_REMOVE_H
#define WRAPPER_REMOVE_H

#include <module_api/Model.h>
#include <boost/noncopyable.hpp>

namespace sword
{
namespace wrapper
{
    class View;

// =============================================================================
/** @class  Remove
    @brief  Remove
*/
// Created: MCO 2012-10-12
// =============================================================================
class Remove : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit Remove( const View& target );
    //@}

    //! @name Operations
    //@{
    void Post() const; // $$$$ MCO : should it be done by the destructor (it cannot throw) ?
    //@}

private:
    //! @name Member data
    //@{
    const SWORD_Model* target_;
    //@}
};

}
}

#endif // WRAPPER_REMOVE_H
