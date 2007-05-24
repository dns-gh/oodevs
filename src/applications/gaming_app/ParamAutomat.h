// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ParamAutomat_h_
#define __ParamAutomat_h_

#include "clients_kernel/Automat_ABC.h"
#include "EntityParameter.h"

// =============================================================================
/** @class  ParamAutomat
    @brief  ParamAutomat
*/
// Created: AGE 2006-11-29
// =============================================================================
class ParamAutomat : public EntityParameter< kernel::Automat_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             ParamAutomat( QObject* parent, const kernel::OrderParameter& parameter );
             ParamAutomat( QObject* parent, const QString& name, const kernel::Automat_ABC& entity );
             ParamAutomat( QObject* parent, const QString& name, bool optional );
    virtual ~ParamAutomat();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( ASN1T_MissionParameter& asn ) const;
    virtual void CommitTo( Action_ABC& action ) const;
    void CommitTo( ASN1T_Automate& asn ) const;
    void CommitTo( ActionParameter_ABC& param ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ParamAutomat( const ParamAutomat& );            //!< Copy constructor
    ParamAutomat& operator=( const ParamAutomat& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    const kernel::OrderParameter* parameter_;
    //@}
};

#endif // __ParamAutomat_h_
