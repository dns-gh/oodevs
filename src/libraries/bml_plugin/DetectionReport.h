// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __DetectionReport_h_
#define __DetectionReport_h_

#include "game_asn/Simulation.h"

namespace xml
{
    class xostream;
}

namespace dispatcher
{
    class Agent;
}

namespace bml
{
    class Publisher_ABC;

// =============================================================================
/** @class  DetectionReport
    @brief  DetectionReport
*/
// Created: SBO 2008-07-22
// =============================================================================
class DetectionReport
{

public:
    //! @name Constructors/Destructor
    //@{
             DetectionReport( const dispatcher::Agent& entity, const dispatcher::Agent& detected, int level );
    virtual ~DetectionReport();
    //@}

    //! @name Operations
    //@{
    void Send( Publisher_ABC& publisher ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    DetectionReport( const DetectionReport& );            //!< Copy constructor
    DetectionReport& operator=( const DetectionReport& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    const dispatcher::Agent& entity_;
    const dispatcher::Agent& detected_;
    int level_;
    //@}
};

}

#endif // __DetectionReport_h_
