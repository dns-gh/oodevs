// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_ObjectResolver_h
#define plugins_hla_ObjectResolver_h

#include "ObjectResolver_ABC.h"
#include <map>

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  ObjectResolver
    @brief  Object resolver
*/
// Created: SLI 2011-06-22
// =============================================================================
class ObjectResolver : public ObjectResolver_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ObjectResolver();
    virtual ~ObjectResolver();
    //@}

    //! @name Operations
    //@{
    virtual void Register( const std::string& identifier, const std::string& name );
    virtual void Unregister( const std::string& identifier );
    virtual std::string Resolve( const std::string& identifier ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, std::string > T_Objects;
    //@}

private:
    //! @name Member data
    //@{
    T_Objects objects_;
    //@}
};

}
}

#endif // plugins_hla_ObjectResolver_h
