// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.

// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_SimulationTimeManager_ABC_h
#define plugins_hla_SimulationTimeManager_ABC_h

namespace plugins
{
namespace hla
{

class SimulationTimeManager_ABC
{
public:
    virtual ~SimulationTimeManager_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual const std::string& getSimulationTime( ) const= 0;
    virtual unsigned int getSimulationtick() const = 0;
    //@}

};

} // namespace hla
} // namespace plugins
#endif // plugins_hla_SimulationTimeManager_ABC_h