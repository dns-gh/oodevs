// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __OptionsPage_h_
#define __OptionsPage_h_

#include "ContentPage.h"

class Application;
class Config;
class DebugConfigPanel;
class ExerciseContainer;
class ExportWidget;
class ImportWidget;
class ModelsWidget;
class TerrainsWidget;

namespace kernel
{
    class Controllers;
}

namespace frontend
{
    struct DebugConfig;
}

namespace tools
{
    class Loader_ABC;
}

// =============================================================================
/** @class  OptionsPage
    @brief  OptionsPage
*/
// Created: SBO 2008-02-21
// =============================================================================
class OptionsPage : public ContentPage
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             OptionsPage( Application& app, QWidget* parent, QStackedWidget* pages,
                  Page_ABC& previous, Config& config,
                  const tools::Loader_ABC& loader, kernel::Controllers& controllers,
                  ExerciseContainer& exercises, frontend::DebugConfig* debug );
    virtual ~OptionsPage();
    //@}

public:
    //! @name Operations
    //@{
    void ShowPackageInstallation( const tools::Path& package );
    //@}

public slots:
    //! @name Operations
    //@{
    void UpdateButton();
    void OnButtonChanged( bool enabled, const QString& text, bool upgrade );
    //@}

private slots:
    //! @name Operations
    //@{
    virtual void OnApply();
    virtual void OnUpgrade();
    void OnChangeLanguage( const QString& lang );
    void OnChangeDataDirectory();
    void OnEditDataDirectory( const QString& text );
    virtual void OnBack();
    //@}

private:
    //! @name Type
    //@{
    enum E_Tabs
    {
        eTabs_Settings,
        eTabs_Import,
        eTabs_Export,
        eTabs_Terrains,
        eTabs_Models,
    };
    void SetSettingsLayout();
    void SetImportLayout();
    void SetExportLayout();
    void SetDebugLayout( frontend::DebugConfig* debug );
    //@}

    //! @name Helpers
    //@{
    void Reconnect();
    virtual void ApplyAction();
    virtual void OnLanguageChanged();
    void Reset();
    void Commit();
    void CreateDataDirectory();
    //@}

private:
    //! @name Member data
    //@{
    Application&             app_;
    QWidget*                 parent_;
    QTabWidget*              tabs_;
    Config&                  config_;
    const tools::Loader_ABC& loader_;
    kernel::Controllers&     controllers_;
    ExerciseContainer&       exercises_;
    //@}

    //! @name Settings tab
    //@{
    std::string  selectedLanguage_;
    tools::Path  selectedDataDir_;
    QLabel*      languageLabel_;
    QComboBox*   languageCombo_;
    QLabel*      dataLabel_;
    QPushButton* dataButton_;
    QLineEdit*   dataDirectory_;
    bool         hasChanged_;
    bool         languageHasChanged_;
    //@}

    //! @name Other tabs
    //@{
    ImportWidget* import_;
    ExportWidget* export_;
    TerrainsWidget* terrains_;
    ModelsWidget* models_;
    DebugConfigPanel* debug_;
    //@}
};

#endif // __OptionsPage_h_
