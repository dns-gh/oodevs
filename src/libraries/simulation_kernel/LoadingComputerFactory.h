// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __transport_LoadingComputerFactory_H__
#define __transport_LoadingComputerFactory_H__

#include "LoadingComputerFactory_ABC.h"

namespace transport
{

// =============================================================================
/** @class  LoadingComputerFactory
    @brief  LoadingComputerFactory
*/
// Created: AHC 2009-10-01
// =============================================================================
class LoadingComputerFactory: public transport::LoadingComputerFactory_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    LoadingComputerFactory();
    virtual ~LoadingComputerFactory();
    //@}

    //! @name Operations
    //@{
    HumanLoadingTimeComputer_ABC& CreateHumanLoadingTimeComputer();
    LoadedStateConsistencyComputer_ABC& CreateLoadedStateConsistencyComputer();
    //@}
private:
    //! @name Attributes
    //@{
    std::auto_ptr<HumanLoadingTimeComputer_ABC> humanLoadingTimeComputer_;
    std::auto_ptr<LoadedStateConsistencyComputer_ABC> loadedStateConsistencyComputer_;
    //@}
};

}

#endif /* __transport_LoadingComputerFactory_H__ */
