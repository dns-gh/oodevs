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

#ifdef __GNUG__
#   pragma interface
#endif

#include "MOS_Types.h"
#include "MOS_ASN_Types.h"
#include "MOS_PopulationPart_ABC.h"

class MOS_Population;


// =============================================================================
/** @class  MOS_PopulationConcentration
    @brief  MOS_PopulationConcentration
    @par    Using example
    @code
    MOS_PopulationConcentration;
    @endcode
*/
// Created: HME 2005-09-29
// =============================================================================
class MOS_PopulationConcentration : public MOS_PopulationPart_ABC
{
    friend class MOS_GLTool;
public:
    //! @name Constructors/Destructor
    //@{
             MOS_PopulationConcentration( uint, MOS_Population* );
    virtual ~MOS_PopulationConcentration();
    //@}

    //! @name Operations
    //@{
	void Update( const ASN1T_MsgPopulationConcentrationUpdate& asnMsg );
    //@}

    //! @name Accessors
    //@{
	const MT_Vector2D&	GetPos();
	uint				GetLivingHumans();
	uint				GetDeadHumans();
	//virtual std::string			GetStringAttitude();
    //@}

    bool HasAttitude;
	bool HasForme;
	bool HasLivingHumans;
	bool HasDeadHumans;
	bool HasDensity;

private:
    //! @name Copy/Assignement
    //@{
    MOS_PopulationConcentration( const MOS_PopulationConcentration& );            //!< Copy constructor
    MOS_PopulationConcentration& operator=( const MOS_PopulationConcentration& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
	MOS_Population*					parent_;
	uint							nID_;
	float							rDensity_;
	int								nLivingHumans_;
	int								nDeadHumans_;
	ASN1T_EnumPopulationAttitude	attitude_;
	T_PointVector					forme_;
	MT_Vector2D						center_;
    //@}			
};



#include "MOS_PopulationConcentration.inl"

#endif // __MOS_PopulationConcentration_h_
