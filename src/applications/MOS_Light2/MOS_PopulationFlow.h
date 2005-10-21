// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MOS_PopulationFlow_h_
#define __MOS_PopulationFlow_h_

#include "MOS_PopulationPart_ABC.h"

class MOS_Population;

// =============================================================================
// Created: HME 2005-09-29
// =============================================================================
class MOS_PopulationFlow : public MOS_PopulationPart_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
     MOS_PopulationFlow( const ASN1T_MsgPopulationFluxCreation& asnMsg, const MOS_Population& );
    ~MOS_PopulationFlow();
    //@}

    //! @name Operations
    //@{
	void Update( const ASN1T_MsgPopulationFluxUpdate& asnMsg );
    void UpdatePathFind();
    //@}

    //! @name Accessors
    //@{
            const T_PointVector& GetFlow            () const;
            const T_PointVector& GetItineraire      () const;
            const MT_Vector2D&   GetTailPosition    () const;
            const MT_Vector2D&   GetHeadPosition    () const;
	virtual const std::string&   GetName            () const;
    virtual const MT_Vector2D&   GetPos             () const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    MOS_PopulationFlow( const MOS_PopulationFlow& );            //!< Copy constructor
    MOS_PopulationFlow& operator=( const MOS_PopulationFlow& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    const std::string strName_;

	T_PointVector	  itineraire_;
	T_PointVector	  flow_;
	int				  nDirection_;
	int				  nSpeed_;
    //@}
};

#include "MOS_PopulationFlow.inl"

#endif // __MOS_PopulationFlow_h_
