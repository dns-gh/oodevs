// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Scheduler_h_
#define __Scheduler_h_

namespace TEST {

    class Action_ABC;
    class Config;

// =============================================================================
/** @class  Scheduler
    @brief  Scheduler
*/
// Created: SBO 2005-08-04
// =============================================================================
class Scheduler
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit Scheduler( const Config& config );
    virtual ~Scheduler();
    //@}

    //! @name Operations
    //@{
    void RecoverIfNeeded( unsigned int nCurrentTick );
    bool Run( unsigned int nCurrentTick );
    void AddAction( Action_ABC& action );
    void AddActions( Action_ABC& action, unsigned int nIteration );
    void ResetExecutionTick();
    //@}

    //! @name Accessors
    //@{
    unsigned int GetNextExecutionTick() const;
    unsigned int GetCurrentTick() const;
    bool MustRecomplete( unsigned int& lastRecompletion ) const;
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
    typedef std::multimap< unsigned int, Action_ABC* > T_ActionMap;
    typedef T_ActionMap::iterator              IT_ActionMap;
    //@}

private:
    //! @name Member data
    //@{
    T_ActionMap  actions_;
    IT_ActionMap itCurrentAction_;
    unsigned int         nNextExecutionTick_;
    unsigned int         nExecutionPeriod_;
    unsigned int         nRecompletionPeriod_;
    int          nCurrentTick_;
    unsigned int         nLastExecutionTick_;
    unsigned int         nSameMissionInterval_;
    unsigned int         nRecoveryTick_;

    // mission per period limiters
    unsigned int         nMissionLimitCounter_;
    unsigned int         nMissionInPeriod_;
    unsigned int         nMaxMissionInPeriod_;

    // test statistics
    unsigned int         nTestRun_;
    unsigned int         nTestTotal_;
    unsigned int         nSimTicks_;
    //@}
};

} // end namespace TEST

#endif // __Scheduler_h_
