// *****************************************************************************
//
// $Created: NLD 2003-08-05 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_DynaObject_Creator.h $
// $Author: Nld $
// $Modtime: 5/04/05 13:16 $
// $Revision: 5 $
// $Workfile: MOS_DynaObject_Creator.h $
//
// *****************************************************************************

#ifndef __MOS_DynaObject_Creator_h_
#define __MOS_DynaObject_Creator_h_

#include "MOS_Types.h"

#include <qframe.h>

class QComboBox;
class MOS_DynaObject;
class MOS_MTH_Localisation;
class MOS_IDManager;

// =============================================================================
// Created: NLD 2003-08-05
// =============================================================================
class MOS_DynaObject_Creator : public QFrame
{
    Q_OBJECT
    MT_COPYNOTALLOWED( MOS_DynaObject_Creator )

public:
     MOS_DynaObject_Creator( QWidget* pParent ); 
    ~MOS_DynaObject_Creator(); 

    void Initialize();

private slots:

    //! @name Slots
    //@{
    void SlotValidate();
    void SlotCancel  ();
    //@}


private:
    const MOS_DynaObject* pObject_;
    
    QComboBox* pTypeComboBox_;
    QComboBox* pTeamComboBox_;

    MOS_MTH_Localisation* pLocalisation_;


public:
    static MOS_IDManager* idManagerFosseAntiChar_;          
    static MOS_IDManager* idManagerAbattis_;                 
    static MOS_IDManager* idManagerBarricade_;              
    static MOS_IDManager* idManagerBouchonMines_;           
    static MOS_IDManager* idManagerZoneMineeLineaire_;      
    static MOS_IDManager* idManagerZoneMineeParDispersion_; 
    static MOS_IDManager* idManagerEboulement_;             
    static MOS_IDManager* idManagerDestructionRoute_;       
    static MOS_IDManager* idManagerDestructionPont_;        
    static MOS_IDManager* idManagerPontFlottantContinu_;           
    static MOS_IDManager* idManagerPontFlottantDiscontinu_;           
    static MOS_IDManager* idManagerPosteTir_;               
    static MOS_IDManager* idManagerZoneProtegee_;           
    static MOS_IDManager* idManagerZoneImplantationCanon_;  
    static MOS_IDManager* idManagerZoneImplantationCOBRA_;  
    static MOS_IDManager* idManagerZoneImplantationLRM_;    
    static MOS_IDManager* idManagerSiteFranchissement_;     
    static MOS_IDManager* idManagerNuageNBC_;               
    static MOS_IDManager* idManagerSiteDecontamination_;    
    static MOS_IDManager* idManagerPlotRavitaillement_;  
    static MOS_IDManager* idManagerZoneBrouillageBrod_;
    static MOS_IDManager* idManagerZoneBrouillageBromure_;
    static MOS_IDManager* idManagerRota_;    
    static MOS_IDManager* idManagerZoneNBC_;
    static MOS_IDManager* idManagerAirePoser_;
    static MOS_IDManager* idManagerPiste_;
    static MOS_IDManager* idManagerPlateForme_;
    static MOS_IDManager* idManagerZoneMobiliteAmelioree_;
    static MOS_IDManager* idManagerZonePoserHelicoptere_;
    static MOS_IDManager* idManagerAireLogistique_;
    static MOS_IDManager* idManagerCampPrisonniers_;
    static MOS_IDManager* idManagerCampRefugies_;
    static MOS_IDManager* idManagerItineraireLogistique_;
    static MOS_IDManager* idManagerPosteControle_;
    static MOS_IDManager* idManagerTerrainLargage_;
    static MOS_IDManager* idManagerZoneForbiddenFire_;
    static MOS_IDManager* idManagerZoneForbiddenMove_;
    static MOS_IDManager* idManagerZoneImplantationMortier_;

};

#ifdef MOS_USE_INLINE
#   include "MOS_DynaObject_Creator.inl"
#endif

#endif // __MOS_DynaObject_Creator_h_
