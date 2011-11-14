// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ImportWidget_h_
#define __ImportWidget_h_

#include <boost/noncopyable.hpp>
#include "clients_gui/LanguageChangeObserver_ABC.h"

class ScenarioEditPage;

namespace tools
{
    class GeneralConfig;
}

// =============================================================================
/** @class  ImportWidget
    @brief  ImportWidget
*/
// Created: JSR 2010-07-13
// =============================================================================
class ImportWidget : public gui::LanguageChangeObserver_ABC< Q3GroupBox >
                   , private boost::noncopyable
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ImportWidget( ScenarioEditPage& page, QWidget* parent, const tools::GeneralConfig& config );
    virtual ~ImportWidget();
    //@}

    //! @name Operations
    //@{
    void InstallExercise();
    bool EnableEditButton();
    void SelectPackage( const QString& filename );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void OnLanguageChanged();
    bool ReadPackageContentFile();
    //@}

private slots:
    //! @name slots
    //@{
    void PackageBrowseClicked();
    //@}

private:
    //! @name Member data
    //@{
    ScenarioEditPage&           page_;
    const tools::GeneralConfig& config_;
    QTabWidget*                 tabs_;
    QLineEdit*                  package_;
    QLineEdit*                  packageName_;
    Q3TextEdit*                 packageDescription_;
    Q3ListBox*                  packageContent_;
    Q3ProgressBar*              packageProgress_;
    QLabel*                     packageLabel_;
    QLabel*                     contentLabel_;
    QPushButton*                browseButton_;
    //@}
};

#endif // __ImportWidget_h_
