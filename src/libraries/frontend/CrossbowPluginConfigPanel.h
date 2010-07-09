// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __CrossbowPluginConfigPanel_h_
#define __CrossbowPluginConfigPanel_h_

#include "PluginConfig_ABC.h"

namespace tools
{
    class GeneralConfig;
}

class QLineEdit;
class QCheckBox;
class QGroupBox;

namespace frontend
{

// =============================================================================
/** @class  CrossbowPluginConfigPanel
    @brief  CrossbowPluginConfigPanel
*/
// Created: SBO 2008-03-05
// =============================================================================
class CrossbowPluginConfigPanel : public PluginConfig_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             CrossbowPluginConfigPanel( QWidget* parent, const tools::GeneralConfig& config );
    virtual ~CrossbowPluginConfigPanel();
    //@}

    //! @name Operations
    //@{
    virtual void Commit( const std::string& exercise, const std::string& session );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    CrossbowPluginConfigPanel( const CrossbowPluginConfigPanel& );            //!< Copy constructor
    CrossbowPluginConfigPanel& operator=( const CrossbowPluginConfigPanel& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const tools::GeneralConfig& config_;
    QGroupBox* box_;
    QLineEdit* geodatabase_;
    QLineEdit* population_;
    QLineEdit* shared_;
    //@}
};

}

#endif // __CrossbowPluginConfigPanel_h_
