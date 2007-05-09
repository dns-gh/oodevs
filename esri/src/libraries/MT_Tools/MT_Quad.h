//****************************************************************************
//
// This file is part of MASA MT library.
// 
// Refer to the included end-user license agreement (License.rtf) for
// restrictions.
//
// Copyright (c) 2002 Mathématiques Appliquées SA (MASA)
// All Rights Reserved. MT is a trademark of MASA Corporation.
//
//*****************************************************************************
// Created: JVT 03-01-09
//*****************************************************************************

#ifndef __MT_Quad_h_
#define __MT_Quad_h_

//=============================================================================
// Created: JVT 03-01-09
//=============================================================================
template< class _T1, class _T2, class _T3, class _T4 >
struct MT_Quad
{
	typedef _T1 first_type;
	typedef _T2 second_type;
	typedef _T3 third_type;
    typedef _T4 forth_type;
	
	explicit MT_Quad()
		: first ( _T1() )
		, second( _T2() )
		, third ( _T3() )
        , forth ( _T4() )
	{
	}
	
	MT_Quad( const _T1& _V1, const _T2& _V2, const _T3& _V3, const _T4& _V4 )
		: first ( _V1 )
		, second( _V2 )
		, third ( _V3 )
        , forth ( _V4 )
	{
	}
	
	template< class U, class V, class W, class X > 
		MT_Quad( const MT_Quad< U, V, W, X >& rhs )
		: first ( rhs.first )
		, second( rhs.second ) 
		, third ( rhs.third )
        , forth ( rhs.forth )
	{
	}
	
	template< class U, class V, class W, class X > 
		MT_Quad& operator=( const MT_Quad< U, V, W, X >& rhs )
	{
		if( this == &rhs )
			return *this;
		
		first  = rhs.first;
		second = rhs.second;
		third  = rhs.third;
        forth  = rhs.forth;
		
		return *this;
	}
	
	_T1 first;
	_T2 second;
	_T3 third;
    _T4 forth;
};



//-----------------------------------------------------------------------------
// Name: make_quad
/**
  Helper function to easyly create MT_Quad
*/
// Created: JVT 03-01-09
//-----------------------------------------------------------------------------
template<class _T1, class _T2, class _T3, class _T4> 
inline
MT_Quad<_T1, _T2, _T3, _T4> __cdecl make_quad(const _T1& _X, const _T2& _Y, const _T3& _Z, const _T4& _T )
{
	return ( MT_Quad< _T1, _T2, _T3>(_X, _Y, _Z, _T) ); 
}

#endif // __MT_Quad_h_
