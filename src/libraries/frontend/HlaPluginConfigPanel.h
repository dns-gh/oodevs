// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __HlaPluginConfigPanel_h_
#define __HlaPluginConfigPanel_h_

#include "PluginConfig_ABC.h"

namespace tools
{
    class GeneralConfig;
}

class QLineEdit;
class QGroupBox;
class QSpinBox;

namespace frontend
{
// =============================================================================
/** @class  HlaPluginConfigPanel
    @brief  HlaPluginConfigPanel
*/
// Created: SBO 2008-03-05
// =============================================================================
class HlaPluginConfigPanel : public PluginConfig_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             HlaPluginConfigPanel( QWidget* parent, const tools::GeneralConfig& config );
    virtual ~HlaPluginConfigPanel();
    //@}

    //! @name Operations
    //@{
    virtual void Commit( const std::string& exercise, const std::string& session );
    //@}

private:
    //! @name Member data
    //@{
    const tools::GeneralConfig& config_;
    const std::string library_;
    QGroupBox* box_;
    QLineEdit* federation_;
    QLineEdit* name_;
    QLineEdit* host_;
    QSpinBox*  port_;
    //@}
};
}

#endif // __HlaPluginConfigPanel_h_
