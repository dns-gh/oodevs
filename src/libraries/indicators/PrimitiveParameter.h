// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __PrimitiveParameter_h_
#define __PrimitiveParameter_h_

namespace xml
{
    class xistream;
    class xostream;
}

namespace indicators
{
    class ElementType;
    class Element_ABC;

// =============================================================================
/** @class  PrimitiveParameter
    @brief  PrimitiveParameter
*/
// Created: SBO 2009-04-06
// =============================================================================
class PrimitiveParameter
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit PrimitiveParameter( xml::xistream& xis );
    virtual ~PrimitiveParameter();
    //@}

    //! @name Operations
    //@{
    std::string GetAttribute() const;
    QString GetName() const;
    const ElementType& GetType() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PrimitiveParameter( const PrimitiveParameter& );            //!< Copy constructor
    PrimitiveParameter& operator=( const PrimitiveParameter& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const QString name_;
    const std::string attribute_;
    std::auto_ptr< ElementType > type_;
    //@}
};

}

#endif // __PrimitiveParameter_h_
