// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ParamDirection_h_
#define __ParamDirection_h_

#include "gaming/ASN_Types.h"
#include "Param_ABC.h"

// =============================================================================
/** @class  ParamDirection
    @brief  ParamDirection
*/
// Created: AGE 2006-03-15
// =============================================================================
class ParamDirection : public QHBox, public Param_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ParamDirection( QWidget* parent, ASN1T_Direction& direction, const QString& label );
    virtual ~ParamDirection();
    //@}

    //! @name Operations
    //@{
    virtual void Commit();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ParamDirection( const ParamDirection& );            //!< Copy constructor
    ParamDirection& operator=( const ParamDirection& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    ASN1T_Direction& direction_;
    QDial* pDial_;
    //@}
};


#endif // __ParamDirection_h_
