// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __MissionType_h_
#define __MissionType_h_

#include "OrderType.h"

namespace kernel
{
    class OrderContext;

// =============================================================================
/** @class  MissionType
    @brief  MissionType
*/
// Created: SBO 2007-04-23
// =============================================================================
class MissionType : public OrderType
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit MissionType( xml::xistream& xis );
             MissionType( xml::xistream& xis, const OrderContext& context );
    virtual ~MissionType();
    //@}

    //! @name Accessors
    //@{
    bool IsAutomat() const;
    //@}

private:
    //! @name Helpers
    //@{
    void Read( xml::xistream& xis );
    void ReadParameter( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    bool automat_;
    //@}
};

}

#endif // __MissionType_h_
