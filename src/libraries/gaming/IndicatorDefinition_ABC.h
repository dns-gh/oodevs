// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __IndicatorDefinition_ABC_h_
#define __IndicatorDefinition_ABC_h_

#include <boost/noncopyable.hpp>

// =============================================================================
/** @class  IndicatorDefinition_ABC
    @brief  IndicatorDefinition_ABC
*/
// Created: SBO 2009-04-29
// =============================================================================
class IndicatorDefinition_ABC : private boost::noncopyable
{
public:
    //! @name Types
    //@{
    typedef std::map< std::string, std::string > T_Parameters;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             IndicatorDefinition_ABC() {}
    virtual ~IndicatorDefinition_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual QString GetName() const = 0;
    virtual QString GetDisplayName() const = 0;
    virtual std::string Commit( const T_Parameters& parameters ) const = 0;
    //@}
};

#endif // __IndicatorDefinition_ABC_h_
