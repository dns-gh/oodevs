// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __FunctionParameter_h_
#define __FunctionParameter_h_

#include "Element_ABC.h"
#pragma warning( push, 0 )
#pragma warning( push, 0 )
#include <QtCore/qstring.h>
#pragma warning( pop )
#pragma warning( pop )

namespace indicators
{
// =============================================================================
/** @class  FunctionParameter
    @brief  FunctionParameter
*/
// Created: SBO 2009-05-11
// =============================================================================
class FunctionParameter : public Element_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             FunctionParameter( const QString& name, const std::string& attribute, boost::shared_ptr< DataType_ABC > type );
    virtual ~FunctionParameter();
    //@}

    //! @name Accessors
    //@{
    virtual std::string GetValue() const;
    virtual const DataType_ABC& GetType() const;
    //@}

    //! @name Operations
    //@{
    virtual void AddParameter( boost::shared_ptr< Element_ABC > element );
    virtual void Serialize( xml::xostream& xos, ElementDeclarator_ABC& declarator ) const;
    //@}

private:
    //! @name Member data
    //@{
    const QString name_;
    const std::string attribute_;
    boost::shared_ptr< DataType_ABC > type_;
    //@}
};

}

#endif // __FunctionParameter_h_
