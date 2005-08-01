//*****************************************************************************
//
// $Created: AGN 03-04-14 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_Mission_OC.h $
// $Author: Nld $
// $Modtime: 9/05/05 12:07 $
// $Revision: 25 $
// $Workfile: MOS_Mission_OC.h $
//
//*****************************************************************************

#ifndef __MOS_Mission_OC_h_
#define __MOS_Mission_OC_h_

#include "MOS_Types.h"

#include "MOS_Mission_ABC.h"

class MOS_ASN_MsgOrderConduite;

//*****************************************************************************
// Created: AGN 03-04-14
//*****************************************************************************
class MOS_Mission_OC
: public MOS_Mission_ABC
{
    MT_COPYNOTALLOWED( MOS_Mission_OC )

public:
     MOS_Mission_OC( E_OrderConduiteID nTypeMission, QWidget* pParent = 0 );
    ~MOS_Mission_OC();

    //-------------------------------------------------------------------------
    /** @name Init */
    //-------------------------------------------------------------------------
    //@{
    virtual void Initialize();
    virtual void Terminate ();

    virtual void SetAgent( MOS_Agent* pAgent );
    virtual std::string  GetName       () const;
    virtual bool IsAutomateMission() const;
    //@}

private:
    //-------------------------------------------------------------------------
    /** @name Network */
    //-------------------------------------------------------------------------
    //@{
    virtual void SendMsg( std::string& sParam );
    virtual void FillRandomParameters();
    virtual void ReloadParameters( MT_InputArchive_ABC& archive );
    virtual void ResaveParameters( MT_OutputArchive_ABC& archive );
    //@}
    
    //-------------------------------------------------------------------------
    /** @name */
    //-------------------------------------------------------------------------
    //@{
    void InitializeChangeReglesEngagement               ();
    void InitializePionAppliquerFeux                    ();
    void InitializePionRenforcerEnVSRAM                 (); 
    void InitializePionTransfererVSRAM                  (); 
    void InitializePionReprendreAuxOrdresVSRAM          ();
    void InitializeModifierTempsBordeeMaintenance       (); 
    void InitializeModifierTempsBordeeSante             (); 
    void InitializeModifierPrioritesReparations         (); 
    void InitializeModifierPrioritesBlesses             (); 
    void InitializeModifierPrioritesTactiquesBlesses    ();
    void InitializeModifierPrioritesTactiquesReparations();
    void InitializePionChangerDePosition                ();
    void InitializeChangerPositionDebarquement          ();
    void InitializeAcquerirObjectif                     ();
    void InitializeAutomateRealiserVariantement         (); 
    void InitializeAutomateDesesquiperSiteFranchissement(); 
    void InitializeAutomateReagirFaceAEni               (); 
    void InitializeAutomateAffecterPionAObstacle        (); 
    void InitializeAutomateReconnaitreZoneImplantation  ();
    void InitializeAutomateTC2GererMaterielAvantDeplacement();
    void InitializeAutomateActiverObstacle();   
    void InitializeAutomateTransfererRenforts();
    //@}

private:
    MOS_ASN_MsgOrderConduite*   pASNMsgOrder_;
    E_OrderConduiteID nTypeOC_;
};

#ifdef MOS_USE_INLINE
#	include "MOS_Mission_OC.inl"
#endif


#endif // __MOS_Mission_OC_h_