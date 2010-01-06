// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __MockNET_Publisher_ABC_h_
#define __MockNET_Publisher_ABC_h_

#include "Network/NET_Publisher_ABC.h"

// =============================================================================
/** @class  MockNET_Publisher_ABC
    @brief  MockNET_Publisher_ABC
*/
// Created: LDC 2010-01-04
// =============================================================================
class MockNET_Publisher_ABC
    : public mockpp::ChainableMockObject
    , public NET_Publisher_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    MockNET_Publisher_ABC() 
     : mockpp::ChainableMockObject( MOCKPP_PCHAR( "MockNET_Publisher_ABC" ) )
     , Send_mocker                ( "Send", this )
    {
        NET_Publisher_ABC::Register( *this ); 
    }
    virtual ~MockNET_Publisher_ABC() { NET_Publisher_ABC::Unregister( *this ); }
    //@}
    
    virtual void Send( ASN1T_MsgsSimToClient& msg )
    {
        Send_mocker.forward( &msg );
    }

    mockpp::ChainableMockMethod< void, ASN1T_MsgsSimToClient* > Send_mocker;
    
};


#endif // __MockPublisher_ABC_h_
