// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ScenarioPage_h_
#define __ScenarioPage_h_

#include "ContentPage.h"

class QListBox;
class QTextEdit;

namespace tools
{
    class GeneralConfig;
}

// =============================================================================
/** @class  ScenarioPage
    @brief  ScenarioPage
*/
// Created: SBO 2008-02-21
// =============================================================================
class ScenarioPage : public ContentPage
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ScenarioPage( QWidgetStack* pages, Page_ABC& previous, const tools::GeneralConfig& config );
    virtual ~ScenarioPage();
    //@}

private slots:
    //! @name Operations
    //@{
    void OnStart();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ScenarioPage( const ScenarioPage& );            //!< Copy constructor
    ScenarioPage& operator=( const ScenarioPage& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void Update();
    //@}

private:
    //! @name Member data
    //@{
    const tools::GeneralConfig& config_;
    QListBox* scenarios_;
    QListBox* teams_;
    QTextEdit* briefing_;
    //@}
};

#endif // __ScenarioPage_h_
