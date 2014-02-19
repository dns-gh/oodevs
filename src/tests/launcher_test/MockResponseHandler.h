// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef launcher_test_MockResponseHandler_h
#define launcher_test_MockResponseHandler_h

#include "frontend/ResponseHandler_ABC.h"

namespace launcher_test
{
// =============================================================================
/** @class  MockResponseHandler
    @brief  Mock response handler
*/
// Created: LGY 2011-05-20
// =============================================================================
MOCK_BASE_CLASS( MockResponseHandler, frontend::ResponseHandler_ABC )
{
    MOCK_METHOD( Handle, 1, void( const sword::ExerciseListResponse& ), HandleExerciseListResponse );
    MOCK_METHOD( Handle, 1, void( const sword::SessionStartResponse& ), HandleSessionStartResponse );
    MOCK_METHOD( Handle, 1, void( const sword::SessionStopResponse& ), HandleSessionStopResponse );
    MOCK_METHOD( Handle, 1, void( const sword::SessionListResponse& ), HandleSessionListResponse );
    MOCK_METHOD( Handle, 1, void( const sword::ProfileListResponse& ), HandleProfileListResponse );
    MOCK_METHOD( Handle, 1, void( const sword::SessionNotification& ), HandleSessionNotification );
    MOCK_METHOD( Handle, 1, void( const sword::SessionParameterChangeResponse& ), HandleSessionParameterChangeResponse );
    MOCK_METHOD( Handle, 1, void( const sword::SessionStatus& ), HandleSessionStatus );
    MOCK_METHOD( Handle, 1, void( const sword::CheckpointListResponse& ), HandleCheckpointListResponse );
    MOCK_METHOD( Handle, 1, void( const sword::CheckpointDeleteResponse& ), HandleCheckpointDeleteResponse );
};

}

#endif // launcher_test_MockResponseHandler_h
