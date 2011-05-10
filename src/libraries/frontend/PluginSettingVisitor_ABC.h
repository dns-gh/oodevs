// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __PluginSettingVisitor_ABC_h_
#define __PluginSettingVisitor_ABC_h_

#include <boost/noncopyable.hpp>

namespace frontend
{

// =============================================================================
/** @class  PluginSettingVisitor_ABC
    @brief  Plugin setting visitor interface
*/
// Created: SBO 2011-05-09
// =============================================================================
class PluginSettingVisitor_ABC : private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
             PluginSettingVisitor_ABC() {}
    virtual ~PluginSettingVisitor_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Visit( const std::string& attribute, const std::string& value ) = 0;
    virtual void Visit( const std::string& attribute, int value ) = 0;
    virtual void Visit( const std::string& attribute, bool value ) = 0;
    //@}
};

}

#endif // __PluginSettingVisitor_ABC_h_
