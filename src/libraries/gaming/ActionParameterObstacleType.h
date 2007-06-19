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

#include "ActionParameter.h"

// =============================================================================
/** @class  ActionParameterObstacleType
    @brief  ActionParameterObstacleType
*/
// Created: SBO 2007-05-25
// =============================================================================
class ActionParameterObstacleType : public ActionParameter< QString >
{

public:
    //! @name Constructors/Destructor
    //@{
             ActionParameterObstacleType( const kernel::OrderParameter& parameter, unsigned int value );
    explicit ActionParameterObstacleType( xml::xistream& xis );
    virtual ~ActionParameterObstacleType();
    //@}

    //! @name Operations
    //@{
    void CommitTo( ASN1T_EnumObstacleType& asn ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionParameterObstacleType( const ActionParameterObstacleType& );            //!< Copy constructor
    ActionParameterObstacleType& operator=( const ActionParameterObstacleType& ); //!< Assignment operator
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

#endif // __ActionParameterObstacleType_h_
