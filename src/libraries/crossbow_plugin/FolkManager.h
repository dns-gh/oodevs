// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __crossbow_FolkManager_h_
#define __crossbow_FolkManager_h_

#include "game_asn/Asn.h"
#include <vector>

namespace dispatcher
{
    class Config;
}

namespace crossbow
{    
// =============================================================================
/** @class  FolkManager
    @brief  FolkManager
*/
// Created: JCR 2007-08-28
// =============================================================================
class FolkManager
{
    //! @name Types
    //@{
    typedef std::vector<std::string>    T_Trait;
    //@}
public:
    //! @name Types
    //@{
	struct PopulationInfo
	{
		PopulationInfo() : id_ ( 0 ), population_ ( 0 ), containers_ ( 5, 0 ) {}
		int		 id_;
		unsigned population_; 
		std::vector<unsigned> containers_;
	};
	typedef std::vector<PopulationInfo>	T_PopulationInfoVector;
	typedef T_PopulationInfoVector::iterator		IT_PopulationInfoVector;
	typedef T_PopulationInfoVector::const_iterator	CIT_PopulationInfoVector;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    explicit FolkManager( const dispatcher::Config& config );
    virtual ~FolkManager();
    //@}

    //! @name Operations
    //@{
	void Initialize();
    void Send( const ASN1T_MsgFolkCreation& asn );
	void Send( const ASN1T_MsgFolkGraphUpdate& asn );    
	//@}

    //! @name 
    //@{
    bool  IsUpdated() const;
	const T_PopulationInfoVector& GetPopulationInfo() const;
    //@}

    //! @name Accessors
    //@{
    unsigned Filter( const ASN1T_MsgFolkGraphEdgeUpdate& asn, /*out*/ std::vector<unsigned>& container ) const;    
    //@}

private:
    //! @name Copy/Assignement
    //@{
    FolkManager( const FolkManager& );            //!< Copy constructor
    FolkManager& operator=( const FolkManager& ); //!< Assignement operator
    //@}

    //! @name 
    //@{
    template< typename MsgFolkManagerCreation_trait >
    void Update ( T_Trait& trait, const MsgFolkManagerCreation_trait& asn );
	void Send( const ASN1T_MsgFolkGraphEdgeUpdate& asn );    
    //@}

private:
    //! @name Member data
    //@{    
    T_Trait     profiles_;
    T_Trait     activities_;
    unsigned    container_size_;
    //@}

    //! @name 
    //@{
	T_PopulationInfoVector dump_;
	IT_PopulationInfoVector	it_;
    //@}
};

}

#endif // __dispatcher_FolkManager_h_
