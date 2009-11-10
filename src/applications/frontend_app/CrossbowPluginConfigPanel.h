// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __CrossbowPluginConfigPanel_h_
#define __CrossbowPluginConfigPanel_h_

#include <qhbox.h>

namespace tools
{
    class GeneralConfig;
}

class QLineEdit;
class QCheckBox;
class QGroupBox;

// =============================================================================
/** @class  CrossbowPluginConfigPanel
    @brief  CrossbowPluginConfigPanel
*/
// Created: SBO 2008-03-05
// =============================================================================
class CrossbowPluginConfigPanel : public QHBox
{

public:
    //! @name Constructors/Destructor
    //@{
             CrossbowPluginConfigPanel( QWidget* parent, const tools::GeneralConfig& config );
    virtual ~CrossbowPluginConfigPanel();
    //@}

    //! @name Operations
    //@{
    bool IsChecked() const;
    void Commit( const std::string& exercise, const std::string& session );
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

#endif // __CrossbowPluginConfigPanel_h_
