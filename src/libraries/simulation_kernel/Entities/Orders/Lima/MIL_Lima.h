//*****************************************************************************
//
// $Created: NLD 2002-08-06 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Orders/Lima/MIL_Lima.h $
// $Author: Nld $
// $Modtime: 19/01/05 16:36 $
// $Revision: 4 $
// $Workfile: MIL_Lima.h $
//
//*****************************************************************************

#ifndef __MIL_Lima_h_
#define __MIL_Lima_h_

#include "MIL.h"

#include "Network/NET_ASN_Types.h"

class MIL_Agent_ABC;
class NET_AS_MOSServer;
class MIL_LimaFunction_ABC;

//=============================================================================
// Created: NLD 2002-08-06
// Last modified: JVT 03-02-27
//=============================================================================
class MIL_Lima
{
    MT_COPYNOTALLOWED( MIL_Lima );

public:
    //! @name Types
    //@{
    enum E_LimaFunctions
    {
        eLimaFuncLD     = 0, // Ligne de débouché
        eLimaFuncLCA    = 1, // Ligne de changement d'attitude
        eLimaFuncLC     = 2, // Ligne de coordination
        eLimaFuncLI     = 3, // Ligne à interdire
        eLimaFuncLO     = 4, // Ligne d'objectif
        eLimaFuncLCAR   = 5, // Ligne de coup d'arrêt
        eLimaFuncLR     = 6, // Ligne de recueil
        eLimaFuncLDM    = 7, // Ligne de début de mission
        eLimaFuncLFM    = 8, // Ligne de fin de mission      
        eLimaFuncLIA    = 9, // Ligne d'identification et de recueil
        eLimaFuncNone
    };
    //@}

public:
     MIL_Lima();
    ~MIL_Lima();

    //! @name Init / Cleanup
    //@{
    bool Initialize( const ASN1T_MsgLimaCreation& asnMsg, MIL_MOSContextID nCtx );
    void Cleanup   ( MIL_MOSContextID nContext );
    void Cleanup   ();
    //@}

    //! @name Main 
    //@{
    bool Intersect2D( const MT_Line& line, MT_Vector2D& vPos ) const;
    bool Intersect2D( const T_PointVector& polyline, T_PointSet& intersectionSet ) const; //$$$ devrait gicler
    //@}

    //! @name Network
    //@{
    void SendFullState() const;
    bool Update       ( const ASN1T_MsgLimaUpdate& asnMsg, MIL_MOSContextID nContext );
    //@}
    
    //! @name Accessors
    //@{
          uint              GetID          () const;
	const NET_AS_MOSServer*	GetAckReceiver () const;
    const T_PointVector&    GetPointVector () const;
          E_LimaFunctions   GetLimaFunction() const;
    //@}

private:
    //! @name Tools
    //@{
    void ReadPoints( const ASN1T__SeqOfCoordUTM& listPoint );
    //@}

private:
    uint            nID_;
    E_LimaFunctions nLimaFunction_;
       
	T_PointVector pointVector_; //$$$ devrait être MT_PolyLine
};

#include "MIL_Lima.inl"

#endif // __MIL_Lima_h_
