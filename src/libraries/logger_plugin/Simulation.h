// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __Simulation_h_
#define __Simulation_h_

#include "clients_kernel/Time_ABC.h"

namespace sword
{
    class ControlInformation;
    class ControlBeginTick;
    class ControlEndTick;
}

namespace plugins
{
namespace logger
{

// =============================================================================
/** @class  Simulation
    @brief  Simulation
*/
// Created: SBO 2010-05-11
// =============================================================================
class Simulation : public kernel::Time_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Simulation();
    virtual ~Simulation();
    //@}

    //! @name Operations
    //@{
    void Update( const sword::ControlInformation& message );
    void Update( const sword::ControlBeginTick& message );

    virtual QDateTime GetDateTime() const;
    virtual QDateTime GetInitialDateTime() const;
    virtual unsigned int GetTickDuration() const;
    virtual QString GetTimeAsString() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Simulation( const Simulation& );            //!< Copy constructor
    Simulation& operator=( const Simulation& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    QDateTime initial_;
    QDateTime current_;
    unsigned int tickDuration_;
    //@}
};

}
}

#endif // __Simulation_h_
