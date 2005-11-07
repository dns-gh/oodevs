//*****************************************************************************
//
// $Created: AGN 03-04-03 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_PopulationConcentrationKnowledge.h $
// $Author: Age $
// $Modtime: 31/03/05 17:36 $
// $Revision: 5 $
// $Workfile: MOS_PopulationConcentrationKnowledge.h $
//
//*****************************************************************************

#ifndef __MOS_PopulationConcentrationKnowledge_h_
#define __MOS_PopulationConcentrationKnowledge_h_

#include "MOS_ASN_Types.h"
#include "MOS_IDManager.h"

class MOS_Gtia;
class MOS_PopulationConcentration;
class MOS_PopulationKnowledge;

// =============================================================================
// Created: APE 2004-03-10
// =============================================================================
class MOS_PopulationConcentrationKnowledge
{
    MT_COPYNOTALLOWED( MOS_PopulationConcentrationKnowledge );
        
public:
    //! @name Constructor/Destructor
    //@{
     MOS_PopulationConcentrationKnowledge( const ASN1T_MsgPopulationConcentrationKnowledgeCreation& asnMsg );
    ~MOS_PopulationConcentrationKnowledge();
    //@}

    //! @name Accessors
    //@{
    uint                               GetID                 () const;
    const MOS_Gtia&                    GetGtia               () const;
    const MOS_PopulationKnowledge*     GetPopulationKnowledge() const;
    const MOS_PopulationConcentration* GetConcentration      () const;
    const MT_Vector2D&                 GetPosition           () const;
    uint                               GetNbrAliveHumans     () const;
    uint                               GetNbrDeadHumans      () const;
    E_PopulationAttitude               GetAttitude           () const;
    MT_Float                           GetRelevance          () const;
    bool                               IsPerceived           () const;
    MT_Float                           GetArea               () const;
    //@}

    //! @name Validity Accessors
    //@{
    bool IsValidNbrAliveHumans() const;
    bool IsValidNbrDeadHumans () const;
    bool IsValidAttitude      () const;
    bool IsValidPerceived     () const;
    //@}

    //! @name Network
    //@{
    void Update( const ASN1T_MsgPopulationConcentrationKnowledgeUpdate& asnMsg );
    //@}

private:
    //! @name Types
    //@{
    template< typename T >
    struct sValidableData
    {
        template< typename T >
        sValidableData( T val, bool bIsValid )
            : value_    ( val )
            , bIsValid_ ( bIsValid )
        {
        }

        template< typename T >
        sValidableData< typename T >& operator=( const T& val )
        {
            value_    = val;
            bIsValid_ = true;
            return *this;
        }

        bool bIsValid_;
        T    value_;
    };
    //@}

private:
    const uint                                   nID_;
          MOS_Gtia*                              pGtia_;
    const MOS_PopulationKnowledge*               pPopulationKnowledge_;
    const MOS_PopulationConcentration*           pConcentration_;
          MT_Vector2D                            position_;
          sValidableData< uint >                 nNbrAliveHumans_;
          sValidableData< uint >                 nNbrDeadHumans_;
          sValidableData< E_PopulationAttitude > eAttitude_;
          sValidableData< bool >                 bIsPerceived_;
          MT_Float                               rRelevance_;
};


#include "MOS_PopulationConcentrationKnowledge.inl"

#endif // __MOS_PopulationConcentrationKnowledge_h_