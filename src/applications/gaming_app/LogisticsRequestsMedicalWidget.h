// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LogisticsRequestsMedicalWidget_h_
#define __LogisticsRequestsMedicalWidget_h_

#include "LogisticConsignsWidget.h"
#include "gaming/LogMedicalConsign.h"
#include <boost/noncopyable.hpp>

class LogMedicalConsigns;
class Publisher_ABC;

// =============================================================================
/** @class  LogisticsRequestsMedicalWidget
    @brief  LogisticsRequestsMedicalWidget
*/
// Created: MMC 2013-09-16
// =============================================================================
class LogisticsRequestsMedicalWidget : public LogisticConsignsWidget< LogMedicalConsigns, LogMedicalConsign >
{
public:
    //! @name Constructors/Destructor
    //@{
             LogisticsRequestsMedicalWidget( QWidget* parent, kernel::Controllers& controllers,
                                             gui::DisplayExtractor& extractor, const kernel::Profile_ABC& profile, Publisher_ABC& publisher );
    virtual ~LogisticsRequestsMedicalWidget();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void OnRequestSelected( const LogisticsConsign_ABC& consign );
    QString GetInjury( const LogMedicalConsign& consign );
    //@}
};

#endif // __LogisticsRequestsMedicalWidget_h_
