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
// $Archive: /MVW_v10/Build/SDK/TIC/src/Entities/Types/PopulationType.h $
// $Author: Sbo $
// $Modtime: 7/07/05 15:10 $
// $Revision: 4 $
// $Workfile: PopulationType.h $
//
// *****************************************************************************

#ifndef __PopulationType_h_
#define __PopulationType_h_

#include "Types.h"

namespace TEST
{
    class PopulationModel;
    class TypeManager;

// =============================================================================
// Created: SBO 2005-05-11
// =============================================================================
class PopulationType
{
    MT_COPYNOTALLOWED( PopulationType );

    //friend class Pawn;

public:
    //! @name Constructors/Destructor
    //@{
             PopulationType( const TypeManager& typeManager, const std::string& strName, XmlInputArchive& archive );
    virtual ~PopulationType();
    //@}

    //! @name Accessors
    //@{
    const std::string&      GetName () const;
          uint              GetId   () const;
    const PopulationModel&  GetModel() const;
    //@}

private:
    //! @name Member data
    //@{
    std::string            strName_;
	uint                   nId_;
    const PopulationModel* pModel_;
    //@}
};

} // end namespace TEST

#include "PopulationType.inl"

#endif // __PopulationType_h_
