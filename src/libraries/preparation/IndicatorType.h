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

namespace xml
{
    class xistream;
}

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
    virtual ~IndicatorType();
    //@}

    //! @name Operations
    //@{
    std::string ToString() const;
    bool operator==( const IndicatorType& rhs ) const;
    bool operator!=( const IndicatorType& rhs ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    IndicatorType( const IndicatorType& );            //!< Copy constructor
    IndicatorType& operator=( const IndicatorType& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    const std::string type_;
    //@}
};

#endif // __IndicatorType_h_
