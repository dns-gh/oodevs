// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __transport_HumanLoadingTimeComputer_ABC_H__
#define __transport_HumanLoadingTimeComputer_ABC_H__

namespace transport
{

// =============================================================================
/** @class  ConsumptionChangeRequestHandler_ABC
    @brief  ConsumptionChangeRequestHandler_ABC
*/
// Created: AHC 2009-10-01
// =============================================================================
class HumanLoadingTimeComputer_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             HumanLoadingTimeComputer_ABC() {}
    virtual ~HumanLoadingTimeComputer_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Reset() = 0;
    virtual void AddHumans( int humansCount ) = 0;
    virtual void AddTime( double loadingPerTimeStep, double unloadingPerTimeStep ) = 0;
    virtual int GetHumansCount() const = 0;
    virtual double GetHumansLoadedPerTimeStep() const = 0;
    virtual double GetHumansUnloadedPerTimeStep() const = 0;
    //@}
};

}

#endif /* __transport_HumanLoadingTimeComputer_ABC_H__ */
