// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __Exercises_h_
#define __Exercises_h_

#include "tools/Resolver.h"

namespace MsgsLauncherToAdmin
{
    class MsgExercicesListResponse;
}

namespace kernel
{
    class Controller;
}

namespace frontend
{
    class Exercise;

// =============================================================================
/** @class  Exercises
    @brief  Exercises
*/
// Created: SBO 2010-10-01
// =============================================================================
class Exercises : public tools::Resolver< Exercise, std::string >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit Exercises( kernel::Controller& controller );
    virtual ~Exercises();
    //@}

    //! @name Operations
    //@{
    void Update( const MsgsLauncherToAdmin::MsgExercicesListResponse& message );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Exercises( const Exercises& );            //!< Copy constructor
    Exercises& operator=( const Exercises& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    //@}
};

}

#endif // __Exercises_h_
