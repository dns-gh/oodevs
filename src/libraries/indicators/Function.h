// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __Function_h_
#define __Function_h_

#include "Element_ABC.h"
#include <boost/shared_ptr.hpp>
#pragma warning( push, 0 )
#pragma warning( push, 0 )
#include <QtCore/qstring.h>
#pragma warning( pop )
#pragma warning( pop )

namespace indicators
{
// =============================================================================
/** @class  Function
    @brief  Function
*/
// Created: SBO 2009-03-17
// =============================================================================
class Function : public Element_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Function( const std::string& id, const QString& name, const std::string& category, boost::shared_ptr< DataType_ABC > type );
    virtual ~Function();
    //@}

    //! @name Accessors
    //@{
    virtual std::string GetValue() const;
    virtual const DataType_ABC& GetType() const;
    //@}

    //! @name Operations
    //@{
    void DeclareParameter( const std::string& attribute, boost::shared_ptr< Element_ABC > element );
    virtual void AddParameter( boost::shared_ptr< Element_ABC > element );
    virtual void Serialize( xml::xostream& xos, ElementDeclarator_ABC& declarator ) const;
    //@}

private:
    //! @name Helpers
    //@{
    void SerializeDeclarations( ElementDeclarator_ABC& declarator ) const;
    void SerializeType( xml::xostream& xos ) const;
    void SerializeParameters( xml::xostream& xos ) const;
    //@}

    //! @name Types
    //@{
    typedef std::vector< std::pair< std::string, boost::shared_ptr< Element_ABC > > > T_Parameters;
    //@}

private:
    //! @name Member data
    //@{
    const QString name_;
    const std::string category_;
    boost::shared_ptr< DataType_ABC > type_;
    T_Parameters parameters_;
    unsigned int parameterSet_;
    //@}
};

}

#endif // __Function_h_
