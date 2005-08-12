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
    bool     Run      ( uint nCurrentTick = 0 );
    void     AddAction( Action_ABC& action, int nExecutionTick = -1 );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Scheduler( const Scheduler& );            //!< Copy constructor
    Scheduler& operator=( const Scheduler& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    uint GetNextExecutionTick();
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
    uint         nNextExecutionTick_;
    //@}
};

} // end namespace TEST

#endif // __Scheduler_h_
