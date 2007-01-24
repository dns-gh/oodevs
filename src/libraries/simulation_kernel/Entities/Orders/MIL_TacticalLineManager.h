// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MIL_TacticalLineManager_h_
#define __MIL_TacticalLineManager_h_

#include "MIL.h"

#include "network/Simulation_Asn.h"

class MIL_Lima;
class MIL_Limit;
class MIL_Formation;
class MIL_Automate;
class TER_LimitData;

// =============================================================================
/** @class  MIL_TacticalLineManager
    @brief  MIL_TacticalLineManager
*/
// Created: NLD 2006-11-10
// =============================================================================
class MIL_TacticalLineManager
{
public:
    //! @name Constructors/Destructor
    //@{
     MIL_TacticalLineManager();
    ~MIL_TacticalLineManager();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()   
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;
    //@}

    //! @name Nework
    //@{
    void OnReceiveMsgLimitCreationRequest   ( const ASN1T_MsgLimitCreationRequest&    asn, uint nCtx );   
    void OnReceiveMsgLimitDestructionRequest( const ASN1T_MsgLimitDestructionRequest& asn, uint nCtx );
    void OnReceiveMsgLimitUpdateRequest     ( const ASN1T_MsgLimitUpdateRequest&      asn, uint nCtx );
    void OnReceiveMsgLimaCreationRequest    ( const ASN1T_MsgLimaCreationRequest&     asn, uint nCtx );
    void OnReceiveMsgLimaDestructionRequest ( const ASN1T_MsgLimaDestructionRequest&  asn, uint nCtx ); 
    void OnReceiveMsgLimaUpdateRequest      ( const ASN1T_MsgLimaUpdateRequest&       asn, uint nCtx );      

    void SendStateToNewClient               () const;
    //@}

    //! @name Operations
    //@{
          void           CreateLimit     ( const MIL_Formation& formation, MIL_InputArchive& archive );
          void           CreateLimit     ( const MIL_Automate&  automate , MIL_InputArchive& archive );
          void           CreateLima      ( const MIL_Formation& formation, MIL_InputArchive& archive );
          void           CreateLima      ( const MIL_Automate&  automate , MIL_InputArchive& archive );
    const TER_LimitData& CreateLimitData ( const T_PointVector& points );
          void           DestroyLimitData( const TER_LimitData& data );   
    //@}

private:
    //! @name Copy/Assignement
    //@{
    MIL_TacticalLineManager( const MIL_TacticalLineManager& );            //!< Copy constructor
    MIL_TacticalLineManager& operator=( const MIL_TacticalLineManager& ); //!< Assignement operator
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< uint, MIL_Limit* > T_LimitMap;
    typedef T_LimitMap::iterator         IT_LimitMap;
    typedef T_LimitMap::const_iterator   CIT_LimitMap;

    typedef std::map< uint, MIL_Lima* > T_LimaMap;
    typedef T_LimaMap::iterator         IT_LimaMap;
    typedef T_LimaMap::const_iterator   CIT_LimaMap;
    
    typedef std::map< T_PointVector, TER_LimitData* > T_LimitDataMap;
    typedef T_LimitDataMap::iterator                  IT_LimitDataMap;
    typedef T_LimitDataMap::const_iterator            CIT_LimitDataMap;
    //@}

private:
    T_LimitMap      limits_;
    T_LimaMap       limas_;
    T_LimitDataMap  limitsData_;
};

#include "MIL_TacticalLineManager.inl"

#endif // __MIL_TacticalLineManager_h_
