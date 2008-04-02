//*****************************************************************************
//
// $Created: RDS 08-04-01 $
// $Archive: /MVW_v10/Build/SDK/TER/src/TER_Object_ABC.inl $
// $Author: RDS $
// $Modtime: 14/06/05 15:22 $
// $Revision: 2 $
// $Workfile: TER_Object_ABC.inl $
//
//*****************************************************************************

// -----------------------------------------------------------------------------
// Name: TER_Object_ABC::load
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
template<class Archive>
void TER_Object_ABC::load( Archive& file, const uint )
{
    file >> location_;
    InsertInWorld();
}

// -----------------------------------------------------------------------------
// Name: TER_Object_ABC::save
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
template<class Archive>
void TER_Object_ABC::save( Archive& file, const uint ) const
{
    file << location_;
}
