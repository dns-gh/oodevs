// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef CORE_MODEL_LISTENER_ABC_H
#define CORE_MODEL_LISTENER_ABC_H

#include <boost/noncopyable.hpp>

namespace core
{
    class Model;

// =============================================================================
/** @class  ModelListener_ABC
    @brief  Model listener definition
*/
// Created: SLI 2011-12-16
// =============================================================================
class ModelListener_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             ModelListener_ABC() {}
    virtual ~ModelListener_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void NotifyChanged( const Model& model ) = 0;
    virtual void NotifyRemoved( const Model& model ) = 0;
    //@}
};

}

#endif // CORE_MODEL_LISTENER_ABC_H
