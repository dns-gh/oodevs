// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __IndicatorPrimitiveParameter_h_
#define __IndicatorPrimitiveParameter_h_

namespace xml
{
    class xistream;
    class xostream;
}

class IndicatorType;
class IndicatorElement_ABC;

// =============================================================================
/** @class  IndicatorPrimitiveParameter
    @brief  IndicatorPrimitiveParameter
*/
// Created: SBO 2009-04-06
// =============================================================================
class IndicatorPrimitiveParameter
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit IndicatorPrimitiveParameter( xml::xistream& xis );
    virtual ~IndicatorPrimitiveParameter();
    //@}

    //! @name Operations
    //@{
    std::string GetAttribute() const;
    const IndicatorType& GetType() const;
    void Serialize( xml::xostream& xos, const IndicatorElement_ABC& element ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    IndicatorPrimitiveParameter( const IndicatorPrimitiveParameter& );            //!< Copy constructor
    IndicatorPrimitiveParameter& operator=( const IndicatorPrimitiveParameter& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const QString name_;
    const std::string attribute_;
    std::auto_ptr< IndicatorType > type_;
    //@}
};

#endif // __IndicatorPrimitiveParameter_h_
