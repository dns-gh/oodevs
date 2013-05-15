// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __actions_ActionError_h_
#define __actions_ActionError_h_

#include "clients_kernel/Extension_ABC.h"
#include <boost/noncopyable.hpp>


namespace actions
{
// =============================================================================
/** @class  ActionError
    @brief  ActionError
*/
// Created: LGY 2014-05-14
// =============================================================================
class ActionError : public kernel::Extension_ABC
                  , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             ActionError( const std::string& error );
    virtual ~ActionError();
    //@}

    //! @name Accessors
    //@{
    const std::string& GetError() const;
    //@}

private:
    //! @name Member data
    //@{
    std::string error_;
    //@}
};

}

#endif // __actions_ActionError_h_
