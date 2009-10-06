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

class DefaultLoadedStateConsistencyComputer: public transport::LoadedStateConsistencyComputer_ABC
{
public:
    DefaultLoadedStateConsistencyComputer();
    virtual ~DefaultLoadedStateConsistencyComputer();

    void Reset();
    void EnableCarrier(bool value);
    void EnableLoadable(bool value);
    bool HasValidCarrier() const;
    bool HasValidLoadable() const;
private:
    bool hasValidCarrier_;
    bool hasValidLoadable_;
};

}

#endif /* __transport_DefaultLoadedStateConsistencyComputer_H__ */
