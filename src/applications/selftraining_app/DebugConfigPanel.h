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

#include <boost/noncopyable.hpp>
// =============================================================================
/** @class  DebugConfigPanel
    @brief  DebugConfigPanel
*/
// Created: NPT 2013-01-03
// =============================================================================
class DebugConfigPanel : public QWidget
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
    //@}

signals:
    //! @name Signals
    //@{
    void SwordVersionSelected( bool isLegacy );
    void IntegrationPathSelected( const QString& integrationPath );
    //@}

private:
    //! @name Helpers
    //@{
    //@}

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
