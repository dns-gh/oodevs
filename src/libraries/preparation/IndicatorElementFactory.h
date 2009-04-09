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
    explicit IndicatorElementFactory( const IndicatorPrimitives& primitives );
    virtual ~IndicatorElementFactory();
    //@}

    //! @name Operations
    //@{
    virtual IndicatorElement_ABC* CreateNumber( double value ) const;
    virtual IndicatorElement_ABC* CreateVariable( const std::string& name ) const;
    virtual IndicatorElement_ABC* CreateExtractor( const std::string& name ) const;
    virtual IndicatorElement_ABC* CreateFunction( const std::string& name ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    IndicatorElementFactory( const IndicatorElementFactory& );            //!< Copy constructor
    IndicatorElementFactory& operator=( const IndicatorElementFactory& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    unsigned long NextId() const;
    //@}

private:
    //! @name Member data
    //@{
    const IndicatorPrimitives& primitives_;
    mutable unsigned long id_;
    //@}
};

#endif // __IndicatorElementFactory_h_
