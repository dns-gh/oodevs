// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PopulationConcentration_h_
#define __PopulationConcentration_h_

#include "ASN_Types.h"
#include "PopulationPart_ABC.h"
#include "Updatable_ABC.h"

// =============================================================================
// Created: HME 2005-09-29
// =============================================================================
class PopulationConcentration : public PopulationPart_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             PopulationConcentration( const ASN1T_MsgPopulationConcentrationCreation& asnMsg, MT_Float density );
    virtual ~PopulationConcentration();
    //@}

    //! @name Accessors
    //@{
//	virtual const MT_Vector2D& GetPos    () const;
    MT_Float   GetArea   () const;

	virtual std::string GetName   () const;
    virtual unsigned int GetLivingHumans() const;
    virtual unsigned int GetDeadHumans() const;
    virtual unsigned int GetDensity() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    PopulationConcentration( const PopulationConcentration& );            //!< Copy constructor
    PopulationConcentration& operator=( const PopulationConcentration& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const ASN1T_MsgPopulationConcentrationUpdate& asnMsg );
    //@}

private:
    //! @name Member data
    //@{
    MT_Vector2D position_;

    MT_Float density_;
    uint     nID_;
    int      nLivingHumans_;
    int      nDeadHumans_;
    //@}			
};

#endif // __PopulationConcentration_h_
