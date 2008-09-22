// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterObstacleType_h_
#define __ActionParameterObstacleType_h_

#include "Parameter.h"

namespace actions {
    namespace parameters {

// =============================================================================
/** @class  ObstacleType
    @brief  ObstacleType
*/
// Created: SBO 2007-05-25
// =============================================================================
class ObstacleType : public Parameter< QString >
{

public:
    //! @name Constructors/Destructor
    //@{
             ObstacleType( const kernel::OrderParameter& parameter, unsigned int value );
    explicit ObstacleType( xml::xistream& xis );
    virtual ~ObstacleType();
    //@}

    //! @name Operations
    //@{
    void CommitTo( ASN1T_EnumObstacleType& asn ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ObstacleType( const ObstacleType& );            //!< Copy constructor
    ObstacleType& operator=( const ObstacleType& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void Serialize( xml::xostream& xos ) const;
    //@}

private:
    //! @name Member data
    //@{
    E_ObstacleType value_;
    //@}
};

    }
}

#endif // __ActionParameterObstacleType_h_
