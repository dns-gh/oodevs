// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __transport_LoadingComputerFactory_ABC_H__
#define __transport_LoadingComputerFactory_ABC_H__

namespace transport
{
    class HumanLoadingTimeComputer_ABC;
    class LoadedStateConsistencyComputer_ABC;

// =============================================================================
/** @class  ConsumptionChangeRequestHandler_ABC
    @brief  ConsumptionChangeRequestHandler_ABC
*/
// Created: AHC 2009-10-01
// =============================================================================
class LoadingComputerFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             LoadingComputerFactory_ABC() {}
    virtual ~LoadingComputerFactory_ABC() {}
    //@}

    //! @name Constructors/Destructor
    //@{
    virtual std::auto_ptr< HumanLoadingTimeComputer_ABC > CreateHumanLoadingTimeComputer() = 0;
    virtual std::auto_ptr< LoadedStateConsistencyComputer_ABC > CreateLoadedStateConsistencyComputer() = 0;
    //@}
};

}

#endif /* __transport_LoadingComputerFactory_ABC_H__ */
