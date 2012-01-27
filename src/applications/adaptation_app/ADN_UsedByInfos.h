// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __ADN_UsedByInfos_h_
#define __ADN_UsedByInfos_h_

#include <boost/noncopyable.hpp>
#include "ADN_Enums.h"

// =============================================================================
/** @class  ADN_UsedByInfos
    @brief  ADN_UsedByInfos
*/
// Created: ABR 2012-01-19
// =============================================================================
struct ADN_UsedByInfos
{
    QString                 usingName_;
    QStringList             usersList_;
    E_WorkspaceElements     targetTab_;
    int                     subTargetTab_;
};

#endif // __ADN_UsedByInfos_h_
