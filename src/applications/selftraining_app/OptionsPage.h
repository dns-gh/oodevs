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
class DataWidget;
class ExerciseContainer;
class ExportWidget;
class ImportWidget;

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
class OptionsPage : public ContentPage
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             OptionsPage( Application& app, QWidget* parent, QStackedWidget* pages,
                  Page_ABC& previous, Config& config,
                  const tools::Loader_ABC& loader, kernel::Controllers& controllers,
                  ExerciseContainer& exercises );
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
    DataWidget* data_;
    //@}
};

#endif // __OptionsPage_h_
