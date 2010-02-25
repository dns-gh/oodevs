// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Objective_h_
#define __Objective_h_


#include "Localisation.h"


namespace Common
{
    class MsgMissionObjective;
}

namespace dispatcher
{

// =============================================================================
/** @class  Objective
    @brief  Objective
*/
// Created: NLD 2007-04-23
// =============================================================================
class Objective
{
public:
    //! @name Constructors/Destructor
    //@{
             Objective();
    explicit Objective( const Common::MsgMissionObjective& asn );
    virtual ~Objective();
    //@}

    //! @name Operations
    //@{
    void Send( Common::MsgMissionObjective& asn ) const;
    //@}

private:
    //! @name Member data
    //@{
    Localisation location_;
    std::string  schedule_;
};

}

#endif // __Objective_h_
