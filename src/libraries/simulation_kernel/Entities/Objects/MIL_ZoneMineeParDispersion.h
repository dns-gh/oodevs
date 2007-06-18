//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_ZoneMineeParDispersion.h $
// $Author: Jvt $
// $Modtime: 14/04/05 11:30 $
// $Revision: 6 $
// $Workfile: MIL_ZoneMineeParDispersion.h $
//
//*****************************************************************************

#ifndef __MIL_ZoneMineeParDispersion_h_
#define __MIL_ZoneMineeParDispersion_h_

#include "MIL.h"

#include "MIL_Obstacle.h"

//=============================================================================
// Created: NLD 2002-12-12
// Last modified: JVT 03-07-31
//=============================================================================
class MIL_ZoneMineeParDispersion : public MIL_Obstacle
{
    MT_COPYNOTALLOWED( MIL_ZoneMineeParDispersion );

public:
             MIL_ZoneMineeParDispersion( const MIL_RealObjectType& type, uint nID, MIL_Army& army );
             MIL_ZoneMineeParDispersion();
    virtual ~MIL_ZoneMineeParDispersion();

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
    virtual void                      Initialize( const TER_Localisation& localisation, uint nNbrMines );
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
    ASN1T_ObjectAttributesDispersedMineArea asnAttributes_;

    MT_Float rMinesDensity_;
    uint     nMinesActivityTime_;
    uint     nDeathTimeStep_;
};

#include "MIL_ZoneMineeParDispersion.inl"

#endif // __MIL_ZoneMineeParDispersion_h_
