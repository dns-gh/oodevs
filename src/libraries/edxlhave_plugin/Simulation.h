// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __dispatcher_plugins_edxl_Simulation_h_
#define __dispatcher_plugins_edxl_Simulation_h_

namespace xml
{
    class xistream;
}

namespace sword
{
    class ControlInformation;
    class ControlEndTick;
    class ObjectMagicActionAck;
}

namespace plugins
{
namespace edxl
{

// =============================================================================
/** @class  Simulation
    @brief  Simulation
*/
// Created: JCR 2010-05-31
// =============================================================================
class Simulation
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit Simulation( xml::xistream& xis );
    virtual ~Simulation();
    //@}

    //! @name
    //@{
    void Receive( const sword::ControlInformation& message );
    void Receive( const sword::ControlEndTick& message );
    bool NeedUpdate() const;
    void Update( const sword::ControlEndTick& message );
    //@}

    //! @name check init validation
    //@{
    void Receive( const sword::ObjectMagicActionAck& ack );
    bool IsInitDone() const;
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
    unsigned lastUpdate_;
    unsigned current_;
    unsigned frequency_;
    unsigned tick_duration_;
    bool modelLoaded_;
    //@}
};

}
}

#endif // __dispatcher_plugins_edxl_Simulation_h_
