//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_ZoneMineeLineaire.h $
// $Author: Jvt $
// $Modtime: 14/04/05 11:29 $
// $Revision: 6 $
// $Workfile: MIL_ZoneMineeLineaire.h $
//
//*****************************************************************************

#ifndef __MIL_ZoneMineeLineaire_h_
#define __MIL_ZoneMineeLineaire_h_

#include "MIL.h"

#include "MIL_Obstacle.h"

//=============================================================================
// Created: NLD 2002-12-12
// Last modified: JVT 03-07-31
//=============================================================================
class MIL_ZoneMineeLineaire : public MIL_Obstacle
{
    MT_COPYNOTALLOWED( MIL_ZoneMineeLineaire );

public:
             MIL_ZoneMineeLineaire( const MIL_RealObjectType& type, uint nID, MIL_Army& army );
             MIL_ZoneMineeLineaire();
    virtual ~MIL_ZoneMineeLineaire();
    
    //! @name CheckPoints
    //@{
    template < typename Archive > void serialize( Archive&, const uint );
    //@}

    //! @name Init
    //@{
    static MIL_RealObject_ABC& Create( const MIL_RealObjectType& type, uint nID, MIL_Army& army );

    virtual bool                      Initialize( const MIL_ObstacleType& obstacleType, DIA_Parameters& diaParameters, uint& nCurrentParamIdx );
    virtual void                      Initialize( MIL_InputArchive& archive );
    virtual ASN1T_EnumObjectErrorCode Initialize( const ASN1T_MagicActionCreateObject& asn );
    //@}

    //! @name Knowledge
    //@{
    virtual DEC_Knowledge_Object& CreateKnowledge( const MIL_Army& teamKnowing );
    //@}

    //! @name Update
    //@{
    virtual void UpdateState();
    //@}

    //! @name Accessors
    //@{
    uint     GetMinesActivityTime() const;
    MT_Float GetMinesDensity     () const;
    //@}

private:
    //! @name Events
    //@{
    virtual void ProcessAgentMovingInside( MIL_Agent_ABC& agent );
    //@}

    //! @name Network
    //@{
    virtual void WriteSpecificAttributes( MT_XXmlOutputArchive& archive ) const;
    virtual void WriteSpecificAttributes( NET_ASN_MsgObjectCreation& asnMsg );
    //@}

    //$$$ HLA

private:
    // Specific attributes
    ASN1T_ObjectAttributesLinearMineArea asnAttributes_;

    MT_Float rMinesDensity_;
    uint     nMinesActivityTime_;
    uint     nDeathTimeStep_;
};

#include "MIL_ZoneMineeLineaire.inl"

#endif // __MIL_ZoneMineeLineaire_h_
