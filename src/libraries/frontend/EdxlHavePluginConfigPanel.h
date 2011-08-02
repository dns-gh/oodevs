// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __EdxlHavePluginConfigPanel_h_
#define __EdxlHavePluginConfigPanel_h_

#include "PluginConfig_ABC.h"

namespace tools
{
    class GeneralConfig;
}

class QLineEdit;
class Q3GroupBox;
class Q3TimeEdit;
class QCheckBox;

namespace frontend
{
// =============================================================================
/** @class  EdxlHavePluginConfigPanel
    @brief  EdxlHavePluginConfigPanel
*/
// Created: SBO 2008-03-05
// =============================================================================
class EdxlHavePluginConfigPanel : public PluginConfig_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             EdxlHavePluginConfigPanel( QWidget* parent, const tools::GeneralConfig& config );
    virtual ~EdxlHavePluginConfigPanel();
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
    Q3GroupBox* box_;
    QLineEdit* host_;
    QCheckBox* ssl_;
    QCheckBox* log_;
    QLineEdit* initializeServiceURI_;
    QLineEdit* updateServiceURI_;
    Q3TimeEdit* frequency_;
    //@}
};
}

#endif // __EdxlHavePluginConfigPanel_h_
