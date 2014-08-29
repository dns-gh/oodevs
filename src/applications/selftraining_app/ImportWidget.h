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
#include "clients_gui/WidgetLanguageObserver_ABC.h"

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
class ImportWidget : public gui::WidgetLanguageObserver_ABC< QWidget >
                   , private boost::noncopyable
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ImportWidget( QWidget* parent, const tools::GeneralConfig& config );
    virtual ~ImportWidget();
    //@}

    //! @name Operations
    //@{
    void InstallExercise();
    void SelectPackage( const tools::Path& filename );
    //@}

signals:
    //! @name Signals
    //@{
    void ButtonChanged( bool enable, const QString& text, bool upgrade );
    //@}

public slots:
    //! @name slots
    //@{
    void OnButtonChanged();
    //@}

private:
    //! @name Helpers
    //@{
    bool IsButtonEnabled() const;
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
    const tools::GeneralConfig& config_;
    QTabWidget*                 tabs_;
    QLineEdit*                  package_;
    QLineEdit*                  packageName_;
    QLineEdit*                  packageVersion_;
    QTextEdit*                  packageDescription_;
    QListWidget*                packageContent_;
    QProgressBar*               packageProgress_;
    QLabel*                     packageLabel_;
    QLabel*                     contentLabel_;
    QLabel*                     versionLabel_;
    QPushButton*                browseButton_;
    bool                        isValidVersion_;
    //@}
};

#endif // __ImportWidget_h_
