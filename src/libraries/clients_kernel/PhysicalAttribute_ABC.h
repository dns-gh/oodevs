// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __PhysicalAttribute_ABC_h_
#define __PhysicalAttribute_ABC_h_

#include "Extension_ABC.h"
#include "Displayable_ABC.h"
#include <boost/noncopyable.hpp>

namespace kernel
{
    class Architecture_ABC;
    class Usages_ABC;

// =============================================================================
/** @class  PhysicalAttribute_ABC
    @brief  PhysicalAttribute_ABC
*/
// Created: ABR 2012-05-22
// =============================================================================
class PhysicalAttribute_ABC : public Extension_ABC
                            , public Displayable_ABC
                            , private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
             PhysicalAttribute_ABC();
    virtual ~PhysicalAttribute_ABC();
    //@}

    //! @name Accessors
    //@{
    virtual Architecture_ABC& GetArchitecture() const;
    virtual Usages_ABC& GetUsages() const;
    virtual unsigned int FindUsagesValue( const std::string& motivation ) const;
    virtual const std::string& GetInvalidMaterial() const;
    virtual const std::string& GetInvalidRoofShape() const;
    //@}

    //! @name Displayable_ABC
    //@{
    virtual void Display         ( Displayer_ABC& ) const;
    virtual void DisplayInList   ( Displayer_ABC& ) const;
    virtual void DisplayInTooltip( Displayer_ABC& ) const;
    virtual void DisplayInSummary( Displayer_ABC& ) const;
    //@}

protected:
    //! @name Member data
    //@{
    std::unique_ptr< Architecture_ABC > architecture_;
    std::unique_ptr< Usages_ABC > usages_;
    //@}
};

} //! namespace kernel

#endif // __PhysicalAttribute_ABC_h_
