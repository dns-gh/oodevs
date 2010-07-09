// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
// LTO
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ImportPage_h_
#define __ImportPage_h_

#include "ContentPage.h"

namespace tools
{
    class GeneralConfig;
}

namespace kernel
{
    class Controllers;
}

class ProgressPage;

// =============================================================================
/** @class  ImportPage
    @brief  ImportPage
*/
// Created: LDC 2010-06-21
// =============================================================================
class ImportPage : public ContentPage
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ImportPage( QWidgetStack* pages, Page_ABC& previous, const tools::GeneralConfig& config, kernel::Controllers& controllers );
    virtual ~ImportPage();
    //@}

private:
    void AddModelChoice( QGroupBox* box );
    void AddScenarioOutput( QGroupBox* box );
    void AddScenarioInput( QGroupBox* box );
    void AddTerrainChoice( QGroupBox* box );
    void LaunchImport();
    void LaunchPreparation();

private slots:
    void OnModelChanged( const QString & );
    void OnTerrainChanged( const QString& );
    void OnOutputName( const QString& );
    void OnChangeScenario();
    virtual void OnEdit();

private:
    const tools::GeneralConfig& config_;
    kernel::Controllers& controllers_;
    ProgressPage* progressPage_;
    QButton* browse_;
    QString model_;
    QString inputScenario_;
    QString outputScenario_;
    QString terrain_;

};

#endif // __ImportPage_h_
