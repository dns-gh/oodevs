// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __PositionsPluginConfigPanel_h_
#define __PositionsPluginConfigPanel_h_

#include <boost/noncopyable.hpp>
#include "PluginConfig_ABC.h"

class QGroupBox;
class QTimeEdit;

namespace tools
{
    class GeneralConfig;
}

namespace frontend
{

// =============================================================================
/** @class  PositionsPluginConfigPanel
    @brief  PositionsPluginConfigPanel
*/
// Created: ABR 2011-04-04
// =============================================================================
class PositionsPluginConfigPanel : public PluginConfig_ABC
                                 , private boost::noncopyable
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             PositionsPluginConfigPanel( QWidget* parent, const tools::GeneralConfig& config );
    virtual ~PositionsPluginConfigPanel();
    //@}

    //! @name Operations
    //@{
    virtual void Commit( const std::string& exercise, const std::string& session );
    //@}

private:
    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    const tools::GeneralConfig& config_;
    QGroupBox* positionsSaverGroup_;
    QTimeEdit* frequency_;
    //@}
};

} // namespace frontend

#endif // __PositionsPluginConfigPanel_h_
