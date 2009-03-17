// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Event_h_
#define __Event_h_

#include <boost/shared_ptr.hpp>
#include "directia/ScriptRef.h"

namespace plugins
{
namespace script
{
    class Condition_ABC;

// =============================================================================
/** @class  Event
    @brief  Event
*/
// Created: AGE 2008-06-12
// =============================================================================
class Event
{

public:
    //! @name Constructors/Destructor
    //@{
             Event( boost::shared_ptr< Condition_ABC > condition, const std::vector< std::string >& states, const directia::ScriptRef& function );
    virtual ~Event();
    //@}

    //! @name Operations
    //@{
    void Deactivate();
    void ChangeState( const std::string& state );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Event( const Event& );            //!< Copy constructor
    Event& operator=( const Event& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    boost::shared_ptr< Condition_ABC > condition_;
    std::set< std::string > states_;
    //@}
};

}
}

#endif // __Event_h_
