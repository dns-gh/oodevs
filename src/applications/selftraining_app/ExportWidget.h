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
    void Update();
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
    void UpdateExercises();

    bool BrowseClicked();
    void WriteContent( zip::ozipfile& archive ) const;
    //@}

    //! @name Copy/Assignment
    //@{
    ExportWidget( const ExportWidget& );            //!< Copy constructor
    ExportWidget& operator=( const ExportWidget& ); //!< Assignment operator
    //@}

private:
    //! @name Types
    //@{
    typedef std::pair< std::string, std::string > T_Package; // <path, filename>
    //@}

private:
    //! @name Member data
    //@{
    const tools::GeneralConfig& config_;
    const tools::Loader_ABC&    fileLoader_;
    ScenarioEditPage& page_;
    Q3ListBox*  list_;
    T_Package package_;
    Q3TextEdit* description_;
    Q3ListView* content_;
    Q3ProgressBar* progress_;
    //@}
};

#endif // __ExportWidget_h_
