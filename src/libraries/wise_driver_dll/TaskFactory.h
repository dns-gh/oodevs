// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __TaskFactory_h_
#define __TaskFactory_h_

#pragma warning( push, 0 )
#include <wise/wiseinterfacetypes.h>
#pragma warning( pop )

class CWISEDriver;
class Model;
class SwordMessagePublisher_ABC;

// =============================================================================
/** @class  TaskFactory
    @brief  TaskFactory
*/
// Created: SEB 2010-10-15
// =============================================================================
class TaskFactory
{

public:
    //! @name Constructors/Destructor
    //@{
             TaskFactory( const Model& model, SwordMessagePublisher_ABC& publisher );
    virtual ~TaskFactory();
    //@}

    //! @name Operations
    //@{
    void OnCreate( CWISEDriver& driver, const WISE_HANDLE& database, const WISE_HANDLE& handle );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TaskFactory( const TaskFactory& );            //!< Copy constructor
    TaskFactory& operator=( const TaskFactory& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const Model& model_;
    SwordMessagePublisher_ABC& publisher_;
    //@}
};

#endif // __TaskFactory_h_
