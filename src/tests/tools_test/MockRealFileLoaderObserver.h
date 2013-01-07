// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __MockRealFileLoaderObserver_h_
#define __MockRealFileLoaderObserver_h_

#include "tools/RealFileLoaderObserver_ABC.h"

// =============================================================================
/** @class  MockEntityResolver
    @brief  MockEntityResolver
*/
// Created: NLD 2011-02-15
// =============================================================================
MOCK_BASE_CLASS( MockRealFileLoaderObserver, tools::RealFileLoaderObserver_ABC )
{
    MOCK_METHOD( NotifyInvalidXml, 2 );
    MOCK_METHOD( NotifyNoXmlSchemaSpecified, 1 );
    MOCK_METHOD( NotifyFileMigrated, 3 );
};

#endif // __MockRealFileLoaderObserver_h_
