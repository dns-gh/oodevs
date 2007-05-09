// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Formation_h_
#define __Formation_h_

#include "game_asn/Asn.h"
#include "ModelRefsContainer.h"
#include "Entity_ABC.h"

namespace dispatcher
{
    class Dispatcher;
    class Automat;
    class Model;
    class Side;
    class Publisher_ABC;

// =============================================================================
/** @class  Formation
    @brief  Formation
*/
// Created: NLD 2006-09-19
// =============================================================================
class Formation : public Entity_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Formation( Model& model, const ASN1T_MsgFormationCreation& msg );
    virtual ~Formation();
    //@}

    //! @name Accessors
    //@{
    ModelRefsContainer< Formation >& GetSubordinates();
    ModelRefsContainer< Automat   >& GetAutomats    ();
    unsigned long                    GetID          () const;
    //@}

    //! @name Main
    //@{
    void Update( const ASN1T_MsgFormationCreation& msg );
    void SendCreation  ( Publisher_ABC& publisher ) const;
    virtual void SendFullUpdate( Publisher_ABC& publisher ) const;
    void Accept( ModelVisitor_ABC& visitor );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Formation( const Formation& );            //!< Copy constructor
    Formation& operator=( const Formation& ); //!< Assignement operator
    //@}

private:
          Model&                               model_;
    const unsigned long                        nID_;
          Side&                                side_;
    const ASN1T_EnumNatureLevel                nLevel_;
          std::string                          strName_;
          Formation*                           pParent_;
          ModelRefsContainer< Automat >        automats_;
          ModelRefsContainer< Formation >      subordinates_;
};

}

#include "Formation.inl"

#endif // __Formation_h_
