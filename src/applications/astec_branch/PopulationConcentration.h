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

// =============================================================================
// Created: HME 2005-09-29
// =============================================================================
class PopulationConcentration
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit PopulationConcentration( const ASN1T_MsgPopulationConcentrationCreation& asnMsg );
    virtual ~PopulationConcentration();
    //@}

    //! @name Operations
    //@{
	void Update( const ASN1T_MsgPopulationConcentrationUpdate& asnMsg );
    //@}

    //! @name Accessors
    //@{
	virtual const MT_Vector2D& GetPos    () const;
	virtual const std::string& GetName   () const;
            MT_Float           GetArea   () const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    PopulationConcentration( const PopulationConcentration& );            //!< Copy constructor
    PopulationConcentration& operator=( const PopulationConcentration& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    const std::string strName_;
    MT_Vector2D position_;

    uint                 nID_;
    int                  nLivingHumans_;
    int                  nDeadHumans_;
    E_PopulationAttitude attitude_;
    //@}			
};

#include "PopulationConcentration.inl"

#endif // __PopulationConcentration_h_
