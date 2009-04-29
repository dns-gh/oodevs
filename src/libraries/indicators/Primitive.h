// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __Primitive_h_
#define __Primitive_h_

namespace xml
{
    class xistream;
    class xostream;
}

namespace indicators
{
    class Element_ABC;
    class PrimitiveParameter;
    class ElementType;

// =============================================================================
/** @class  Primitive
    @brief  Primitive
    // $$$$ SBO 2009-04-15: TODO: reverse dependency between Primitive and Instance (Element)
*/
// Created: SBO 2009-04-06
// =============================================================================
class Primitive
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit Primitive( xml::xistream& xis );
    virtual ~Primitive();
    //@}

    //! @name Accessors
    //@{
    QString GetName() const;
    std::string GetCategory() const;
    QString GetPrototype() const;
    QString GetComment() const;
    const ElementType& GetType() const;
    const PrimitiveParameter* FindParameter( unsigned int index ) const;
    unsigned int ParameterCount() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Primitive( const Primitive& );            //!< Copy constructor
    Primitive& operator=( const Primitive& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadParameter( xml::xistream& xis );
    QString BuildParameterList() const;
    //@}

    //! @name Types
    //@{
    typedef std::vector< PrimitiveParameter* > T_Parameters;
    //@}

private:
    //! @name Member data
    //@{
    const QString name_;
    const std::string category_;
    const QString comment_;
    std::auto_ptr< ElementType > type_;
    T_Parameters parameters_;
    //@}
};

}

#endif // __Primitive_h_
