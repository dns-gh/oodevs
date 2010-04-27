#ifndef __MockMIL_MissionType_ABC_h_
#define __MockMIL_MissionType_ABC_h_

#include "Entities/Orders/MIL_MissionType_ABC.h"

MOCK_BASE_CLASS( MockMIL_MissionType_ABC, MIL_MissionType_ABC )
{
    MOCK_METHOD( GetDIABehavior, 1 );
};

#endif // __MockMIL_MissionType_ABC_h_
