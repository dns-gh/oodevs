#ifndef __StubMIL_PopulationType_h_
#define __StubMIL_PopulationType_h_

#include "Entities/Populations/MIL_PopulationType.h"

class StubMIL_PopulationType : public MIL_PopulationType
{
public:
    StubMIL_PopulationType( const DEC_Model_ABC& model ) : MIL_PopulationType( model, 1. ) {}
    virtual ~StubMIL_PopulationType() {}
};

#endif
