 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Object_h_
#define __Object_h_

#include "game_asn/Simulation.h"
#include "Localisation.h"
#include "Entity_ABC.h"

namespace dispatcher
{
    class Side;
    class Model;
    class ObjectAttribute_ABC;

// =============================================================================
/** @class  Object
    @brief  Object
*/
// Created: NLD 2006-09-19
// =============================================================================
class Object : public Entity_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Object( Model& model, const ASN1T_MsgObjectCreation& msg );
    virtual ~Object();
    //@}

    //! @name Accessors
    //@{
    unsigned long GetID() const;
    //@}

    //! @name Operations
    //@{
    using Entity_ABC::Update;
    void Update( const ASN1T_MsgObjectUpdate&   msg );
    virtual void SendCreation   ( ClientPublisher_ABC& publisher ) const;
    virtual void SendFullUpdate ( ClientPublisher_ABC& publisher ) const;
    virtual void SendDestruction( ClientPublisher_ABC& publisher ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Object( const Object& );            //!< Copy constructor
    Object& operator=( const Object& ); //!< Assignement operator
    //@}

private:
    //! @name Types
    //@{
    //$$$ bullshit
    struct T_Optionals 
    {
        unsigned obstacle_de_manoeuvre_activePresent : 1;
        unsigned type_obstaclePresent                : 1;
    };
    //@}

public:
    const unsigned long          nID_;
    const ASN1T_EnumObjectType   nType_; // XML reference - no resolved by dispatcher
    const ASN1T_EnumObstacleType nObstacleType_;
    const std::string            strName_;
          Localisation           localisation_;
          Side&                  side_;
    const unsigned int           nTypeDotationForConstruction_; // XML reference - no resolved by dispatcher
    const unsigned int           nTypeDotationForMining_; // XML reference - no resolved by dispatcher

    ObjectAttribute_ABC* pAttributes_;
    unsigned int         nPercentageConstruction_;
    unsigned int         nPercentageMining_;
    unsigned int         nPercentageBypassing_;
    bool                 bReservedObstacleActivated_;
    unsigned int         nNbrDotationForConstruction_;
    unsigned int         nNbrDotationForMining_;

    T_Optionals          optionals_;
};

}

#include "Object.inl"

#endif // __Object_h_
