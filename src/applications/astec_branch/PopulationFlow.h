// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PopulationFlow_h_
#define __PopulationFlow_h_

#include "ASN_Types.h"
#include "PopulationPart_ABC.h"
#include "Updatable_ABC.h"
#include "Extension_ABC.h"

class CoordinateConverter;

// =============================================================================
// Created: HME 2005-09-29
// =============================================================================
class PopulationFlow : public PopulationPart_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit PopulationFlow( const ASN1T_MsgPopulationFluxCreation& asnMsg, const CoordinateConverter& converter );
    virtual ~PopulationFlow();
    //@}

    //! @name Operations
    //@{
    virtual std::string  GetName() const;
    virtual unsigned int GetLivingHumans() const;
    virtual unsigned int GetDeadHumans() const;
    virtual unsigned int GetDensity() const;
    //@}

    void UpdatePathFind();
    //! @name Accessors
    //@{
//	virtual const std::string&   GetName            () const;
//    virtual const MT_Vector2D&   GetPos             () const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    PopulationFlow( const PopulationFlow& );            //!< Copy constructor
    PopulationFlow& operator=( const PopulationFlow& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const ASN1T_MsgPopulationFluxUpdate& asnMsg );
    //@}

private:
    //! @name Member data
    //@{
    const CoordinateConverter& converter_;

    uint          nID_;
	T_PointVector itineraire_;
	T_PointVector flow_;
	int			  nDirection_;
	int			  nSpeed_;
    
    int           nLivingHumans_;
    int           nDeadHumans_;
    float         rDensity_;
    //@}
};

#endif // __PopulationFlow_h_
