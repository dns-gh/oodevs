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
    class Config;

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
             Scheduler( const Config& config );
    virtual ~Scheduler();
    //@}

    //! @name Operations
    //@{
    void     RecoverIfNeeded   ( uint nCurrentTick );
    bool     Run               ( uint nCurrentTick );
    void     AddAction         ( Action_ABC& action );
    void     AddAction         ( Action_ABC& action, uint nExecutionTick );
    void     AddActions        ( Action_ABC& action, uint nIteration );
    void     ResetExecutionTick();
    //@}

    //! @name Accessors
    //@{
    uint     GetNextExecutionTick() const;
    uint     GetCurrentTick      () const;
    bool     MustRecomplete      () const;
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
    typedef std::multimap< uint, Action_ABC* > T_ActionMap;
    typedef T_ActionMap::iterator              IT_ActionMap;
    //@}

private:
    //! @name Member data
    //@{
    T_ActionMap  actions_;
    IT_ActionMap itCurrentAction_;
    uint         nNextExecutionTick_;
    uint         nExecutionPeriod_;
    uint         nRecompletionPeriod_;
    int          nCurrentTick_;
    uint         nLastExecutionTick_;
    uint         nSameMissionInterval_;
    uint         nRecoveryTick_;

    // test statistics
    uint         nTestRun_;
    uint         nTestTotal_;
    //@}
};

} // end namespace TEST

#endif // __Scheduler_h_
