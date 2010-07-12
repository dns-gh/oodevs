// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __transport_DefaultLoadedStateConsistencyComputer_H__
#define __transport_DefaultLoadedStateConsistencyComputer_H__

#include "LoadedStateConsistencyComputer_ABC.h"

namespace transport
{

// =============================================================================
/** @class  DefaultLoadedStateConsistencyComputer
    @brief  DefaultLoadedStateConsistencyComputer
*/
// Created: AHC 2009-30-09
// =============================================================================
class DefaultLoadedStateConsistencyComputer: public transport::LoadedStateConsistencyComputer_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             DefaultLoadedStateConsistencyComputer();
    virtual ~DefaultLoadedStateConsistencyComputer();
    //@}

    //! @name Operations
    //@{
    void Reset();
    void EnableCarrier(bool value);
    void EnableLoadable(bool value);
    bool HasValidCarrier() const;
    bool HasValidLoadable() const;
    //@}

private:
    //! @name Attributes
    //@{
    bool hasValidCarrier_;
    bool hasValidLoadable_;
    //@}
};

}

#endif /* __transport_DefaultLoadedStateConsistencyComputer_H__ */
