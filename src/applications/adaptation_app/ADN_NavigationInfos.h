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

#include <boost/noncopyable.hpp>
#include "ADN_Enums.h"

// =============================================================================
/** @class  ADN_NavigationInfos
    @brief  ADN_NavigationInfos
*/
// Created: ABR 2012-03-05
// =============================================================================
class ADN_NavigationInfos : private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
             ADN_NavigationInfos() {}
    virtual ~ADN_NavigationInfos() {}
    //@}

    //! @name Types
    //@{
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
    //@}
};

#endif // __ADN_NavigationInfos_h_
