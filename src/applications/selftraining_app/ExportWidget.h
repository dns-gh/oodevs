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

#include <qgroupbox.h>

class QListView;
class QProgressBar;
class ScenarioEditPage;

namespace tools
{
    class GeneralConfig;
}

// =============================================================================
/** @class  ExportWidget
    @brief  ExportWidget
*/
// Created: JSR 2010-07-15
// =============================================================================
class ExportWidget : public QGroupBox
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ExportWidget( ScenarioEditPage& page, QWidget* parent, const tools::GeneralConfig& config );
    virtual ~ExportWidget();
    //@}

    //! @name Operations
    //@{
    void ExportPackage();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ExportWidget( const ExportWidget& );            //!< Copy constructor
    ExportWidget& operator=( const ExportWidget& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
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
    ScenarioEditPage& page_;
    QListBox*  list_;
    T_Package package_;
    QTextEdit* description_;
    QListView* content_;
    QProgressBar* progress_;
    //@}
};

#endif // __ExportWidget_h_
