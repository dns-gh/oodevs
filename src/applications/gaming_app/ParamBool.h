// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ParamBool_h_
#define __ParamBool_h_

#include "game_asn/Asn.h"
#include "Param_ABC.h"

// =============================================================================
/** @class  ParamBool
    @brief  ParamBool
*/
// Created: AGE 2006-03-15
// =============================================================================
class ParamBool : public Param_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ParamBool( ASN1BOOL& asn, const QString& name, bool defaultValue = false );
    virtual ~ParamBool();
    //@}

    //! @name Operations
    //@{
    virtual void BuildInterface( QWidget* parent );
    virtual void Commit();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ParamBool( const ParamBool& );            //!< Copy constructor
    ParamBool& operator=( const ParamBool& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    ASN1BOOL& asn_;
    const bool default_;
    QCheckBox* checkBox_;
    //@}
};

#endif // __ParamBool_h_
