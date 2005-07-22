// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-05-11 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/communications/CommunicationLink.h $
// $Author: Sbo $
// $Modtime: 12/07/05 18:52 $
// $Revision: 4 $
// $Workfile: CommunicationLink.h $
//
// *****************************************************************************

#ifndef __CommunicationLink_h_
#define __CommunicationLink_h_

#include "Types.h"
#include "Communications/CommunicationManager.h"
#include "Entities/Automat.h"
#include "Entities/Pawn.h"

namespace TIC
{
    class CommunicationType;

// =============================================================================
/** @class  CommunicationLink
    @brief  CommunicationLink
    @par    Using example
    @code
    CommunicationLink;
    @endcode
*/
// Created: SBO 2005-05-24
// =============================================================================
class CommunicationLink
{
public:
    //! @name Comparator
    //@{
    struct sCommunicationLinkLess
    {
        bool operator()( T_EntityId lhs, T_EntityId rhs ) const
        {
            return lhs < rhs;
        }

        bool operator()( const Pawn& lhs, const Pawn& rhs ) const
        {
            return operator() ( lhs.GetId(), rhs.GetId() );
        }

        bool operator()( const Automat& lhs, const Automat& rhs ) const
        {
            return operator() ( lhs.GetId(), rhs.GetId() );
        }

        bool operator()(const CommunicationLink* lhs, const CommunicationLink* rhs) const
        {
            return ( operator() ( *lhs->pPawn_   , *rhs->pPawn_    ) || 
                    !operator() ( *rhs->pPawn_   , *lhs->pPawn_    ) && 
                     operator() ( *lhs->pAutomat_, *rhs->pAutomat_ ) );
        }
    };
    //@}

public:
    //! @name Static Operations
    //@{
    static       void               Initialize();
    static       void               Terminate ();
    static       void               Register  ( CommunicationLink& link );
    static       void               Flush     ();

    static       uint               GetCount  ();
    static const CommunicationLink* GetAt     ( uint nIdx );
    static       CommunicationLink* Find      ( const Automat& automat, const Pawn& pawn );
    //@}

    //! @name Constructors/Destructor
    //@{
    explicit CommunicationLink( const Automat& automat, const Pawn& pawn );
    virtual ~CommunicationLink();
    //@}

    //! @name Accessors
    //@{
          uint               GetAutomatId         () const;
          uint               GetPawnId            () const;
          uint               GetCommunicationCount( CommunicationManager::ESource eSource ) const;
    const Communication_ABC* GetCommunicationAt   ( CommunicationManager::ESource eSource, uint nIdx ) const;

          Communication_ABC* GetCommunicationBySubject( CommunicationManager::ESource eSource, T_EntityId nSubjectId );
          Communication_ABC* GetCommunicationByType   ( CommunicationManager::ESource eSource, 
                                                        const CommunicationType&      type  );
    //@}

    //! @name Operations
    //@{
    void RegisterCommunication ( CommunicationManager::ESource eSource, Communication_ABC& com );
    void OnCommunicationUpdated();
    //@}

    //! @name Operators
    //@{
    bool operator==( const CommunicationLink& link ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::multimap< CommunicationManager::ESource, Communication_ABC* > T_ComSourceMap;
    typedef T_ComSourceMap::iterator                                           IT_ComSourceMap;
    typedef T_ComSourceMap::const_iterator                                     CIT_ComSourceMap;

    typedef std::set< CommunicationLink*, sCommunicationLinkLess > T_ComLinkSet;
    typedef T_ComLinkSet::iterator         IT_ComLinkSet;
    typedef T_ComLinkSet::const_iterator   CIT_ComLinkSet;
    //@}

private:
    //! @name Member data
    //@{
    const Automat* pAutomat_;
    const Pawn*    pPawn_;

    T_ComSourceMap coms_;
    mutable bool   bUpdated_;
    //@}

    //! @name Static member
    //@{
    static T_ComLinkSet links_;
    //@}
};

} // end namespace TIC

#include "CommunicationLink.inl"

#endif // __CommunicationLink_h_
