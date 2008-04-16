// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __InstallPackagePanel_h_
#define __InstallPackagePanel_h_

#include "Panel_ABC.h"

namespace zip
{
    class izipfile;
}

namespace frontend
{
    class Config;
}

class QLineEdit;
class QListBox;
class QProgressBar;
class QTextEdit;
class InfoBubble;

// =============================================================================
/** @class  InstallPackagePanel
    @brief  InstallPackagePanel
*/
// Created: SBO 2008-03-14
// =============================================================================
class InstallPackagePanel : public Panel_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             InstallPackagePanel( QWidgetStack* widget, QAction& action, const frontend::Config& config, ActionsContext& context );
    virtual ~InstallPackagePanel();
    //@}

private slots:
    //! @name Operations
    //@{
    void BrowseClicked();
    void InstallPackage();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    InstallPackagePanel( const InstallPackagePanel& );            //!< Copy constructor
    InstallPackagePanel& operator=( const InstallPackagePanel& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void Update();
    bool ReadContentFile();
    std::string GetDestinationDirectory() const;
    //@}

private:
    //! @name Member data
    //@{
    const frontend::Config& config_;
    QLineEdit* package_;
    QLineEdit* name_;
    QTextEdit* description_;
    QListBox* content_;
    QProgressBar* progress_;
    QPushButton* okay_;
    InfoBubble* bubble_;
    //@}
};

#endif // __InstallPackagePanel_h_
