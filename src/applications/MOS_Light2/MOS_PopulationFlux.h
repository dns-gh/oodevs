// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MOS_PopulationFlux_h_
#define __MOS_PopulationFlux_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "MOS_PopulationPart_ABC.h"

class MOS_Population;


// =============================================================================
/** @class  MOS_PopulationFlux
    @brief  MOS_PopulationFlux
    @par    Using example
    @code
    MOS_PopulationFlux;
    @endcode
*/
// Created: HME 2005-09-29
// =============================================================================
class MOS_PopulationFlux : public MOS_PopulationPart_ABC
{
    friend class MOS_GLTool;
public:
    //! @name Constructors/Destructor
    //@{
             MOS_PopulationFlux( uint , MOS_Population* );
    virtual ~MOS_PopulationFlux();
    //@}

    //! @name Operations
    //@{
	void Update( const ASN1T_MsgPopulationFluxUpdate& asnMsg );
    //@}

    //! @name Accessors
    //@{
	const MT_Vector2D& GetPos();
	uint				GetLivingHumans();
	uint				GetDeadHumans();
	uint				GetID();
	std::string			GetName();
	std::string GetStringAttitude();
    //@}

	bool HasFlux;
	bool HasItineraire;
	bool HasDirection;	
	bool HasVitesse;		
	bool HasLivingHumans;	
	bool HasDeadHumans;	
	bool HasAttitude;		

public:

	class iterator
	{
	public:
		iterator( T_PointVector& flux )
			: flux_			( flux )
		{
			it = flux_.begin();
		}
		~iterator()
		{
		}
		iterator& operator++()
		{
			++it;
			return *this;
		}
		MT_Vector2D&	operator*()
		{
			return *it;
		}
		bool operator==( iterator& it2 )
		{
			return ( it == it2.it && it == it2.it );
		}
		bool operator!=( iterator& it2 )
		{
			return ( it != it2.it || it != it2.it );
		}
		iterator& operator=( iterator& it2 )
		{
			it = it2.it;
			flux_ = it2.flux_;
			return *this;
		}
	private:
		IT_PointVector					it;
		T_PointVector&					flux_;
	};

	iterator& begin()
	{
		return *new iterator( flux_ );
	};
	iterator& end()
	{
		return *new iterator( flux_ );
	};


private:
    //! @name Copy/Assignement
    //@{
    MOS_PopulationFlux( const MOS_PopulationFlux& );            //!< Copy constructor
    MOS_PopulationFlux& operator=( const MOS_PopulationFlux& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
	MOS_Population*					parent_;
	uint							nID_;
	T_PointVector					itineraire_;
	T_PointVector					flux_;
	int								direction_;
	int								vitesse_;
	int								nLivingHumans_;
	int								nDeadHumans_;
	ASN1T_EnumPopulationAttitude	attitude_;
    //@}
};



#include "MOS_PopulationFlux.inl"

#endif // __MOS_PopulationFlux_h_
