 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PopulationFlowPart_h_
#define __PopulationFlowPart_h_

#include "Localisation.h"

namespace sword
{
    class MsgFlowPart;
}

namespace dispatcher
{

// =============================================================================
/** @class  PopulationFlowPart
    @brief  PopulationFlowPart
*/
// Created: NLD 2006-09-19
// =============================================================================
class PopulationFlowPart
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit PopulationFlowPart( const sword::MsgFlowPart& asnMsg );
    virtual ~PopulationFlowPart();
    //@}

    //! @name Operations
    //@{
    void Send( sword::MsgFlowPart& asnMsg ) const ;
    //@}

private:
    Localisation localisation_;
    unsigned int nRelevance_;
};

}

#endif // __PopulationFlowPart_h_
