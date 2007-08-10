// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __NET_Simulation_ABC_h_
#define __NET_Simulation_ABC_h_

// =============================================================================
/** @class  NET_Simulation_ABC
    @brief  Simulation definition
*/
// Created: AGE 2007-08-10
// =============================================================================
class NET_Simulation_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             NET_Simulation_ABC() {};
    virtual ~NET_Simulation_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual void SendControlInformation() const = 0;
    virtual void Stop() = 0;
    virtual void Pause() = 0;
    virtual void Resume() = 0;
    virtual void SetTimeFactor( unsigned timeFactor ) = 0;
    //@}
};

#endif // __NET_Simulation_ABC_h_
