// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __IndicatorElementFactory_h_
#define __IndicatorElementFactory_h_

#include "IndicatorElementFactory_ABC.h"

class IndicatorPrimitives;
class IndicatorVariables;

// =============================================================================
/** @class  IndicatorElementFactory
    @brief  IndicatorElementFactory
*/
// Created: SBO 2009-03-17
// =============================================================================
class IndicatorElementFactory : public IndicatorElementFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             IndicatorElementFactory( const IndicatorPrimitives& primitives, const IndicatorVariables& variables );
    virtual ~IndicatorElementFactory();
    //@}

    //! @name Operations
    //@{
    virtual boost::shared_ptr< IndicatorElement_ABC > CreateNumber( double value ) const;
    virtual boost::shared_ptr< IndicatorElement_ABC > CreateString( const std::string& value ) const;
    virtual boost::shared_ptr< IndicatorElement_ABC > CreateVariable( const std::string& name ) const;
    virtual boost::shared_ptr< IndicatorElement_ABC > CreateFunction( const std::string& name ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    IndicatorElementFactory( const IndicatorElementFactory& );            //!< Copy constructor
    IndicatorElementFactory& operator=( const IndicatorElementFactory& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    std::string NextId() const;
    //@}

private:
    //! @name Member data
    //@{
    const IndicatorPrimitives& primitives_;
    const IndicatorVariables& variables_;
    mutable unsigned long id_;
    //@}
};

#endif // __IndicatorElementFactory_h_
