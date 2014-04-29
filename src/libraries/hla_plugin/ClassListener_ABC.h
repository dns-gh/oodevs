// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_ClassListener_ABC_h
#define plugins_hla_ClassListener_ABC_h

#include <string>
#include <vector>

namespace hla
{
class AttributeIdentifier;
}

namespace plugins
{
namespace hla
{

class HlaClass_ABC;
class HlaObject_ABC;
typedef std::vector< ::hla::AttributeIdentifier > T_AttributeIdentifiers;

// =============================================================================
/** @class  ClassListener_ABC
    @brief  ClassListener_ABC
*/
// Created: AHC 2012-02-27
// =============================================================================
class ClassListener_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    virtual ~ClassListener_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void RemoteCreated( const std::string& identifier, HlaClass_ABC& hlaClass, HlaObject_ABC& object ) = 0;
    virtual void RemoteDestroyed( const std::string& identifier ) = 0;
    virtual void LocalCreated( const std::string& identifier, HlaClass_ABC& hlaClass, HlaObject_ABC& object ) = 0;
    virtual void LocalDestroyed( const std::string& identifier ) = 0;
    virtual void Divested( const std::string& identifier, const T_AttributeIdentifiers& attributes ) = 0;
    virtual void Acquired( const std::string& identifier, const T_AttributeIdentifiers& attributes ) = 0;
    //@}
};

}
}
#endif // plugins_hla_ClassListener_ABC_h
