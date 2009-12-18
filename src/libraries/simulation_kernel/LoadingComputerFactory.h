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
    virtual std::auto_ptr< HumanLoadingTimeComputer_ABC > CreateHumanLoadingTimeComputer();
    virtual std::auto_ptr< LoadedStateConsistencyComputer_ABC > CreateLoadedStateConsistencyComputer();
    //@}
};

}

#endif /* __transport_LoadingComputerFactory_H__ */
