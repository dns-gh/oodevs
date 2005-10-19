// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MOS_PopulationConcentration_h_
#define __MOS_PopulationConcentration_h_

#include "MOS_PopulationPart_ABC.h"

class MOS_Population;


// =============================================================================
// Created: HME 2005-09-29
// =============================================================================
class MOS_PopulationConcentration : public MOS_PopulationPart_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             MOS_PopulationConcentration( const ASN1T_MsgPopulationConcentrationCreation& asnMsg , const MOS_Population& pop );
             MOS_PopulationConcentration( MT_Vector2D point, E_PopulationAttitude attitude , int persons , const MOS_Population& parent  );
    virtual ~MOS_PopulationConcentration();
    //@}

    //! @name Operations
    //@{
	void Update( const ASN1T_MsgPopulationConcentrationUpdate& asnMsg );
    //@}

    //! @name Accessors
    //@{
	virtual const MT_Vector2D& GetPos () const;
	virtual const std::string& GetName() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    MOS_PopulationConcentration( const MOS_PopulationConcentration& );            //!< Copy constructor
    MOS_PopulationConcentration& operator=( const MOS_PopulationConcentration& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    const std::string strName_;
	      float		  rDensity_;
	      MT_Vector2D position_;
    //@}			
};

#include "MOS_PopulationConcentration.inl"

#endif // __MOS_PopulationConcentration_h_
