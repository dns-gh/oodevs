// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef kernel_DictionaryExtensions_h_
#define kernel_DictionaryExtensions_h_

#include "Extension_ABC.h"
#include "Serializable_ABC.h"
#include <boost/noncopyable.hpp>
#include "tools/Observer_ABC.h"
#include "tools/ElementObserver_ABC.h"

namespace xml
{
    class xistream;
    class xostream;
}

namespace kernel
{
    class Controllers;
    class Entity_ABC;
    class ExtensionTypes;

// =============================================================================
/** @class  DictionaryExtensions
    @brief  DictionaryExtensions
*/
// Created: JSR 2010-10-04
// =============================================================================
class DictionaryExtensions : public Extension_ABC
                           , public Serializable_ABC
                           , public tools::Observer_ABC
                           , public tools::ElementObserver_ABC< kernel::Entity_ABC >
                           , private boost::noncopyable
{
public:
    //! @name Types
    //@{
    typedef std::map< std::string, std::string > T_Extensions;
    typedef T_Extensions::const_iterator       CIT_Extensions;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             DictionaryExtensions( Controllers& controllers, const std::string& extensionType, const ExtensionTypes& resolver );
             DictionaryExtensions( Controllers& controllers, const std::string& extensionType, xml::xistream& xis, const ExtensionTypes& resolver );
    virtual ~DictionaryExtensions();
    //@}

    //! @name Operations
    //@{
    virtual void SerializeAttributes( xml::xostream& xos ) const;
    virtual void NotifyDeleted( const kernel::Entity_ABC& element );
    void SetEnabled( bool enabled );
    bool IsEnabled() const;
    void SetValue( const std::string& name, const std::string& value );
    const std::string& GetValue( const std::string& name ) const;
    const T_Extensions& GetExtensions() const;
    const ExtensionTypes& GetExtensionTypes() const;
    //@}

protected:
    //! @name Helpers
    //@{
    void SetValueWithDictionnaryLink( const std::string& name, std::string value );
    std::string GetValueWithDictionnaryLink( const std::string& name ) const;
    //@}

private:
    //! @name Helpers
    //@{
    void ReadExtension( xml::xistream& xis );
    //@}

protected:
    //! @name Member data
    //@{
    Controllers& controllers_;
    bool enabled_;
    T_Extensions extensions_;
    const std::string extensionType_;
    const ExtensionTypes& resolver_;
    //@}
};

}

#endif // kernel_DictionaryExtensions_h_
