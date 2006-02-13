// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Paths_h_
#define __Paths_h_

#include "ASN_Types.h"
#include "Extension_ABC.h"
#include "Updatable_ABC.h"

// =============================================================================
/** @class  Paths
    @brief  Paths
*/
// Created: AGE 2006-02-13
// =============================================================================
class Paths : public Extension_ABC
            , public Updatable_ABC< ASN1T_MsgUnitPathFind >
            , public Updatable_ABC< ASN1T_MsgUnitAttributes >
{

public:
    //! @name Constructors/Destructor
    //@{
             Paths();
    virtual ~Paths();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Paths( const Paths& );            //!< Copy constructor
    Paths& operator=( const Paths& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    typedef std::vector< MT_Vector2D > T_Points;
    //@}

    //! @name Helpers
    //@{
    virtual void Update( const ASN1T_MsgUnitPathFind& message );
    virtual void Update( const ASN1T_MsgUnitAttributes& message );
    //@}

private:
    //! @name Member data
    //@{
    T_Points plannedPath_;
    T_Points previousPath_;
    //@}
};

#endif // __Paths_h_
