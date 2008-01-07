// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __App_h_
#define __App_h_

#include <string>

namespace dispatcher
{
    class Replayer;
}

// =============================================================================
/** @class  App
    @brief  My first application
*/
// Created: AGE 2007-04-10
// =============================================================================
class App
{

public:
    //! @name Constructors/Destructor
    //@{
             App( int argc, char** argv );
    virtual ~App();
    //@}

    //! @name Operations
    //@{
    void Execute(); 
    //@}

private:
    //! @name Copy/Assignment
    //@{
    App( const App& );            //!< Copy constructor
    App& operator=( const App& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    dispatcher::Replayer* replayer_;
    //@}
};

#endif // __App_h_
