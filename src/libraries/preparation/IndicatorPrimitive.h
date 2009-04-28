// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __IndicatorPrimitive_h_
#define __IndicatorPrimitive_h_

namespace xml
{
    class xistream;
    class xostream;
}

class IndicatorElement_ABC;
class IndicatorPrimitiveParameter;
class IndicatorType;

// =============================================================================
/** @class  IndicatorPrimitive
    @brief  IndicatorPrimitive
    // $$$$ SBO 2009-04-15: TODO: reverse dependency between Primitive and Instance (Element)
*/
// Created: SBO 2009-04-06
// =============================================================================
class IndicatorPrimitive
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit IndicatorPrimitive( xml::xistream& xis );
    virtual ~IndicatorPrimitive();
    //@}

    //! @name Accessors
    //@{
    QString GetName() const;
    std::string GetCategory() const;
    QString GetPrototype() const;
    QString GetComment() const;
    const IndicatorType& GetType() const;
    const IndicatorPrimitiveParameter* FindParameter( unsigned int index ) const;
    unsigned int ParameterCount() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    IndicatorPrimitive( const IndicatorPrimitive& );            //!< Copy constructor
    IndicatorPrimitive& operator=( const IndicatorPrimitive& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadParameter( xml::xistream& xis );
    QString BuildParameterList() const;
    //@}

    //! @name Types
    //@{
    typedef std::vector< IndicatorPrimitiveParameter* > T_Parameters;
    //@}

private:
    //! @name Member data
    //@{
    const QString name_;
    const std::string category_;
    const QString comment_;
    std::auto_ptr< IndicatorType > type_;
    T_Parameters parameters_;
    //@}
};

#endif // __IndicatorPrimitive_h_
