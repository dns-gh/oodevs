// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __BreakdownPart_h_
#define __BreakdownPart_h_

namespace tools
{
    template< typename T, typename U > class Resolver_ABC;
}

namespace kernel
{
    class DotationType;

// =============================================================================
/** @class  BreakdownPart
    @brief  BreakdownPart
*/
// Created: ABR 2014-02-20
// =============================================================================
class BreakdownPart
{

public:
    //! @name Constructors/Destructor
    //@{
             BreakdownPart();
             BreakdownPart( xml::xistream& xis, const tools::Resolver_ABC< DotationType, std::string >& dotationResolver );
    virtual ~BreakdownPart();
    //@}

    //! @name Operations
    //@{
    unsigned int GetQuantity() const;
    const DotationType& GetResource() const;
    //@}

private:
    //! @name Member data
    //@{
    unsigned int quantity_;
    const DotationType* resource_;
    //@}
};

} //! namespace kernel

#endif // __BreakdownPart_h_
