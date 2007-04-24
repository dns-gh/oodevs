// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __FragOrderType_h_
#define __FragOrderType_h_

#include "OrderType.h"

namespace kernel
{

// =============================================================================
/** @class  FragOrderType
    @brief  FragOrderType
*/
// Created: SBO 2007-04-23
// =============================================================================
class FragOrderType : public OrderType
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit FragOrderType( xml::xistream& xis );
    virtual ~FragOrderType();
    //@}

    //! @name Operations
    //@{
    bool IsDefaultOrder   () const;
    bool IsMissionRequired() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    FragOrderType( const FragOrderType& );            //!< Copy constructor
    FragOrderType& operator=( const FragOrderType& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadParameter( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    bool isDefaultOrder_;
    bool isWithoutMission_;
    //@}
};

}

#endif // __FragOrderType_h_
