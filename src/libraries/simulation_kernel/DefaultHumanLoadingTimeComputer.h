// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef DEFAULTHUMANLOADINGTIMECOMPUTER_H_
#define DEFAULTHUMANLOADINGTIMECOMPUTER_H_

#include "HumanLoadingTimeComputer_ABC.h"

namespace transport
{

// =============================================================================
/** @class  DefaultHumanLoadingTimeComputer
    @brief  DefaultHumanLoadingTimeComputer
*/
// Created: AHC 2009-10-01
// =============================================================================
class DefaultHumanLoadingTimeComputer: public transport::HumanLoadingTimeComputer_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    DefaultHumanLoadingTimeComputer();
    virtual ~DefaultHumanLoadingTimeComputer();
    //@}

    //! @name Operations
    //@{
    void Reset();
    void AddHumans(int humansCount);
    void AddTime(double loadingPerTimeStep, double unloadingPerTimeStep);
    int GetHumansCount() const;
    double GetHumansLoadedPerTimeStep() const;
    double GetHumansUnloadedPerTimeStep() const;
    //@}
private:
    //! @name Attributes
    //@{
    int humansCount_;
    double humansLoadedPerTimeStep_;
    double humansUnloadedPerTimeStep_;
    //@}
};

}

#endif /* DEFAULTHUMANLOADINGTIMECOMPUTER_H_ */
