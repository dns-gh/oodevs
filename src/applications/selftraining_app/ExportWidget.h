// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ExportWidget_h_
#define __ExportWidget_h_

#include <boost/noncopyable.hpp>
#include "clients_gui/LanguageChangeObserver_ABC.h"
#include "tools/ElementObserver_ABC.h"

class ScenarioEditPage;

namespace tools
{
    class GeneralConfig;
    class Loader_ABC;
}

namespace zip
{
    class ozipfile;
}

namespace frontend
{
    class Exercise_ABC;
}

namespace kernel
{
    class Controllers;
}

class ExerciseListView;

// =============================================================================
/** @class  ExportWidget
    @brief  ExportWidget
*/
// Created: JSR 2010-07-15
// =============================================================================
class ExportWidget : public gui::LanguageChangeObserver_ABC< Q3GroupBox >
                   , public tools::Observer_ABC
                   , public tools::ElementObserver_ABC< frontend::Exercise_ABC >
                   , private boost::noncopyable
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ExportWidget( ScenarioEditPage& page, QWidget* parent, const tools::GeneralConfig& config, const tools::Loader_ABC& fileLoader, kernel::Controllers& controllers );
    virtual ~ExportWidget();
    //@}

    //! @name Operations
    //@{
    void Update( Q3ListBoxItem* item = 0 );
    void ExportPackage();
    bool EnableEditButton();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnSelectionChanged( const QModelIndex& next, const QModelIndex& previous );
    void OnSelectionChanged( Q3ListBoxItem* item );
    void OnModelNameChanged( const QString& text );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyCreated( const frontend::Exercise_ABC& exercise );
    virtual void NotifyDeleted( const frontend::Exercise_ABC& exercise );

    virtual void OnLanguageChanged();
    QString GetCurrentSelection() const;
    QTextEdit* GetCurrentDescription() const;
    QString GetCurrentPackage() const;
    bool BrowseClicked();
    void InternalExportPackage( zip::ozipfile& archive );
    void WriteContent( zip::ozipfile& archive ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::pair< std::string, std::string > T_Package; // <path, filename>
    enum E_Tabs{ eTabs_Exercise = 0, eTabs_Terrain = 1, eTabs_Models = 2 };
    //@}

private:
    //! @name Member data
    //@{
    const tools::GeneralConfig& config_;
    const tools::Loader_ABC&    fileLoader_;
    kernel::Controllers&        controllers_;
    ScenarioEditPage&           page_;
    QTabWidget*                 tabs_;
    // Common
    Q3ProgressBar*              progress_;
    T_Package                   package_;
    // Exercises
    ExerciseListView*           exerciseList_;
    QTextEdit*                  exerciseDescription_;
    Q3ListView*                 exerciseContent_;
    // Terrains
    QTextEdit*                  terrainDescription_;
    Q3ListBox*                  terrainList_;
    // Referential
    QTextEdit*                  modelDescription_;
    QLineEdit*                  modelName_;
    QCheckBox*                  decisionalCheckBox_;
    Q3ListBox*                  physicalList_;
    // Labels
    QLabel*                     exerciseDescriptionLabel_;
    QLabel*                     exerciseLabel_;
    QLabel*                     packageContentLabel_;
    QLabel*                     terrainDescriptionLabel_;
    QLabel*                     terrainLabel_;
    QLabel*                     modelsNameLabel_;
    QLabel*                     modelsDescriptionLabel_;
    QLabel*                     modelsDecisionalLabel_;
    QLabel*                     modelsPhysicalLabel_;
    //@}
};

#endif // __ExportWidget_h_
