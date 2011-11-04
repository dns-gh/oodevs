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

// =============================================================================
/** @class  ExportWidget
    @brief  ExportWidget
*/
// Created: JSR 2010-07-15
// =============================================================================
class ExportWidget : public Q3GroupBox
                   , private boost::noncopyable
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ExportWidget( ScenarioEditPage& page, QWidget* parent, const tools::GeneralConfig& config, const tools::Loader_ABC& fileLoader );
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
    void OnSelectionChanged( Q3ListBoxItem* item );
    //@}

private:
    //! @name Helpers
    //@{
    Q3ListBoxItem* GetCurrentSelection() const;
    QTextEdit* GetCurrentDescription() const;
    bool BrowseClicked();
    void InternalExportPackage( zip::ozipfile& archive );
    void WriteContent( zip::ozipfile& archive ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::pair< std::string, std::string > T_Package; // <path, filename>
    enum E_Tabs{ exercise = 0, terrain = 1, models = 2 };
    //@}

private:
    //! @name Member data
    //@{
    const tools::GeneralConfig& config_;
    const tools::Loader_ABC&    fileLoader_;
    ScenarioEditPage&           page_;
    QTabWidget*                 tabs_;
    // Common
    Q3ProgressBar*              progress_;
    T_Package                   package_;
    // Exercises
    Q3ListBox*                  exerciseList_;
    QTextEdit*                  exerciseDescription_;
    Q3ListView*                 exerciseContent_;
    // Terrains
    QTextEdit*                  terrainDescription_;
    Q3ListBox*                  terrainList_;
    // Referential
    QTextEdit*                  modelDescription_;
    QCheckBox*                  decisionalCheckBox_;
    Q3ListBox*                  physicalList_;
    //@}
};

#endif // __ExportWidget_h_
