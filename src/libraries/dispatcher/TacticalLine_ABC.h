// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TacticalLine_ABC_h_
#define __TacticalLine_ABC_h_

#include "game_asn/Asn.h"
#include "Localisation.h"
#include "Entity_ABC.h"

namespace dispatcher
{
    class Automat;
    class Formation;
    class Model;

// =============================================================================
/** @class  TacticalLine_ABC
    @brief  TacticalLine_ABC
*/
// Created: SBO 2006-11-15
// =============================================================================
class TacticalLine_ABC : public Entity_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             TacticalLine_ABC( const Model& model, unsigned int id, const ASN1T_TacticalLine& asn );
    virtual ~TacticalLine_ABC();
    //@}

    //! @name Operations
    //@{
    void Send( ASN1T_Location& asn ) const;
    //@}

protected:
    //! @name Operations
    //@{
    void Update( const ASN1T_TacticalLine& asn );
    void Send  ( ASN1T_TacticalLine& asn ) const;
    //@}

    //! @name Accessors
    //@{
    unsigned int GetID() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    TacticalLine_ABC( const TacticalLine_ABC& );            //!< Copy constructor
    TacticalLine_ABC& operator=( const TacticalLine_ABC& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void UpdateDiffusion( const ASN1T_TacticalLinesDiffusion& diffusion );
    //@}

public:
    //! @name Member data
    //@{
    const Model&        model_;
    const unsigned int  id_;
          std::string   strName_;
          Localisation  location_;
    const Automat*      automat_;
    const Formation*    formation_;
    //@}
};

}

#endif // __TacticalLine_ABC_h_

