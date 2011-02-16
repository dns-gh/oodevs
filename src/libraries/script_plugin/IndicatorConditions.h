// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __IndicatorConditions_h_
#define __IndicatorConditions_h_

#include "dispatcher/Registrable_ABC.h"
#include <boost/shared_ptr.hpp>

namespace kernel
{
    class Controller;
}

namespace dispatcher
{
    class Config;
}

namespace plugins
{
namespace script
{
    class Condition_ABC;
    class Controller;

// =============================================================================
/** @class  IndicatorConditions
    @brief  IndicatorConditions
*/
// Created: SBO 2009-06-03
// =============================================================================
class IndicatorConditions : public dispatcher::Registrable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             IndicatorConditions( kernel::Controller& controller, const dispatcher::Config& config );
    virtual ~IndicatorConditions();
    //@}

    //! @name Operations
    //@{
    virtual void RegisterIn( directia::brain::Brain& brain );
    std::string PrependSessionPath( const std::string& filename );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    IndicatorConditions( const IndicatorConditions& );            //!< Copy constructor
    IndicatorConditions& operator=( const IndicatorConditions& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    boost::shared_ptr< Condition_ABC > IndicatorChanged();
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const dispatcher::Config& config_;
    //@}
};

}
}

#endif // __IndicatorConditions_h_
