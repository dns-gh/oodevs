// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Simulation_h_
#define __Simulation_h_

#include "ASN_Types.h"

class Controller;

// =============================================================================
/** @class  Simulation
    @brief  Simulation
*/
// Created: AGE 2006-02-10
// =============================================================================
class Simulation
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit Simulation(); // $$$$ AGE 2006-02-14: autre chose que juste controller ?
    virtual ~Simulation();
    //@}

    //! @name Operations
    //@{
    void Pause( bool );
    void ChangeSpeed( int timeFactor );
    void Update( const ASN1T_MsgCtrlInfo& asnMsg );
    void BeginTick( int );
    void EndTick();

    int GetTime() const;
    bool IsPaused() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Simulation( const Simulation& );            //!< Copy constructor
    Simulation& operator=( const Simulation& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
//    Controller& controller_;
    unsigned int tickDuration_;
    unsigned int timeFactor_;
    int time_;
    bool paused_;
    bool tickStart_;
    //@}
};

#endif // __Simulation_h_
