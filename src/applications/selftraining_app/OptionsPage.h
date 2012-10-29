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

#include "LauncherClientPage.h"

class Application;
class Config;
class DataWidget;
class ImportWidget;
class ExportWidget;

namespace frontend
{
    class LauncherClient;
}

namespace tools
{
    class Loader_ABC;
}

namespace kernel
{
    class Controllers;
}

// =============================================================================
/** @class  OptionsPage
    @brief  OptionsPage
*/
// Created: SBO 2008-02-21
// =============================================================================
class OptionsPage : public LauncherClientPage
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             OptionsPage( Application& app, QWidget* parent, Q3WidgetStack* pages,
                          Page_ABC& previous, Config& config,
                          const tools::Loader_ABC& loader, kernel::Controllers& controllers,
                          frontend::LauncherClient& launcher );
    virtual ~OptionsPage();
    //@}

public:
    //! @name Operations
    //@{
    void ShowPackageInstallation( const QString& package );
    //@}

public slots:
    //! @name Operations
    //@{
    void UpdateButton();
    void OnButtonChanged( bool enabled, const QString& text );
    //@}

private slots:
    //! @name Operations
    //@{
    virtual void OnApply();
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
    void SetDataLayout();
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

    //! @name Types
    //@{
    typedef std::map< std::string, QString > T_Languages;
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
    //@}

    //! @name Settings tab
    //@{
    T_Languages  languages_;
    std::string  selectedLanguage_;
    std::string  selectedDataDir_;
    QLabel*      languageLabel_;
    QComboBox*   languageCombo_;
    QLabel*      dataLabel_;
    QPushButton* dataButton_;
    QLineEdit*   dataDirectory_;
    bool         hasChanged_;
    bool         languageHasChanged_;
    //@}

    //! @name Import tab
    //@{
    ImportWidget* import_;
    //@}

    //! @name Export tab
    //@{
    ExportWidget* export_;
    //@}

    //! @name Terrains/Models tab
    //@{
    DataWidget* data_;
    //@}
};

#endif // __OptionsPage_h_
