// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __IndicatorFunction_h_
#define __IndicatorFunction_h_

#include "IndicatorElement_ABC.h"
#include <boost/shared_ptr.hpp>

class IndicatorPrimitive;
class IndicatorPrimitiveParameter;
class IndicatorTypeResolver;

// =============================================================================
/** @class  IndicatorFunction
    @brief  IndicatorFunction
*/
// Created: SBO 2009-03-17
// =============================================================================
class IndicatorFunction : public IndicatorElement_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             IndicatorFunction( const std::string& id, const IndicatorPrimitive& primitive );
    virtual ~IndicatorFunction();
    //@}

    //! @name Accessors
    //@{
    virtual const IndicatorType& GetType() const;
    //@}

    //! @name Operations
    //@{
    virtual void AddParameter( boost::shared_ptr< IndicatorElement_ABC > element );
    virtual void Serialize( xml::xostream& xos ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    IndicatorFunction( const IndicatorFunction& );            //!< Copy constructor
    IndicatorFunction& operator=( const IndicatorFunction& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void SerializeType( xml::xostream& xos ) const;
    void SerializeParameters( xml::xostream& xos ) const;
    //@}

    //! @name Types
    //@{
    typedef std::vector< std::pair< const IndicatorPrimitiveParameter*, boost::shared_ptr< IndicatorElement_ABC > > > T_Parameters;
    //@}

private:
    //! @name Member data
    //@{
    const IndicatorPrimitive& primitive_;
    boost::shared_ptr< IndicatorTypeResolver > typeResolver_;
    std::auto_ptr< IndicatorType > type_;
    T_Parameters parameters_;
    //@}
};

#endif // __IndicatorFunction_h_
