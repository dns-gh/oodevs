 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DotationQuota_h_
#define __DotationQuota_h_

namespace Common
{
    class MsgDotationQuota;
}

namespace dispatcher
{
    class Model_ABC;

// =============================================================================
/** @class  DotationQuota
    @brief  DotationQuota
*/
// Created: NLD 2006-09-19
// =============================================================================
class DotationQuota
{
public:
    //! @name Constructors/Destructor
    //@{
    DotationQuota( const Model_ABC& model, const Common::MsgDotationQuota& message );
    virtual ~DotationQuota();
    //@}

    //! @name Operations
    //@{
    void Update( const Common::MsgDotationQuota& message );
    void Send  ( Common::MsgDotationQuota& message ) const ;
    //@}

private:
    const unsigned int nDotationType_; // XML reference - no resolved by dispatcher
          unsigned int nQuota_;
};

}

#endif // __DotationQuota_h_
