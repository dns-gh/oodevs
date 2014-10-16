// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ReplaySynchronizer_h_
#define __ReplaySynchronizer_h_

#include "ReplayModel_ABC.h"

namespace dispatcher
{
    class ClientPublisher_ABC;
    class Model;

// =============================================================================
/** @class  ReplaySynchronizer
    @brief  replay synchronizer
*/
// Created: AGE 2007-04-10
// =============================================================================
class ReplaySynchronizer : public ReplayModel_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ReplaySynchronizer( ClientPublisher_ABC& publisher, Model& model );
    virtual ~ReplaySynchronizer();
    //@}

    //! @name Operations
    //@{
    virtual void StartSynchronisation();
    virtual void EndSynchronisation();
    virtual bool IsSynching() const;
    //@}

private:
    //! @name Member data
    //@{
    ClientPublisher_ABC& publisher_;
    Model&               model_;
    bool                 synching_;
    //@}
};

}

#endif // __ReplaySynchronizer_h_
