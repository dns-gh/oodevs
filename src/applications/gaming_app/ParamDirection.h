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

#include "game_asn/Asn.h"
#include "Param_ABC.h"

// =============================================================================
/** @class  ParamDirection
    @brief  ParamDirection
*/
// Created: AGE 2006-03-15
// =============================================================================
class ParamDirection : public Param_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ParamDirection( const QString& name );
    virtual ~ParamDirection();
    //@}

    //! @name Operations
    //@{
    virtual void BuildInterface( QWidget* parent );
    virtual void CommitTo( ASN1T_MissionParameter& asn ) const;
    virtual void CommitTo( ASN1T_OrderContext& asn ) const;
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
    QDial* dial_;
    //@}
};


#endif // __ParamDirection_h_
