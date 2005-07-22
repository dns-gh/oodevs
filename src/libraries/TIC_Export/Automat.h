// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-05-18 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Export/Automat.h $
// $Author: Sbo $
// $Modtime: 24/06/05 10:19 $
// $Revision: 6 $
// $Workfile: Automat.h $
//
// *****************************************************************************

#ifndef __TICExport_Automat_h_
#define __TICExport_Automat_h_

#include <string>

namespace TIC
{
    class Automat;
    class Pawn;
}

namespace TICExport
{
    class Pawn;

// =============================================================================
/** @class  Automat
    @brief  Automat
    @par    Using example
    @code
    Automat;
    @endcode
*/
// Created: SBO 2005-05-18
// =============================================================================
class Automat
{
public:
	//! @name Constructors/Destructor
    //@{
             Automat();
             Automat( unsigned int nIdx );
    virtual ~Automat();
    //@}

    //! @name Accessors
    //@{
    const Pawn         GetPc            () const;
          unsigned int GetPawnCount     () const;
    const Pawn         GetPawnAt        ( unsigned int nIdx ) const;

          unsigned int GetId            () const;
    const std::string& GetTeam          () const;
          unsigned int GetKnowledgeGroup() const;
    //@}

private:
	//! @name Member data
    //@{
    const TIC::Automat*   pAutomat_;
	//@}
};

} // end namespace TICExport

#endif // __TICExport_Automat_h_
