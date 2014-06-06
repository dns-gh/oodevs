// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __IndicatorBuilder_h_
#define __IndicatorBuilder_h_

#include <string>

namespace indicators
{
    class DataTypeFactory;
    class ElementFactory;
    class Primitives;
    class Variables;
}

namespace tools
{
    class SessionConfig;
}

namespace xml
{
    class xostream;
}

namespace plugins
{
namespace score
{

// =============================================================================
/** @class  IndicatorBuilder
    @brief  Indicator builder
*/
// Created: SBO 2009-08-21
// =============================================================================
class IndicatorBuilder
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit IndicatorBuilder( const tools::SessionConfig& config );
    virtual ~IndicatorBuilder();
    //@}

    //! @name Operations
    //@{
    void Start();
    void AddVariable( const std::string& name, const std::string& type, const std::string& value );
    void Build( const std::string& name, const std::string& formula, xml::xostream& xos );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    IndicatorBuilder( const IndicatorBuilder& );            //!< Copy constructor
    IndicatorBuilder& operator=( const IndicatorBuilder& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    std::unique_ptr< indicators::Primitives > primitives_;
    std::unique_ptr< indicators::Variables > variables_;
    std::unique_ptr< indicators::DataTypeFactory > typeFactory_;
    std::unique_ptr< indicators::ElementFactory > factory_;
    //@}
};

}
}

#endif // __IndicatorBuilder_h_
