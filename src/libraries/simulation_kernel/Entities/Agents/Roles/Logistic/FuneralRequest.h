// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __FuneralRequest_h_
#define __FuneralRequest_h_

#include "FuneralRequest_ABC.h"

class Human_ABC;

namespace logistic {
    class LogisticHierarchy_ABC;

// =============================================================================
/** @class  FuneralRequest
    @brief  FuneralRequest
*/
// Created: NLD 2011-08-24
// =============================================================================
class FuneralRequest : public FuneralRequest_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             FuneralRequest( Human_ABC& human );
    virtual ~FuneralRequest();
    //@}

    //! @name Operations
    //@{
    virtual const LogisticHierarchy_ABC& GetLogisticHierarchy() const;
    virtual const MT_Vector2D&           GetPosition() const;
    //@}

    //! @name Events
    //@{
    virtual void OnHandledByFuneral();
    virtual void OnBackFromFuneral ();
    //@}

    //! @name Network
    //@{
    virtual sword::EnumHumanRank GetRank() const;
    virtual void Serialize( sword::UnitId& unitId ) const;
    //@}

private:
    Human_ABC& human_;
};

} // end namespace logistic

#endif // __FuneralRequest_h_
