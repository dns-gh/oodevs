// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef InputPropagationAttribute_h
#define InputPropagationAttribute_h

#include "clients_kernel/ObjectExtensions.h"
#include "clients_kernel/Serializable_ABC.h"

namespace kernel
{
    class PropertiesDictionary;
}

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  InputPropagationAttribute
    @brief  Input propagation attribute
*/
// Created: LGY 2012-10-05
// =============================================================================
class InputPropagationAttribute : public kernel::InputPropagationAttribute_ABC
                                , public kernel::Serializable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             InputPropagationAttribute( kernel::PropertiesDictionary& dictionary, const QString& source );
             InputPropagationAttribute( xml::xistream& xis, kernel::PropertiesDictionary& dictionary );
    virtual ~InputPropagationAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void SerializeObjectAttributes( xml::xostream& xos ) const;
    //@}

private:
    //! @name Helpers
    //@{
    void CreateDictionary( kernel::PropertiesDictionary& dictionary );
    //@}

private:
    //! @name Member data
    //@{
    QString source_;
    //@}
};

#endif // InputPropagationAttribute_h
