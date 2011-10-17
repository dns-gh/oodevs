// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __FuneralPackagingResource_ABC_h_
#define __FuneralPackagingResource_ABC_h_

#include <boost/enable_shared_from_this.hpp>

class PHY_DotationCategory;

namespace logistic {

// =============================================================================
/** @class  FuneralPackingResource
    @brief  FuneralPackingResource
*/
// Created: NLD 2011-08-24
// =============================================================================
class FuneralPackagingResource
{
public:
    //! @name Constructors/Destructor
    //@{
    FuneralPackagingResource( xml::xistream& xis );
    virtual ~FuneralPackagingResource();
    //@}

    //! @name Getters
    //@{
    const PHY_DotationCategory& GetDotationCategory() const;
          unsigned              GetProcessDuration() const;
          bool                  IsTerminal() const;
    //@}

private:
    const PHY_DotationCategory* dotationCategory_;
    unsigned processDuration_;
    bool terminal_;
};

} // end namespace logistic

#endif // __FuneralPackagingResource_ABC_h_

