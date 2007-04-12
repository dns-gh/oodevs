// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MyFirstApplication_h_
#define __MyFirstApplication_h_

namespace dispatcher
{
    class Replayer;
}

// =============================================================================
/** @class  MyFirstApplication
    @brief  My first application
*/
// Created: AGE 2007-04-10
// =============================================================================
class MyFirstApplication
{

public:
    //! @name Constructors/Destructor
    //@{
             MyFirstApplication( int argc, char** argv );
    virtual ~MyFirstApplication();
    //@}

    //! @name Operations
    //@{
    void Execute(); 
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MyFirstApplication( const MyFirstApplication& );            //!< Copy constructor
    MyFirstApplication& operator=( const MyFirstApplication& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    dispatcher::Replayer* replayer_;
    //@}
};

#endif // __MyFirstApplication_h_
