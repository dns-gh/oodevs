// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __transport_LoadedStateConsistencyComputer_ABC_H__
#define __transport_LoadedStateConsistencyComputer_ABC_H__

namespace transport
{

// =============================================================================
/** @class  LoadedStateConsistencyComputer_ABC
    @brief  LoadedStateConsistencyComputer_ABC
*/
// Created: AHC 2009-10-01
// =============================================================================
class LoadedStateConsistencyComputer_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             LoadedStateConsistencyComputer_ABC() {}
    virtual ~LoadedStateConsistencyComputer_ABC() {}
    //@}

    //! @name Attributes
    //@{
    virtual void Reset() = 0;
    virtual void EnableCarrier( bool value ) = 0;
    virtual void EnableLoadable( bool value ) = 0;
    virtual bool HasValidCarrier() const = 0;
    virtual bool HasValidLoadable() const = 0;
    //@}
};

}

#endif /* __transport_LoadedStateConsistencyComputer_ABC_H__ */
