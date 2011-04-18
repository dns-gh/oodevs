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
#include <qhbox.h>

namespace frontend
{

// =============================================================================
/** @class  PluginConfig_ABC
    @brief  PluginConfig_ABC
*/
// Created: SBO 2009-12-09
// =============================================================================
class PluginConfig_ABC : private boost::noncopyable
                       , public QHBox
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit PluginConfig_ABC( QWidget* parent ) : QHBox( parent ) {}
    virtual ~PluginConfig_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual bool IsAvailable() const { return true; }
    virtual void Commit( const std::string& exercise, const std::string& session ) = 0;
    //@}
};

}

#endif // __PluginConfig_ABC_h_
