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
             MissionType( xml::xistream& xis, bool context );
    virtual ~MissionType();
    //@}

    //! @name Operations
    //@{
    bool IsAutomat() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MissionType( const MissionType& );            //!< Copy constructor
    MissionType& operator=( const MissionType& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadParameter( xml::xistream& xis );
    void AddContextParameters();
    //@}

private:
    //! @name Member data
    //@{
    bool automat_;
    //@}
};

}

#endif // __MissionType_h_
