//*****************************************************************************
// Created: JVT 02-08-30
//*****************************************************************************

// -----------------------------------------------------------------------------
// Name: DotationType::GetName
// Created: NLD 2004-09-10
// -----------------------------------------------------------------------------
inline
const std::string& DotationType::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: DotationType::GetID
// Created: NLD 2006-08-05
// -----------------------------------------------------------------------------
inline
uint DotationType::GetID() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: DotationType::GetCategory
// Created: NLD 2006-08-05
// -----------------------------------------------------------------------------
inline
const std::string& DotationType::GetCategory() const
{
    return strCategory_;
}

// -----------------------------------------------------------------------------
// Name: DotationType::CanBeUsedForIndirectFire
// Created: NLD 2006-08-07
// -----------------------------------------------------------------------------
inline
bool DotationType::CanBeUsedForIndirectFire() const
{
    return bCanBeUsedForIndirectFire_;
}
