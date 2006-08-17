// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-05-17 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Entities/Types/PawnType.h $
// $Author: Sbo $
// $Modtime: 7/07/05 15:10 $
// $Revision: 4 $
// $Workfile: PawnType.h $
//
// *****************************************************************************

#ifndef __PawnType_h_
#define __PawnType_h_

#include "Types.h"

namespace TEST
{
    class PawnModel;
    class TypeManager;

// =============================================================================
/** @class  PawnType
    @brief  PawnType
    @par    Using example
    @code
    PawnType;
    @endcode
*/
// Created: SBO 2005-05-11
// =============================================================================
class PawnType
{
    MT_COPYNOTALLOWED( PawnType );

    //friend class Pawn;

public:
    //! @name Constructors/Destructor
    //@{
             PawnType( const TypeManager& typeManager, const std::string& strName, XmlInputArchive& archive );
    virtual ~PawnType();
    //@}

    //! @name Accessors
    //@{
    const std::string& GetName () const;
          uint         GetId   () const;
    const PawnModel&   GetModel() const;
    //@}

private:
    //! @name Member data
    //@{
    std::string          strName_;
	uint                 nId_;
    const PawnModel*     pModel_;
    //@}
};

} // end namespace TEST

#include "PawnType.inl"

#endif // __PawnType_h_
