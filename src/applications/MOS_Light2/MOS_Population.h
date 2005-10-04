// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MOS_Population_h_
#define __MOS_Population_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "MOS_Types.h"
#include "MOS_ASN_Types.h"

#include "MOS_PopulationPart_ABC.h"

class MOS_PopulationConcentration;
class MOS_PopulationFlux;
class MOS_Team;
class MOS_TypePopulation;


// =============================================================================
/** @class  MOS_Population
    @brief  MOS_Population
    @par    Using example
    @code
    MOS_Population;
    @endcode
*/
// Created: HME 2005-09-29
// =============================================================================
class MOS_Population
{
    friend class MOS_GLTool;
public :

	typedef std::map< MIL_AgentID , MOS_PopulationConcentration* >  T_ConcentrationMap;
	typedef T_ConcentrationMap::iterator							IT_ConcentrationMap;
	typedef T_ConcentrationMap::const_iterator						CIT_ConcentrationMap;

	typedef std::map< MIL_AgentID , MOS_PopulationFlux* >			T_FluxMap;
	typedef T_FluxMap::iterator										IT_FluxMap;
	typedef T_FluxMap::const_iterator								CIT_FluxMap;

public:

             MOS_Population( const ASN1T_MsgPopulationCreation& asnMsg );
    virtual ~MOS_Population();


	void	UpdatePopulationFlux			( const ASN1T_MsgPopulationFluxUpdate& asnMsg );
	void	UpdatePopulationConcentration	( const ASN1T_MsgPopulationConcentrationUpdate& asnMsg );
	void	CreatePopulationFlux			( const ASN1T_MsgPopulationFluxCreation& asnMsg );
	void	CreatePopulationConcentration	( const ASN1T_MsgPopulationConcentrationCreation& asnMsg );
	void	DeletePopulationFlux			( const ASN1T_MsgPopulationFluxDestruction& asnMsg );
	void	DeletePopulationConcentration	( const ASN1T_MsgPopulationConcentrationDestruction& asnMsg );
	void	UpdatePopulation				( const ASN1T_MsgPopulationUpdate& asnMsg ); 

	MIL_AgentID			GetPopulationID();
	const MOS_Team&		GetTeam();
	const std::string&	GetName();
	const MT_Vector2D&	GetPos();
	uint				GetLivingHumans();
	uint				GetDeadHumans();

public:

	class iterator
	{
	public:
		iterator( T_ConcentrationMap& cm, T_FluxMap& fm, bool e = true )
		: concentrationMap_	( cm )
		, fluxMap_			( fm )
		{
			onCon_  = e;
			if ( e )
			{
				itCon   = concentrationMap_.begin();
				itFlux	= fluxMap_.begin();
			}
			else
			{
				itCon   = concentrationMap_.end();
				itFlux	= fluxMap_.end();
			}
		}
		~iterator()
		{
		}
		iterator& operator++()
		{
			if ( onCon_ )
			{
				++itCon;
				if ( itCon == concentrationMap_.end() )
					onCon_ = false;
			}
			else
				++itFlux;
			return *this;
		}
		MOS_PopulationPart_ABC*	operator*()
		{
			if ( onCon_ )
				return (MOS_PopulationPart_ABC*) itCon->second;
			else
				return (MOS_PopulationPart_ABC*) itFlux->second;
		}
		bool operator==( iterator& it )
		{
			return ( itCon == it.itCon && itFlux == it.itFlux );
		}
		bool operator!=( iterator& it )
		{
			return ( itCon != it.itCon || itFlux != it.itFlux );
		}
		iterator& operator=( iterator& it )
		{
			itCon = it.itCon;
			itFlux = it.itFlux;
			onCon_ = it.onCon_;
			return *this;
		}
	private:
		bool onCon_;
		IT_ConcentrationMap itCon;
		IT_FluxMap			itFlux;
		T_ConcentrationMap&			concentrationMap_;
		T_FluxMap&					fluxMap_;
	};

	iterator& begin()
	{
		return *new iterator( concentrationMap_ , fluxMap_ );
	};
	iterator& end()
	{
		return *new iterator( concentrationMap_ , fluxMap_, false );
	};

private:
    //! @name Member data
    //@{
	MIL_AgentID					nPopulationID_;
	T_ConcentrationMap			concentrationMap_;
	T_FluxMap					fluxMap_;
	const MOS_TypePopulation*	pTypePopulation_;
	MOS_Team*					pTeam_;
	std::string					sName_;
    //@}
};



#include "MOS_Population.inl"

#endif // __MOS_Population_h_
