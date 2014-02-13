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

#include "clients_gui/WidgetLanguageObserver_ABC.h"
#include <boost/noncopyable.hpp>
#include <tools/ElementObserver_ABC.h>

namespace tools
{
    class GeneralConfig;
    class Loader_ABC;

namespace zip
{
    class OutputArchive;
}
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
class ExportWidget : public gui::WidgetLanguageObserver_ABC< QWidget >
                   , public tools::Observer_ABC
                   , public tools::ElementObserver_ABC< frontend::Exercise_ABC >
                   , private boost::noncopyable
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ExportWidget( QWidget* parent, const tools::GeneralConfig& config, const tools::Loader_ABC& fileLoader, kernel::Controllers& controllers );
    virtual ~ExportWidget();
    //@}

    //! @name Operations
    //@{
    void Update( QListWidgetItem* item = 0 );
    void ExportPackage();
    //@}

signals:
    //! @name Signals
    //@{
    void ButtonChanged( bool enable, const QString& text );
    //@}

public slots:
    //! @name Slots
    //@{
    void OnButtonChanged();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnSelectionChanged( const QModelIndex& next, const QModelIndex& previous );
    void OnSelectionChanged( QListWidgetItem* item );
    void OnModelNameChanged( const QString& text );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyCreated( const frontend::Exercise_ABC& exercise );
    virtual void NotifyDeleted( const frontend::Exercise_ABC& exercise );

    bool IsButtonEnabled();
    virtual void OnLanguageChanged();
    QString GetCurrentSelection() const;
    QTextEdit* GetCurrentDescription() const;
    QString GetCurrentPackage() const;
    bool BrowseClicked();
    void ExportPackage( tools::zip::OutputArchive& archive );
    void ExportExercise( tools::zip::OutputArchive& archive );
    void ExportTerrain( tools::zip::OutputArchive& archive );
    void ExportModels( tools::zip::OutputArchive& archive );
    void WriteContent( tools::zip::OutputArchive& archive ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::pair< tools::Path, tools::Path > T_Package; // <path, filename>
    enum E_Tabs{ eTabs_Exercise = 0, eTabs_Terrain = 1, eTabs_Models = 2 };
    //@}

private:
    //! @name Member data
    //@{
    const tools::GeneralConfig& config_;
    const tools::Loader_ABC&    fileLoader_;
    kernel::Controllers&        controllers_;
    QTabWidget*                 tabs_;
    // Common
    QProgressBar*               progress_;
    T_Package                   package_;
    // Exercises
    ExerciseListView*           exerciseList_;
    QTextEdit*                  exerciseDescription_;
    QTreeView*                  exerciseContent_;
    QStandardItemModel          exerciseContentModel_;
    // Terrains
    QTextEdit*                  terrainDescription_;
    QListWidget*                terrainList_;
    // Referential
    QTextEdit*                  modelDescription_;
    QLineEdit*                  modelName_;
    QCheckBox*                  decisionalCheckBox_;
    QListWidget*                physicalList_;
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
