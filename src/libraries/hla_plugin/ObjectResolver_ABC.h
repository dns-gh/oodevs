// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_ObjectResolver_ABC_h
#define plugins_hla_ObjectResolver_ABC_h

#include <boost/noncopyable.hpp>
#include <string>

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  ObjectResolver_ABC
    @brief  Object resolver definition
*/
// Created: SLI 2011-06-22
// =============================================================================
class ObjectResolver_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             ObjectResolver_ABC() {}
    virtual ~ObjectResolver_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Register( const std::string& identifier, const std::string& name ) = 0;
    virtual void Unregister( const std::string& identifier ) = 0;
    virtual std::string Resolve( const std::string& identifier ) const = 0;
    //@}
};

}
}

#endif // plugins_hla_ObjectResolver_ABC_h
