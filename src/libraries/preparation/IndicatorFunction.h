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

class IndicatorPrimitive;
class IndicatorPrimitiveParameter;

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
             IndicatorFunction( unsigned long id, const IndicatorPrimitive& primitive );
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
    virtual void SerializeDeclaration( xml::xostream& xos ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    IndicatorFunction( const IndicatorFunction& );            //!< Copy constructor
    IndicatorFunction& operator=( const IndicatorFunction& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    typedef std::map< const IndicatorPrimitiveParameter*, boost::shared_ptr< IndicatorElement_ABC > > T_Parameters;
    //@}

private:
    //! @name Member data
    //@{
    const IndicatorPrimitive& primitive_;
    T_Parameters parameters_;
    //@}
};

#endif // __IndicatorFunction_h_
