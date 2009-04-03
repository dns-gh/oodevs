// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __IndicatorElementFactory_ABC_h_
#define __IndicatorElementFactory_ABC_h_

class IndicatorElement_ABC;

// =============================================================================
/** @class  IndicatorElementFactory_ABC
    @brief  IndicatorElementFactory_ABC
*/
// Created: SBO 2009-03-17
// =============================================================================
class IndicatorElementFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             IndicatorElementFactory_ABC() {}
    virtual ~IndicatorElementFactory_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual IndicatorElement_ABC* CreateNumber( double value ) const = 0;
    virtual IndicatorElement_ABC* CreateVariable( const std::string& name ) const = 0;
    virtual IndicatorElement_ABC* CreateExtractor( const std::string& name ) const = 0;
    virtual IndicatorElement_ABC* CreateFunction( const std::string& name ) const = 0;
    //@}
};

#endif // __IndicatorElementFactory_ABC_h_
