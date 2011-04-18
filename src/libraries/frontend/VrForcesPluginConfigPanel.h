// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __VrForcesPluginConfigPanel_h_
#define __VrForcesPluginConfigPanel_h_

#include "PluginConfig_ABC.h"

namespace tools
{
    class GeneralConfig;
}

class QLineEdit;
class QGroupBox;

namespace frontend
{
// =============================================================================
/** @class  VrForcesPluginConfigPanel
    @brief  VrForces plugin configuration panel
*/
// Created: SBO 2011-04-18
// =============================================================================
class VrForcesPluginConfigPanel : public PluginConfig_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             VrForcesPluginConfigPanel( QWidget* parent, const tools::GeneralConfig& config );
    virtual ~VrForcesPluginConfigPanel();
    //@}

    //! @name Operations
    //@{
    virtual bool IsAvailable() const;
    virtual void Commit( const std::string& exercise, const std::string& session );
    //@}

private:
    //! @name Member data
    //@{
    const tools::GeneralConfig& config_;
    const std::string library_;
    QGroupBox* box_;
    QLineEdit* fom_;
    //@}
};
}

#endif // __VrForcesPluginConfigPanel_h_
