// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-05-11 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Entities/ConcreteEntity.h $
// $Author: Sbo $
// $Modtime: 19/07/05 16:31 $
// $Revision: 6 $
// $Workfile: ConcreteEntity.h $
//
// *****************************************************************************

#ifndef __ConcreteEntity_h_
#define __ConcreteEntity_h_

#include "Types.h"
#include "Tools/Position.h"
#include "Messages/ASN_Types.h"

namespace TIC
{

// =============================================================================
/** @class  ConcreteEntity
    @brief  ConcreteEntity
    @par    Using example
    @code
    ConcreteEntity;
    @endcode
*/
// Created: SBO 2005-05-11
// =============================================================================
class ConcreteEntity
{

public:
    //! @name Constructors/Destructor
    //@{
             ConcreteEntity();
    virtual ~ConcreteEntity();
    //@}

    //! @name Accessors
    //@{
          T_Direction  GetDirection() const;
          T_Speed      GetSpeed    () const;
    const Position&    GetPosition () const;
          int32        GetState    () const;
		  T_Height     GetHeight   () const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ConcreteEntity( const ConcreteEntity& );            //!< Copy constructor
    ConcreteEntity& operator=( const ConcreteEntity& ); //!< Assignement operator
    //@}

protected:
    //! @name Member data
    //@{
    T_Direction nDirection_;
    T_Speed     rSpeed_;
    Position    position_;
    T_Height    nHeight_;

    // state
    ASN1T_EnumEtatOperationnel eOpState_;

    //@}
};

} // end namespace TIC

#include "ConcreteEntity.inl"

#endif // __ConcreteEntity_h_
