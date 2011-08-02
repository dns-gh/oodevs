// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __CreateTerrainPage_h_
#define __CreateTerrainPage_h_

#include "ContentPage.h"

namespace kernel
{
    class Controllers;
}

class Config;
class ProgressPage;

// =============================================================================
/** @class  CreateTerrainPage
    @brief  CreateTerrainPage
*/
// Created: JSR 2010-06-11
// =============================================================================
class CreateTerrainPage : public ContentPage
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             CreateTerrainPage( Q3WidgetStack* pages, Page_ABC& previous, kernel::Controllers& controllers, const Config& config );
    virtual ~CreateTerrainPage();
    //@}

private slots:
    //! @name Operations
    //@{
    virtual void OnStart();
    void EditNameChanged( const QString& );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    CreateTerrainPage( const CreateTerrainPage& );            //!< Copy constructor
    CreateTerrainPage& operator=( const CreateTerrainPage& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void Update();
    //@}

private:
    //! @name Member data
    //@{
    const Config& config_;
    kernel::Controllers& controllers_;
    ProgressPage* progressPage_;
    QLineEdit* editName_;
    QLabel* label_;
    QStringList existingTerrains_;
    //@}
};

#endif // __CreateTerrainPage_h_
