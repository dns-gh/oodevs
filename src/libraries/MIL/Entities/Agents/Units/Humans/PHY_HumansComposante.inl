// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Humans/PHY_HumansComposante.inl $
// $Author: Jvt $
// $Modtime: 1/04/05 11:25 $
// $Revision: 3 $
// $Workfile: PHY_HumansComposante.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante::serialize
// Created: JVT 2005-04-01
// -----------------------------------------------------------------------------
template< typename Archive >
inline
void PHY_HumansComposante::serialize( Archive& file, const uint )
{
    file & pComposante_
         & humans_;
}
