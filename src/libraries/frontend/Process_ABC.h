// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __Process_ABC_h_
#define __Process_ABC_h_

#include <boost/enable_shared_from_this.hpp>
#include <string>

class QString;

namespace frontend
{

// =============================================================================
/** @class  Process_ABC
    @brief  Process_ABC
*/
// Created: SBO 2008-10-14
// =============================================================================
class Process_ABC : public boost::enable_shared_from_this< frontend::Process_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             Process_ABC() {}
    virtual ~Process_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual unsigned int GetPercentage() const = 0;
    virtual QString      GetStatus() const = 0;
    virtual std::string  GetStartedExercise() const = 0;
    virtual std::string  GetExercise() const = 0;
    virtual std::string  GetSession() const = 0;
    //@}
};

}

#endif // __Process_ABC_h_
