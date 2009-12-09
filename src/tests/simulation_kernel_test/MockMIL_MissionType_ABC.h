#ifndef __MockMIL_MissionType_ABC_h_
#define __MockMIL_MissionType_ABC_h_

#include "Entities/Orders/MIL_MissionType_ABC.h"

class MockMIL_MissionType_ABC 
    : public MIL_MissionType_ABC
    , public mockpp::ChainableMockObject
{
public:
    MockMIL_MissionType_ABC()
        : mockpp::ChainableMockObject( "MockMIL_MissionType_ABC", 0 )
        , GetDIABehavior_mocker( "GetDIABehavior", this )
    {}

    virtual const std::string& GetDIABehavior( MIL_MissionType_ABC::Phase phase ) const
    {
       return *GetDIABehavior_mocker.forward( phase );
    }
    
    mockpp::ChainableMockMethod< std::string*, MIL_MissionType_ABC::Phase > GetDIABehavior_mocker;
};

#endif // __MockMIL_MissionType_ABC_h_
