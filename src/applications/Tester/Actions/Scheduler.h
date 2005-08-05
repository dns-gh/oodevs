// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-08-04 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#ifndef __Scheduler_h_
#define __Scheduler_h_

#ifdef __GNUG__
#   pragma interface
#endif

namespace TEST {

    class Action_ABC;

// =============================================================================
/** @class  Scheduler
    @brief  Scheduler
    @par    Using example
    @code
    Scheduler;
    @endcode
*/
// Created: SBO 2005-08-04
// =============================================================================
class Scheduler
{

public:
    //! @name Constructors/Destructor
    //@{
             Scheduler();
    virtual ~Scheduler();
    //@}

    //! @name Operations
    //@{
    bool     Run      ();
    void     AddAction( Action_ABC& action );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Scheduler( const Scheduler& );            //!< Copy constructor
    Scheduler& operator=( const Scheduler& ); //!< Assignment operator
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< uint, Action_ABC* > T_ActionMap;
    typedef T_ActionMap::iterator         IT_ActionMap;
    //@}

private:
    //! @name Member data
    //@{
    T_ActionMap  actions_;
    IT_ActionMap itCurrentAction_;
    //@}
};

} // end namespace TEST

#include "Scheduler.inl"

#endif // __Scheduler_h_
