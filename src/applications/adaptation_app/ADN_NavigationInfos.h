// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __ADN_NavigationInfos_h_
#define __ADN_NavigationInfos_h_

#include "ADN_Enums.h"

namespace ADN_NavigationInfos
{
    struct UsedBy
    {
        UsedBy() : usingName_( "" ), targetTab_( eNbrWorkspaceElements ), subTargetTab_( -1 ) {}

        QString                 usingName_;
        QStringList             usersList_;
        E_WorkspaceElements     targetTab_;
        int                     subTargetTab_;
    };

    struct GoTo
    {
        GoTo() : targetName_( "" ), targetTab_( eNbrWorkspaceElements ), subTargetTab_( -1 ), sourceColumn_( -1 ) {}

        QString                 targetName_;
        E_WorkspaceElements     targetTab_;
        int                     subTargetTab_;
        int                     sourceColumn_;
    };
};

#endif // __ADN_NavigationInfos_h_
