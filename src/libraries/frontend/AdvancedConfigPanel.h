// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __AdvancedConfigPanel_h_
#define __AdvancedConfigPanel_h_

#include "PluginConfig_ABC.h"

namespace tools
{
    class GeneralConfig;
}

class QCheckBox;
class QSpinBox;

namespace frontend
{
// =============================================================================
/** @class  AdvancedConfigPanel
    @brief  AdvancedConfigPanel
*/
// Created: JSR 2010-07-15
// =============================================================================
class AdvancedConfigPanel : public PluginConfig_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             AdvancedConfigPanel( QWidget* parent, const tools::GeneralConfig& config );
    virtual ~AdvancedConfigPanel();
    //@}

    //! @name Operations
    //@{
    virtual QString GetName() const;
    virtual void Commit( const std::string& exercise, const std::string& session );
    //@}

private:
    //! @name Member data
    //@{
    const tools::GeneralConfig& config_;
    QSpinBox* stepSpin_;
    QSpinBox* factorSpin_;
    QSpinBox* endtickSpin_;
    QCheckBox* pausedCheckBox_;
    QSpinBox* pathThreads_;
    QSpinBox* fragmentsFrequency_;
    //@}
};
}

#endif // __AdvancedConfigPanel_h_
