// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __FuneralRequest_ABC_h_
#define __FuneralRequest_ABC_h_

#include "MT_Tools/MT_Vector2DTypes.h"

namespace sword {
    class UnitId;
    enum EnumHumanRank;
}

namespace logistic {
    class LogisticHierarchy_ABC;

// =============================================================================
/** @class  FuneralRequest_ABC
    @brief  FuneralRequest_ABC
*/
// Created: NLD 2011-08-24
// =============================================================================
class FuneralRequest_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             FuneralRequest_ABC() {}
    virtual ~FuneralRequest_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual const LogisticHierarchy_ABC& GetLogisticHierarchy() const = 0;
    virtual const MT_Vector2D&           GetPosition() const = 0;
    //@}

    //! @name Events
    //@{
    virtual void OnHandledByFuneral() = 0;
    virtual void OnBackFromFuneral () = 0;
    //@}

    //! @name Network
    //@{
    virtual sword::EnumHumanRank GetRank() const = 0;
    virtual void Serialize( sword::UnitId& unitId ) const = 0;
    //@}
};

} // end namespace logistic

#endif // __FuneralRequest_ABC_h_
