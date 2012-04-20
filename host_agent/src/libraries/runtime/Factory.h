// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************
#ifndef FACTORY_H__
#define FACTORY_H__

#include <memory>
#include <boost/noncopyable.hpp>

namespace cpplog
{
    class BaseLogger;
}

namespace runtime
{
    class Runtime_ABC;

// =============================================================================
/** @class  Factory
    @brief  Runtime factory definition
*/
// Created: BAX 2012-03-08
// =============================================================================
class Factory : public boost::noncopyable
{
public:
    //! @name Constructor/Destructor
    //@{
     Factory( cpplog::BaseLogger& log );
    ~Factory();
    //@}

    //! @name Operations
    //@{
    const Runtime_ABC& GetRuntime() const;
    //@}

private:
    //! @name Private members
    //@{
    struct Private;
    const  std::auto_ptr< Private > private_;
    //@}
};

}

#endif
