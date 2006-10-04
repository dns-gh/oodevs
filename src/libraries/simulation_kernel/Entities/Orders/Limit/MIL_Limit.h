//*****************************************************************************
//
// $Created: NLD 2002-08-06 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Orders/Limit/MIL_Limit.h $
// $Author: Age $
// $Modtime: 13/05/05 18:13 $
// $Revision: 7 $
// $Workfile: MIL_Limit.h $
//
//*****************************************************************************

#ifndef __MIL_Limit_h_
#define __MIL_Limit_h_

#include "MIL.h"

#include "Network/NET_ASN_Types.h"

class MIL_Agent_ABC;
class MIL_Fuseau;
class TER_DynamicData;

//=============================================================================
// Created: NLD 2002-08-06
// Last modified: JVT 03-02-27
//=============================================================================
class MIL_Limit
{
    MT_COPYNOTALLOWED( MIL_Limit )

public:
     MIL_Limit();
    ~MIL_Limit();

    //! @name Init / Cleanup
    //@{
    bool Initialize( const ASN1T_MsgLimitCreation& asnMsg, MIL_MOSContextID nCtx );
    void Initialize( const T_PointVector& points );
    void Cleanup   ( MIL_MOSContextID nContext );
    void Cleanup   ();
    //@}

    //! @name Network
    //@{
    void SendFullState() const;
    bool Update       ( const ASN1T_MsgLimitUpdate& asnMsg, MIL_MOSContextID nContext );
    //@}
    
    //! @name Accessors
    //@{
          uint           GetID    () const;
    const T_PointVector& GetPoints() const;
          MT_Float       GetLength() const;
    //@}

    //! @name Operations
    //@{
    void RegisterFuseau( const MIL_Fuseau& fuseau ) const;
    void RemoveFuseau  ( const MIL_Fuseau& fuseau ) const;

    MT_Float Distance( const MT_Vector2D& p ) const;
    //@}

private:
    //! @name Tools
    //@{
    void ReadPoints            ( const ASN1T__SeqOfCoordUTM& listPoint );
    void InitializeDistanceData();

    void Destroy();

    void RegisterLimit( const MIL_Limit& other, const MIL_Fuseau& fuseau ) const;
    void RemoveLimit( const MIL_Limit& other ) const;
    bool IncreaseReference( const MIL_Limit& other ) const;
    bool DecreaseReference( const MIL_Limit& other ) const;
    void CreateDynamicData( const MIL_Limit& other, const MIL_Fuseau& fuseau ) const;
    TER_DynamicData* CreateDynamicData( const T_PointVector& points ) const;
    MT_Float SquareDistance( const MT_Vector2D& p ) const;
    void CreateDistanceDatas();
    //@}

    //! @name Types
    //@{
    typedef std::pair< TER_DynamicData*, uint >            T_ReferencedData;
    typedef std::map< const MIL_Limit*, T_ReferencedData > T_DynamicDatas;
    typedef T_DynamicDatas::iterator                      IT_DynamicDatas;

    class DistanceData
    {
    public:
        DistanceData( const MT_Vector2D& from, const MT_Vector2D& to );
        MT_Float SquareDistance( const MT_Vector2D& p ) const;
    private:
        MT_Vector2D origin_;
        MT_Vector2D direction_;
        MT_Float    rSquareLength_;
    };
    typedef std::vector< DistanceData >       T_DistanceDatas;
    typedef T_DistanceDatas::const_iterator CIT_DistanceDatas;
    //@}

private:
    uint nID_;
    uint nLevel_;
       
	T_PointVector   points_;
    T_DistanceDatas distanceDatas_;

    mutable T_DynamicDatas fuseauDatas_;
    bool    bDestroyed_;
};


#   include "MIL_Limit.inl"
#endif // __MIL_Limit_h_
