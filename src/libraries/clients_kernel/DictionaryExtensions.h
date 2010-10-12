// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __DictionaryExtensions_h_
#define __DictionaryExtensions_h_

#include "Extension_ABC.h"
#include "Serializable_ABC.h"

namespace xml
{
    class xistream;
    class xostream;
}

namespace kernel
{

// =============================================================================
/** @class  DictionaryExtensions
    @brief  DictionaryExtensions
*/
// Created: JSR 2010-10-04
// =============================================================================
class DictionaryExtensions : public Extension_ABC
                           , public Serializable_ABC
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
             DictionaryExtensions();
    explicit DictionaryExtensions( xml::xistream& xis );
    virtual ~DictionaryExtensions();
    //@}

    //! @name Operations
    //@{
    virtual void SerializeAttributes( xml::xostream& xos ) const;
    void SetEnabled( bool enabled );
    bool IsEnabled() const;
    void SetValue( const std::string& name, const std::string& value );
    const std::string& GetValue( const std::string& name ) const;
    const T_Extensions& GetExtensions() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    DictionaryExtensions( const DictionaryExtensions& );            //!< Copy constructor
    DictionaryExtensions& operator=( const DictionaryExtensions& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadExtension( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    bool enabled_;
    T_Extensions extensions_; 
    //@}
};

}

#endif // __DictionaryExtensions_h_
