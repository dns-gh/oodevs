// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __OrderParameter_h_
#define __OrderParameter_h_

#include "OrderParameterValue.h"
#include <set>
#include <map>

namespace xml
{
    class xistream;
}

namespace kernel
{

class OrderParameterValueVisitor_ABC
{
public:
             OrderParameterValueVisitor_ABC() {}
    virtual ~OrderParameterValueVisitor_ABC() {}

    virtual void Visit( const OrderParameterValue& value ) = 0;
};

class ChoicesVisitor_ABC
{
public:
             ChoicesVisitor_ABC() {}
    virtual ~ChoicesVisitor_ABC() {}

    virtual void Visit( const std::string& type ) = 0;
};

// =============================================================================
/** @class  OrderParameter
    @brief  OrderParameter
*/
// Created: SBO 2007-04-23
// =============================================================================
class OrderParameter
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit OrderParameter( xml::xistream& xis );
             OrderParameter( const std::string& name, const std::string& type, bool optional, unsigned int min = 1, unsigned int max = 1 );
    virtual ~OrderParameter();
    //@}

    //! @name Operations
    //@{
    const std::string& GetName() const;
    const std::string& GetType() const;
    const std::string& GetKeyName() const;
    void SetName( const std::string& name );
    void SetOptional( bool optional );
    void SetMinMaxOccurs( unsigned int min, unsigned int max );
    void SetKeyName( const std::string& name );
    bool IsOptional() const;
    bool HasGenObject( const std::string& type ) const; // $$$$ ABR 2013-01-08: Rename this method, not only about gen objects, that can be about object knowledge too
    unsigned int MinOccurs() const;
    unsigned int MaxOccurs() const;
    double MinValue() const;
    double MaxValue() const;
    bool IsList() const;
    const OrderParameterValue* FindValue( unsigned int id ) const;
    const OrderParameterValue& GetValue( unsigned int id ) const;
    const std::string& GetChoice( unsigned int id ) const;
    void AddValue( int id, const std::string& name );
    virtual void Accept( OrderParameterValueVisitor_ABC& visitor ) const;
    virtual void Accept( ChoicesVisitor_ABC& visitor ) const;
    void AddChoice( const std::string& choice );
    std::string CompatibleType( const std::string& type ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< unsigned int, OrderParameterValue >   T_OrderParameterValues;
    typedef T_OrderParameterValues::const_iterator        CIT_OrderParameterValues;
    typedef std::set< std::string >                         T_Aliases;
    typedef T_Aliases::const_iterator                     CIT_Aliases;
    //@}

    //! @name Helpers
    //@{
    void ReadValue( xml::xistream& xis );
    void ReadChoice( xml::xistream& xis, T_Aliases& data, bool lowerCase );
    //@}

private:
    //! @name Member data
    //@{
    std::string name_;
    std::string type_;
    std::string keyName_;
    bool optional_;
    unsigned int minOccurs_;
    unsigned int maxOccurs_;
    double minValue_;
    double maxValue_;
    T_OrderParameterValues values_;
    T_Aliases aliases_;
    T_Aliases genObjects_;
    //@}
};

}

#endif // __OrderParameter_h_
