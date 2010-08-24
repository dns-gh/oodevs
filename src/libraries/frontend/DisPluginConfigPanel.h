// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DisPluginConfigPanel_h_
#define __DisPluginConfigPanel_h_

#include "PluginConfig_ABC.h"

namespace tools
{
    class GeneralConfig;
}

class QLineEdit;
class QSpinBox;
class QCheckBox;
class QGroupBox;

namespace frontend
{

// =============================================================================
/** @class  DisPluginConfigPanel
    @brief  DisPluginConfigPanel
*/
// Created: SBO 2008-03-05
// =============================================================================
class DisPluginConfigPanel : public PluginConfig_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             DisPluginConfigPanel( QWidget* parent, const tools::GeneralConfig& config );
    virtual ~DisPluginConfigPanel();
    //@}

    //! @name Operations
    //@{
    virtual void Commit( const std::string& exercise, const std::string& session );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    DisPluginConfigPanel( const DisPluginConfigPanel& );            //!< Copy constructor
    DisPluginConfigPanel& operator=( const DisPluginConfigPanel& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const tools::GeneralConfig& config_;
    QGroupBox* box_;
    QCheckBox* tic_;
    QLineEdit* server_;
    QSpinBox*  port_;
    QSpinBox*  site_;
    QSpinBox*  application_;
    QSpinBox*  exercise_;
    //@}
};

}

#endif // __DisPluginConfigPanel_h_
