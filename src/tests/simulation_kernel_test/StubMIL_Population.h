#ifndef __StubMIL_Population_h_
#define __StubMIL_Population_h_

#include "Entities/Populations/MIL_Population.h"

class StubMIL_Population : public MIL_Population
{
public:
             StubMIL_Population( MIL_PopulationType& type, MIL_Army_ABC& army )
                 : MIL_Population( type )
                 , army_( army )
             {
                 // NOTHING
             }
    virtual ~StubMIL_Population()
             {
                // NOTHING
             }
    virtual MIL_Army_ABC& GetArmy() const
    {
        return army_;
    }
private:
     MIL_Army_ABC& army_;
};

#endif __StubMIL_Population_h_
