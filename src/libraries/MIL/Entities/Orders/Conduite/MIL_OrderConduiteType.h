// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Orders/Conduite/MIL_OrderConduiteType.h $
// $Author: Nld $
// $Modtime: 19/10/04 13:55 $
// $Revision: 1 $
// $Workfile: MIL_OrderConduiteType.h $
//
// *****************************************************************************

#ifndef __MIL_OrderConduiteType_h_
#define __MIL_OrderConduiteType_h_

#include "MIL.h"
#include "Network/NET_ASN_Types.h"

class MIL_OrderConduite_ABC;
class MIL_OrderConduiteRequest_ABC;
class MIL_KnowledgeGroup;

// =============================================================================
// @class  MIL_OrderConduiteType
// Created: JVT 2004-08-03
// =============================================================================
class MIL_OrderConduiteType
{
    MT_COPYNOTALLOWED( MIL_OrderConduiteType )

public:
    //! @name Manager
    //@{
    template< typename OrderConduite > static 
    void RegisterOrderConduite( const std::string& strName, uint nConduiteID, uint nAsnConduiteID, const std::string& strConduiteDIATypeName, bool bAvailableForAllMissions, bool bAvailableWithoutMission, uint nRequestAsnID = (uint)-1 );
    static void InitializeDIA();
    static void Terminate ();

    static const MIL_OrderConduiteType* FindOrderConduiteType( const std::string& strName );
    static const MIL_OrderConduiteType* FindOrderConduiteType( uint nConduiteID );
    static const MIL_OrderConduiteType* FindOrderConduiteType( const ASN1T_MsgOrderConduite_order_conduite& asn );
    //@}

    //! @name Operations
    //@{
    bool IsAvailableForAllMissions() const;
    bool IsAvailableWithoutMission() const;
    bool CanBeRequested           () const;
    //@}

    //! @name Accessors
    //@{
    uint               GetConduiteID         () const;
    const std::string& GetConduiteDIATypeName() const;
    uint               GetAsnRequestID       () const;
    //@}

    //! @name Operations
    //@{
    MIL_OrderConduite_ABC&        InstanciateOrderConduite       ( const MIL_KnowledgeGroup& knowledgeGroup ) const;
    MIL_OrderConduite_ABC&        InstanciateOrderConduite       () const;
    MIL_OrderConduiteRequest_ABC& InstanciateOrderConduiteRequest() const;
    bool operator==( const MIL_OrderConduiteType& rhs ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< const MIL_OrderConduiteType* > T_OrderConduiteFromIDVector;
    typedef T_OrderConduiteFromIDVector::const_iterator CIT_OrderConduiteFromIDVector;

    typedef std::map< uint /*asn*/, const MIL_OrderConduiteType* > T_OrderConduiteFromAsnIDMap;
    typedef T_OrderConduiteFromAsnIDMap::const_iterator            CIT_OrderConduiteFromAsnIDMap;

    typedef std::map< std::string, const MIL_OrderConduiteType* > T_OrderConduiteFromNameMap;
    typedef T_OrderConduiteFromNameMap::const_iterator            CIT_OrderConduiteFromNameMap;

    typedef MIL_OrderConduite_ABC&(* T_OrderConduiteAllocator )( const MIL_KnowledgeGroup* pKnowledgeGroup, const MIL_OrderConduiteType& type );
    typedef void(* T_DiaInitializer )( const MIL_OrderConduiteType& type );
    //@}

private:
     MIL_OrderConduiteType( const std::string& strName, uint nConduiteID, uint nAsnConduiteID, const std::string& strConduiteDIATypeName, T_OrderConduiteAllocator allocator, T_DiaInitializer diaInitializer, bool bAvailableForAllMissions, bool bAvailableWithoutMission, uint nRequestAsnID  = (uint)-1 );
    ~MIL_OrderConduiteType();

private:
    const std::string              strName_;
    const uint                     nConduiteID_;
    const uint                     nAsnConduiteID_;
    const std::string              strConduiteDIATypeName_;
    const T_OrderConduiteAllocator orderConduiteAllocator_;
    const T_DiaInitializer         orderConduiteDiaInitializer_;
    const bool                     bAvailableForAllMissions_;
    const bool                     bAvailableWithoutMission_;

    const uint                     nAsnRequestID_;

private:
    static T_OrderConduiteFromIDVector        orderConduitesFromID_;
    static T_OrderConduiteFromAsnIDMap        orderConduitesFromAsnID_;
    static T_OrderConduiteFromNameMap         orderConduitesFromName_;
};

#include "MIL_OrderConduiteType.inl"

#endif // __MIL_OrderConduiteType_h_
