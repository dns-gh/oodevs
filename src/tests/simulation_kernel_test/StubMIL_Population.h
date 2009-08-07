#ifndef __StubMIL_Population_h_
#define __StubMIL_Population_h_

#include "Entities/Populations/MIL_Population.h"

class StubMIL_Population : public MIL_Population
{
public:
    StubMIL_Population( MIL_PopulationType& type ) : MIL_Population( type ) {}
};

#endif