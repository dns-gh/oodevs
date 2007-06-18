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

#include "game_asn/Asn.h"
#include "Localisation.h"

namespace dispatcher
{
    class Model;

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
     PopulationFlowPart( const Model& model, const ASN1T_FlowPart& asnMsg );
    ~PopulationFlowPart();
    //@}

    //! @name Operations
    //@{
    void Send( ASN1T_FlowPart& asnMsg ) const ;

    static void AsnDelete( ASN1T_FlowPart& asnMsg );
    //@}

private:
    const Localisation localisation_;
    const unsigned int nRelevance_;
};

}

#endif // __PopulationFlowPart_h_
