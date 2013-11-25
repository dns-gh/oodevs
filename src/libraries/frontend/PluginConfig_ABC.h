// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __PluginConfig_ABC_h_
#define __PluginConfig_ABC_h_

#include <boost/noncopyable.hpp>
#include "clients_gui/WidgetLanguageObserver_ABC.h"

namespace frontend
{

// =============================================================================
/** @class  PluginConfig_ABC
    @brief  PluginConfig_ABC
*/
// Created: SBO 2009-12-09
// =============================================================================
class PluginConfig_ABC : public gui::WidgetLanguageObserver_ABC< QWidget >
                       , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit PluginConfig_ABC( QWidget* parent ) : gui::WidgetLanguageObserver_ABC< QWidget >( parent ) {}
    virtual ~PluginConfig_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual QString GetName() const = 0;
    virtual bool IsAvailable() const { return true; }
    virtual bool IsVisible() const { return true; }
    virtual void Commit( const tools::Path& exercise, const tools::Path& session ) = 0;
    //@}
};

}

#endif // __PluginConfig_ABC_h_
