// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __DebugConfigPanel_h_
#define __DebugConfigPanel_h_

#include "clients_gui/LanguageChangeObserver_ABC.h"
#include <boost/noncopyable.hpp>

// =============================================================================
/** @class  DebugConfigPanel
    @brief  DebugConfigPanel
*/
// Created: NPT 2013-01-03
// =============================================================================
class DebugConfigPanel : public gui::LanguageChangeObserver_ABC< QWidget >
                       , private boost::noncopyable
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             DebugConfigPanel();
    virtual ~DebugConfigPanel();
    //@}

    //! @name Operations
    //@{
    virtual void OnLanguageChanged();
    //@}

signals:
    //! @name Signals
    //@{
    void SwordVersionSelected( bool isLegacy );
    void IntegrationPathSelected( const QString& integrationPath );
    //@}

private:
    //! @name Member data
    //@{
    QStringList* pathList_;
    //legacy
    QLabel* legacyLabel_;
    QCheckBox* legacyCheckBox_;
    QGroupBox* legacyBox_;

    //Integration Layer Configuration
    QLabel* integrationLabel_;
    QComboBox* integrationComboBox_;
    QPushButton* integrationButton_;
    QGroupBox* integrationBox_;
    //@}

private slots:
    //! @name Operations
    //@{
    void SwordVersionChecked( bool state );
    void OnChangeIntegrationDirectory();
    void OnEditIntegrationDirectory( const QString& );
    //@}
};

#endif // __DebugConfigPanel_h_
