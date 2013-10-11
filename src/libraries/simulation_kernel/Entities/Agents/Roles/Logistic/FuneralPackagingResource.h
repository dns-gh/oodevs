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

class PHY_DotationCategory;

namespace logistic
{
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
    explicit FuneralPackagingResource( xml::xistream& xis );
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

}

#endif // __FuneralPackagingResource_ABC_h_

