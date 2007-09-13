// *****************************************************************************
//
// $Created: NLD 2004-10-20 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/decision/DEC_Tools.inl $
// $Author: Jvt $
// $Modtime: 11/05/05 17:06 $
// $Revision: 18 $
// $Workfile: DEC_Tools.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: DEC_Tools::CheckTypeObjectif
// Created: NLD 2007-05-14
// -----------------------------------------------------------------------------
inline
bool DEC_Tools::CheckTypeObjectif( const DIA_Variable_ABC& diaVariable )
{
    assert( pTypeObjectif_ );
    return diaVariable.GetType() == *pTypeObjectif_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Tools::CheckTypeFuseau
// Created: NLD 2007-04-05
// -----------------------------------------------------------------------------
inline
bool DEC_Tools::CheckTypeFuseau( const DIA_Variable_ABC& diaVariable )
{
    assert( pTypeFuseau_ );
    return diaVariable.GetType() == *pTypeFuseau_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Tools::CheckTypeEquipement
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
inline
bool DEC_Tools::CheckTypeEquipement( const DIA_Variable_ABC& diaVariable )
{
    assert( pTypeEquipement_ );
    return diaVariable.GetType() == *pTypeEquipement_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Tools::CheckTypeTirIndirect
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
inline
bool DEC_Tools::CheckTypeTirIndirect( const DIA_Variable_ABC& diaVariable )
{
    assert( pTypeTirIndirect_ );
    return diaVariable.GetType() == *pTypeTirIndirect_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Tools::CheckTypeDotation
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
inline
bool DEC_Tools::CheckTypeDotation( const DIA_Variable_ABC& diaVariable )
{
    assert( pTypeDotation_ );
    return diaVariable.GetType() == *pTypeDotation_;
}


// -----------------------------------------------------------------------------
// Name: DEC_Tools::CheckTypeMaintenancePriorities
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
inline
bool DEC_Tools::CheckTypeMaintenancePriorities( const DIA_Variable_ABC& diaVariable )
{
    assert( pTypeMaintenancePriorities_ );
    return diaVariable.GetType() == *pTypeMaintenancePriorities_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Tools::CheckTypeSantePriorites
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
inline
bool DEC_Tools::CheckTypeSantePriorites( const DIA_Variable_ABC& diaVariable )
{
    assert( pTypeSantePriorites_ );
    return diaVariable.GetType() == *pTypeSantePriorites_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Tools::TypePerceptionObjectsLocalisation
// Created: JVT 2005-01-19
// -----------------------------------------------------------------------------
inline
bool DEC_Tools::CheckTypePerceptionObjectsLocalisation( const DIA_Variable_ABC& diaVariable )
{
    assert( pTypePerceptionObjectsLocalisation_ );
    return diaVariable.GetType() == *pTypePerceptionObjectsLocalisation_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Tools::CheckTypePerceptionPoint
// Created: NLD 2004-10-21
// -----------------------------------------------------------------------------
inline
bool DEC_Tools::CheckTypePerceptionPoint( const DIA_Variable_ABC& diaVariable )
{
    assert( pTypePerceptionPoint_ );
    return diaVariable.GetType() == *pTypePerceptionPoint_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Tools::CheckTypePerceptionLocalisation
// Created: JVT 2004-10-22
// -----------------------------------------------------------------------------
inline
bool DEC_Tools::CheckTypePerceptionLocalisation( const DIA_Variable_ABC& diaVariable )
{
    assert( pTypePerceptionLocalisation_ );
    return diaVariable.GetType() == *pTypePerceptionLocalisation_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Tools::CheckTypePerceptionRadar
// Created: JVT 2004-10-22
// -----------------------------------------------------------------------------
inline
bool DEC_Tools::CheckTypePerceptionRadar( const DIA_Variable_ABC& diaVariable )
{
    assert( pTypePerceptionRadar_ );
    return diaVariable.GetType() == *pTypePerceptionRadar_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Tools::CheckTypePerceptionSurveillance
// Created: NLD 2004-11-17
// -----------------------------------------------------------------------------
inline
bool DEC_Tools::CheckTypePerceptionSurveillance( const DIA_Variable_ABC& diaVariable )
{
    assert( pTypePerceptionSurveillance_ );
    return diaVariable.GetType() == *pTypePerceptionSurveillance_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Tools::CheckTypePerceptionFlyingShell
// Created: NLD 2005-02-21
// -----------------------------------------------------------------------------
inline
bool DEC_Tools::CheckTypePerceptionFlyingShell( const DIA_Variable_ABC& diaVariable )
{
    assert( pTypePerceptionFlyingShell_ );
    return diaVariable.GetType() == *pTypePerceptionFlyingShell_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Tools::CheckTypePoint
// Created: NLD 2004-10-20
// -----------------------------------------------------------------------------
inline
bool DEC_Tools::CheckTypePoint( const DIA_Variable_ABC& diaVariable )
{
    assert( pTypePoint_ );
    return diaVariable.GetType() == *pTypePoint_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Tools::CheckTypeDirection
// Created: NLD 2004-10-20
// -----------------------------------------------------------------------------
inline
bool DEC_Tools::CheckTypeDirection( const DIA_Variable_ABC& diaVariable )
{
    assert( pTypeDirection_ );
    return diaVariable.GetType() == *pTypeDirection_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Tools::CheckTypeItineraire
// Created: NLD 2004-10-20
// -----------------------------------------------------------------------------
inline
bool DEC_Tools::CheckTypeItineraire( const DIA_Variable_ABC& diaVariable )
{
    assert( pTypeItineraire_ );
    return diaVariable.GetType() == *pTypeItineraire_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Tools::CheckTypeListePoints
// Created: NLD 2004-10-20
// -----------------------------------------------------------------------------
inline
bool DEC_Tools::CheckTypeListePoints( const DIA_Variable_ABC& diaVariable )
{
    assert( pTypeListePoints_ );
    return diaVariable.GetType() == *pTypeListePoints_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Tools::CheckTypeLocalisation
// Created: NLD 2004-10-20
// -----------------------------------------------------------------------------
inline
bool DEC_Tools::CheckTypeLocalisation( const DIA_Variable_ABC& diaVariable )
{
    assert( pTypeLocalisation_ );
    return diaVariable.GetType() == *pTypeLocalisation_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Tools::CheckTypeCalculLignesAvantArriere
// Created: NLD 2004-10-20
// -----------------------------------------------------------------------------
inline
bool DEC_Tools::CheckTypeCalculLignesAvantArriere( const DIA_Variable_ABC& diaVariable )
{
    assert( pTypeCalculLignesAvantArriere_ );
    return diaVariable.GetType() == *pTypeCalculLignesAvantArriere_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Tools::CheckTypeLima
// Created: NLD 2004-10-20
// -----------------------------------------------------------------------------
inline
bool DEC_Tools::CheckTypeLima( const DIA_Variable_ABC& diaVariable )
{
    assert( pTypeLima_ );
    return diaVariable.GetType() == *pTypeLima_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Tools::CheckTypeGenObjet
// Created: NLD 2004-10-20
// -----------------------------------------------------------------------------
inline
bool DEC_Tools::CheckTypeGenObjet( const DIA_Variable_ABC& diaVariable )
{
    assert( pTypeGenObjet_ );
    return diaVariable.GetType() == *pTypeGenObjet_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Tools::CheckTypeConnaissanceObjet
// Created: NLD 2004-10-20
// -----------------------------------------------------------------------------
inline
bool DEC_Tools::CheckTypeConnaissanceObjet( const DIA_Variable_ABC& diaVariable )
{
    assert( pTypeConnaissanceObjet_ );
    return diaVariable.GetType() == *pTypeConnaissanceObjet_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Tools::CheckTypeConnaissancePopulation
// Created: NLD 2004-10-20
// -----------------------------------------------------------------------------
inline
bool DEC_Tools::CheckTypeConnaissancePopulation( const DIA_Variable_ABC& diaVariable )
{
    assert( pTypeConnaissancePopulation_ );
    return diaVariable.GetType() == *pTypeConnaissancePopulation_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Tools::CheckTypeConnaissanceAgent
// Created: NLD 2004-10-20
// -----------------------------------------------------------------------------
inline
bool DEC_Tools::CheckTypeConnaissanceAgent( const DIA_Variable_ABC& diaVariable )
{
    assert( pTypeConnaissanceAgent_ );
    return diaVariable.GetType() == *pTypeConnaissanceAgent_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Tools::CheckTypePopulationConnaissanceAgent
// Created: NLD 2005-11-10
// -----------------------------------------------------------------------------
inline
bool DEC_Tools::CheckTypePopulationConnaissanceAgent( const DIA_Variable_ABC& diaVariable )
{
    assert( pTypePopulationConnaissanceAgent_ );
    return diaVariable.GetType() == *pTypePopulationConnaissanceAgent_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Tools::CheckTypePopulationConnaissanceObjet
// Created: NLD 2005-12-05
// -----------------------------------------------------------------------------
inline
bool DEC_Tools::CheckTypePopulationConnaissanceObjet( const DIA_Variable_ABC& diaVariable )
{
    assert( pTypePopulationConnaissanceObjet_ );
    return diaVariable.GetType() == *pTypePopulationConnaissanceObjet_;
}
    
// -----------------------------------------------------------------------------
// Name: DEC_Tools::CheckTypeAction
// Created: NLD 2004-10-20
// -----------------------------------------------------------------------------
inline
bool DEC_Tools::CheckTypeAction( const DIA_Variable_ABC& diaVariable )
{
    assert( pTypeAction_ );
    return diaVariable.GetType() == *pTypeAction_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Tools::CheckTypePion
// Created: NLD 2004-10-20
// -----------------------------------------------------------------------------
inline
bool DEC_Tools::CheckTypePion( const DIA_Variable_ABC& diaVariable )
{
    assert( pTypePion_ );
    if( diaVariable.Type() != eObject )
        return false;
    DIA_TypedObject* pObject = diaVariable.ToObject();
    if( !pObject ) 
        return diaVariable.GetType().IsKindOf( *pTypePion_ );
    return pObject->GetType().IsKindOf( *pTypePion_ );
}

// -----------------------------------------------------------------------------
// Name: DEC_Tools::CheckTypePion
// Created: NLD 2004-10-21
// -----------------------------------------------------------------------------
inline
bool DEC_Tools::CheckTypePion( const DIA_TypedObject& object )
{
    assert( pTypePion_ );
    return object.GetType().IsKindOf( *pTypePion_ );
}

// -----------------------------------------------------------------------------
// Name: DEC_Tools::CheckTypeAutomate
// Created: NLD 2004-10-20
// -----------------------------------------------------------------------------
inline
bool DEC_Tools::CheckTypeAutomate( const DIA_Variable_ABC& diaVariable )
{
    assert( pTypeAutomate_ );
    if( diaVariable.Type() != eObject )
        return false;
    DIA_TypedObject* pObject = diaVariable.ToObject();
    if( !pObject ) 
        return diaVariable.GetType().IsKindOf( *pTypeAutomate_ );
    return pObject->GetType().IsKindOf( *pTypeAutomate_ );
}

// -----------------------------------------------------------------------------
// Name: DEC_Tools::CheckTypeAutomate
// Created: NLD 2004-10-21
// -----------------------------------------------------------------------------
inline
bool DEC_Tools::CheckTypeAutomate( const DIA_TypedObject& object )
{
    assert( pTypeAutomate_ );
    return object.GetType().IsKindOf( *pTypeAutomate_ );
}

// -----------------------------------------------------------------------------
// Name: DEC_Tools::CheckTypeMissionPion
// Created: NLD 2004-10-20
// -----------------------------------------------------------------------------
inline
bool DEC_Tools::CheckTypeMissionPion( const DIA_Variable_ABC& diaVariable )
{
    assert( pTypeMissionPion_ );
    if( diaVariable.Type() != eObject )
        return false;
    DIA_TypedObject* pObject = diaVariable.ToObject();
    if( !pObject ) 
        return diaVariable.GetType().IsKindOf( *pTypeMissionPion_ );
    return pObject->GetType().IsKindOf( *pTypeMissionPion_ );
}

// -----------------------------------------------------------------------------
// Name: DEC_Tools::CheckTypeMissionAutomate
// Created: NLD 2004-10-20
// -----------------------------------------------------------------------------
inline
bool DEC_Tools::CheckTypeMissionAutomate( const DIA_Variable_ABC& diaVariable )
{
    assert( pTypeMissionAutomate_ );
    if( diaVariable.Type() != eObject )
        return false;
    DIA_TypedObject* pObject = diaVariable.ToObject();
    if( !pObject ) 
        return diaVariable.GetType().IsKindOf( *pTypeMissionAutomate_ );
    return pObject->GetType().IsKindOf( *pTypeMissionAutomate_ );
}

// -----------------------------------------------------------------------------
// Name: DEC_Tools
// Created: NLD 2004-10-21
// -----------------------------------------------------------------------------
inline
const DIA_TypeDef& DEC_Tools::GetTypeLocalisation()
{
    assert( pTypeLocalisation_ );
    return *pTypeLocalisation_;
}
// -----------------------------------------------------------------------------
// Name: DEC_Tools::GetTypeConnaissanceAgent
// Created: NLD 2004-10-21
// -----------------------------------------------------------------------------
inline
const DIA_TypeDef& DEC_Tools::GetTypeConnaissanceAgent()
{
    assert( pTypeConnaissanceAgent_ );
    return *pTypeConnaissanceAgent_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Tools::GetTypeConnaissanceObjet
// Created: NLD 2004-10-21
// -----------------------------------------------------------------------------
inline
const DIA_TypeDef& DEC_Tools::GetTypeConnaissanceObjet()
{
    assert( pTypeConnaissanceObjet_ );
    return *pTypeConnaissanceObjet_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Tools::GetTypeListePoints
// Created: NLD 2004-10-21
// -----------------------------------------------------------------------------
inline
const DIA_TypeDef& DEC_Tools::GetTypeListePoints()
{
    assert( pTypeListePoints_ );
    return *pTypeListePoints_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Tools::GetTypeItineraire
// Created: NLD 2004-10-21
// -----------------------------------------------------------------------------
inline
const DIA_TypeDef& DEC_Tools::GetTypeItineraire()
{
    assert( pTypeItineraire_ );
    return *pTypeItineraire_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Tools::GetTypeAction
// Created: NLD 2004-10-21
// -----------------------------------------------------------------------------
inline
const DIA_TypeDef& DEC_Tools::GetTypeAction()
{
    assert( pTypeAction_ );
    return *pTypeAction_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Tools::GetTypeLima
// Created: NLD 2004-10-21
// -----------------------------------------------------------------------------
inline
const DIA_TypeDef& DEC_Tools::GetTypeLima()
{
    assert( pTypeLima_ );
    return *pTypeLima_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Tools::GetTypePoint
// Created: NLD 2004-10-21
// -----------------------------------------------------------------------------
inline
const DIA_TypeDef& DEC_Tools::GetTypePoint()
{
    assert( pTypePoint_ );
    return *pTypePoint_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Tools::GetTypeDirection
// Created: NLD 2004-10-21
// -----------------------------------------------------------------------------
inline
const DIA_TypeDef& DEC_Tools::GetTypeDirection()
{
    assert( pTypeDirection_ );
    return *pTypeDirection_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Tools::GetTypeCalculLignesAvantArriere
// Created: NLD 2004-10-21
// -----------------------------------------------------------------------------
inline
const DIA_TypeDef& DEC_Tools::GetTypeCalculLignesAvantArriere()
{
    assert( pTypeCalculLignesAvantArriere_ );
    return *pTypeCalculLignesAvantArriere_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Tools::GetTypePerceptionObjectsLocalisation
// Created: JVT 2005-01-19
// -----------------------------------------------------------------------------
inline
const DIA_TypeDef& DEC_Tools::GetTypePerceptionObjectsLocalisation()
{
    assert( pTypePerceptionObjectsLocalisation_ );
    return *pTypePerceptionObjectsLocalisation_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Tools::GetTypePerceptionPoint
// Created: NLD 2004-10-21
// -----------------------------------------------------------------------------
inline
const DIA_TypeDef& DEC_Tools::GetTypePerceptionPoint()
{
    assert( pTypePerceptionPoint_ );
    return *pTypePerceptionPoint_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Tools::GetTypePerceptionLocalisation
// Created: JVT 2004-10-22
// -----------------------------------------------------------------------------
inline
const DIA_TypeDef& DEC_Tools::GetTypePerceptionLocalisation()
{
    assert( pTypePerceptionLocalisation_ );
    return *pTypePerceptionLocalisation_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Tools::GetTypePerceptionRadar
// Created: JVT 2004-10-22
// -----------------------------------------------------------------------------
inline
const DIA_TypeDef& DEC_Tools::GetTypePerceptionRadar()
{
    assert( pTypePerceptionRadar_ );
    return *pTypePerceptionRadar_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Tools::GetTypePerceptionSurveillance
// Created: JVT 2004-10-22
// -----------------------------------------------------------------------------
inline
const DIA_TypeDef& DEC_Tools::GetTypePerceptionSurveillance()
{
    assert( pTypePerceptionSurveillance_ );
    return *pTypePerceptionSurveillance_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Tools::GetTypeMaintenancePriorities
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
inline
const DIA_TypeDef& DEC_Tools::GetTypeMaintenancePriorities()
{
    assert( pTypeMaintenancePriorities_ );
    return *pTypeMaintenancePriorities_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Tools::GetTypeSantePriorites
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
inline
const DIA_TypeDef& DEC_Tools::GetTypeSantePriorites()
{
    assert( pTypeSantePriorites_ );
    return *pTypeSantePriorites_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Tools::GetTypePerceptionFlyingShell
// Created: NLD 2005-02-21
// -----------------------------------------------------------------------------
inline
const DIA_TypeDef& DEC_Tools::GetTypePerceptionFlyingShell()
{
    assert( pTypePerceptionFlyingShell_ );
    return *pTypePerceptionFlyingShell_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Tools::GetTypeConnaissancePopulation
// Created: NLD 2005-10-21
// -----------------------------------------------------------------------------
inline
const DIA_TypeDef& DEC_Tools::GetTypeConnaissancePopulation()
{
    assert( pTypeConnaissancePopulation_ );
    return *pTypeConnaissancePopulation_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Tools::GetTypePopulationConnaissanceAgent
// Created: NLD 2005-11-10
// -----------------------------------------------------------------------------
inline
const DIA_TypeDef& DEC_Tools::GetTypePopulationConnaissanceAgent()
{
    assert( pTypePopulationConnaissanceAgent_ );
    return *pTypePopulationConnaissanceAgent_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Tools::GetTypePopulationConnaissanceObjet
// Created: NLD 2005-11-10
// -----------------------------------------------------------------------------
inline
const DIA_TypeDef& DEC_Tools::GetTypePopulationConnaissanceObjet()
{
    assert( pTypePopulationConnaissanceObjet_ );
    return *pTypePopulationConnaissanceObjet_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Tools::GetTypeDotation
// Created: NLD 2004-10-21
// -----------------------------------------------------------------------------
inline
const DIA_TypeDef& DEC_Tools::GetTypeDotation()
{
    assert( pTypeDotation_ );
    return *pTypeDotation_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Tools::GetTypeGenObjet
// Created: NLD 2006-10-26
// -----------------------------------------------------------------------------
inline
const DIA_TypeDef& DEC_Tools::GetTypeGenObjet()
{
    assert( pTypeGenObjet_ );
    return *pTypeGenObjet_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Tools::GetTypeEquipement
// Created: NLD 2006-12-06
// -----------------------------------------------------------------------------
inline
const DIA_TypeDef& DEC_Tools::GetTypeEquipement()
{
    assert( pTypeEquipement_ );
    return *pTypeEquipement_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Tools::GetTypeTirIndirect
// Created: NLD 2006-12-06
// -----------------------------------------------------------------------------
inline
const DIA_TypeDef& DEC_Tools::GetTypeTirIndirect()
{
    assert( pTypeTirIndirect_ );
    return *pTypeTirIndirect_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Tools::GetTypeFuseau
// Created: NLD 2006-12-06
// -----------------------------------------------------------------------------
inline
const DIA_TypeDef& DEC_Tools::GetTypeFuseau()
{
    assert( pTypeFuseau_ );
    return *pTypeFuseau_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Tools::GetTypeObjectif
// Created: NLD 2007-05-14
// -----------------------------------------------------------------------------
inline
const DIA_TypeDef& DEC_Tools::GetTypeObjectif()
{
    assert( pTypeObjectif_ );
    return *pTypeObjectif_;
}
