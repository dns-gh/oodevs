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

#include "PopulationPart_ABC.h"

#include "OptionalValue.h"

class Population;

// =============================================================================
// Created: HME 2005-09-29
// =============================================================================
class PopulationFlow : public PopulationPart_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
     PopulationFlow( const ASN1T_MsgPopulationFluxCreation& asnMsg, const Population& );
    ~PopulationFlow();
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
    PopulationFlow( const PopulationFlow& );            //!< Copy constructor
    PopulationFlow& operator=( const PopulationFlow& ); //!< Assignement operator
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

#include "PopulationFlow.inl"

#endif // __PopulationFlow_h_
