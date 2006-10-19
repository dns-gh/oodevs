//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_NuageNBC.h $
// $Author: Jvt $
// $Modtime: 30/03/05 11:15 $
// $Revision: 6 $
// $Workfile: MIL_NuageNBC.h $
//
//*****************************************************************************

#ifndef __MIL_NuageNBC_h_
#define __MIL_NuageNBC_h_

#include "MIL.h"

#include "MIL_RealObject_ABC.h"

class MIL_NbcAgentType;

//=============================================================================
// Created: NLD 2002-12-12
// Last modified: JVT 03-07-31
//=============================================================================
class MIL_NuageNBC : public MIL_RealObject_ABC
{
    MT_COPYNOTALLOWED( MIL_NuageNBC );

public:
             MIL_NuageNBC();
    virtual ~MIL_NuageNBC();

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;
    //@}

    //! @name Init
    //@{
    static MIL_RealObject_ABC& Create();

            void                      Initialize( const MIL_Army& army, const TER_Localisation& localisation, const MIL_NbcAgentType& nbcAgent );
    virtual bool                      Initialize( const MIL_Army& army, DIA_Parameters& diaParameters, uint& nCurrentParamIdx );
    virtual void                      Initialize( uint nID, MIL_InputArchive& archive );
    virtual ASN1T_EnumObjectErrorCode Initialize( uint nID, const ASN1T_MagicActionCreateObject& asnCreateObject );
    //@}

    //! @name Update
    //@{
    virtual void UpdateState();
    //@}
            
    //! @name Accessors
    //@{
    const MIL_NbcAgentType& GetNbcAgentType() const;
    //@}

    //! @name Knowledge
    //@{
    virtual DEC_Knowledge_Object& CreateKnowledge( const MIL_Army& teamKnowing );
    //@}

    //! @name HLA
    //@{
    virtual bool Initialize( const std::string& strOption, const std::string& strExtra, double rCompletion, double rMining, double rBypass );
    virtual void Deserialize( const AttributeIdentifier& attributeID, Deserializer deserializer );
    virtual void Serialize( HLA_UpdateFunctor& functor ) const;
    //@}

private:
    //! @name Events
    //@{
    virtual void ProcessAgentInside  ( MIL_Agent_ABC& agent );
    virtual void ProcessAgentEntering( MIL_Agent_ABC& agent );
    //@}

    //! @name Tools
    //@{
    bool UpdateShape( TER_Localisation& newLocalisation );
    //@}
                
    //! @name Network
    //@{
    virtual void WriteSpecificAttributes( MT_XXmlOutputArchive& archive ) const;
    virtual void WriteSpecificAttributes( NET_ASN_MsgObjectCreation& asnMsg );
    //@}

private:
    const MIL_NbcAgentType* pNbcAgentType_;

    // Specific attributes
    ASN1T_AttrObjectNuageNBC asnAttributes_;
    
    uint nDeathTimeStep_;

    // shape
    MT_Vector2D vOrigin_;
    MT_Float    rCurrentCircleRadius_;
    MT_Float    rCurrentPropagationLenght_;
};

#include "MIL_NuageNBC.inl"

#endif // __MIL_NuageNBC_h_
