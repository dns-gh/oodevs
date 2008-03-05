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

#include <qhbox.h>

namespace tools
{
    class GeneralConfig;
}

class QLineEdit;

// =============================================================================
/** @class  HlaPluginConfigPanel
    @brief  HlaPluginConfigPanel
*/
// Created: SBO 2008-03-05
// =============================================================================
class HlaPluginConfigPanel : public QHBox
{

public:
    //! @name Constructors/Destructor
    //@{
             HlaPluginConfigPanel( QWidget* parent, const tools::GeneralConfig& config );
    virtual ~HlaPluginConfigPanel();
    //@}

    //! @name Operations
    //@{
    void Commit( const std::string& exercise, const std::string& session );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    HlaPluginConfigPanel( const HlaPluginConfigPanel& );            //!< Copy constructor
    HlaPluginConfigPanel& operator=( const HlaPluginConfigPanel& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    const tools::GeneralConfig& config_;
    QGroupBox* box_;
    QLineEdit* federation_;
    QLineEdit* name_;
    //@}
};

#endif // __HlaPluginConfigPanel_h_
