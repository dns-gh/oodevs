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

#include <qgroupbox.h>

class QProgressBar;
class ScenarioEditPage;
namespace
{
    class TabWidget;
}

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
class ImportWidget : public QGroupBox
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
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ImportWidget( const ImportWidget& );            //!< Copy constructor
    ImportWidget& operator=( const ImportWidget& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void InstallPackage();
    bool ReadPackageContentFile();

    // LTO begin
    void ImportScenario();
    void AddModelChoice( QGroupBox* box );
    void AddOutput( QGroupBox* box );
    void AddInput( QGroupBox* box );
    void AddTerrainChoice( QGroupBox* box );
    // LTO end
    //@}

private slots:
    //! @name slots
    //@{
    void PackageBrowseClicked();
    // LTO begin
    void OnModelChanged( const QString & model );
    void OnTerrainChanged( const QString& terrain );
    void OnOutputName( const QString& scenario );
    void OnChangeScenario();
    // LTO end
    //@}

private:
    //! @name Member data
    //@{
    ScenarioEditPage& page_;
    const tools::GeneralConfig& config_;
    TabWidget* tabs_;
    QLineEdit* package_;
    QLineEdit* packageName_;
    QTextEdit* packageDescription_;
    QListBox* packageContent_;
    QProgressBar* packageProgress_;
    // LTO begin
    QButton* browse_;
    QString model_;
    QString inputScenario_;
    QString outputScenario_;
    QString terrain_;
    //@}
};

#endif // __ImportWidget_h_
