//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_BouchonMines.h $
// $Author: Jvt $
// $Modtime: 14/04/05 11:10 $
// $Revision: 6 $
// $Workfile: MIL_BouchonMines.h $
//
//*****************************************************************************

#ifndef __MIL_BouchonMines_h_
#define __MIL_BouchonMines_h_

#include "MIL.h"

#include "MIL_Obstacle.h"

class MIL_Agent_ABC;

//=============================================================================
// Created: NLD 2002-12-12
// Last modified: JVT 03-07-31
//=============================================================================
class MIL_BouchonMines : public MIL_Obstacle
{
    MT_COPYNOTALLOWED( MIL_BouchonMines );

public:
             MIL_BouchonMines( const MIL_RealObjectType& type, uint nID, MIL_Army& army );
             MIL_BouchonMines();
    virtual ~MIL_BouchonMines();
 
    //! @name CheckPoints
    //@{
    template < typename Archive > void serialize( Archive&, const uint );
    //@}

    //! @name Init
    //@{
    static MIL_RealObject_ABC& Create( const MIL_RealObjectType& type, uint nID, MIL_Army& army );
    
    virtual bool                      Initialize( DIA_Parameters& diaParameters, uint& nCurrentParamIdx );
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
    uint GetMinesActivityTime() const;
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
    ASN1T_AttrObjectBouchonMines asnAttributes_;

    uint nMinesActivityTime_;
    uint nDeathTimeStep_;
};

#include "MIL_BouchonMines.inl"

#endif // __MIL_BouchonMines_h_
