// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __IndicatorType_h_
#define __IndicatorType_h_

#include <boost/shared_ptr.hpp>

namespace xml
{
    class xistream;
}

class IndicatorTypeResolver;

// =============================================================================
/** @class  IndicatorType
    @brief  IndicatorType
*/
// Created: SBO 2009-04-06
// =============================================================================
class IndicatorType
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit IndicatorType( xml::xistream& xis );
    explicit IndicatorType( const std::string& type );
             IndicatorType( const IndicatorType& type, boost::shared_ptr< IndicatorTypeResolver > resolver );
    virtual ~IndicatorType();
    //@}

    //! @name Operations
    //@{
    std::string ToString() const;
    std::string Resolve() const;
    bool operator!=( const IndicatorType& rhs ) const;
    bool IsAbstract() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
//    IndicatorType( const IndicatorType& type ); //<! Copy constructor
    IndicatorType& operator=( const IndicatorType& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    std::string type_;
    boost::shared_ptr< IndicatorTypeResolver > resolver_;
    //@}
};

#endif // __IndicatorType_h_
