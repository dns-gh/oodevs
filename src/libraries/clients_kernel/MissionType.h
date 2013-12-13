// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MissionType_h_
#define __MissionType_h_

#include "OrderType.h"
#include "ENT/ENT_Enums.h"

namespace kernel
{

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
    explicit MissionType( xml::xistream& xis, E_MissionType type_ );
    virtual ~MissionType();
    //@}

    //! @name Accessors
    //@{
    E_MissionType GetType() const;
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
    E_MissionType type_;
    //@}
};

}

#endif // __MissionType_h_
