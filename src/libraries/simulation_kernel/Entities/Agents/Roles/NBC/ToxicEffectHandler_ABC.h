/*
 * ToxicEffectHandler.h
 *
 *  Created on: 22 sept. 2009
 *      Author: ahc
 */

#ifndef __NBC_Toxic_Effect_Handler_ABC__
#define __NBC_Toxic_Effect_Handler_ABC__

class MIL_ToxicEffectManipulator;

namespace nbc
{
	// =============================================================================
	// @class  ToxicEffectHandler_ABC
	// Created: JVT 2004-08-03
	// =============================================================================
	class ToxicEffectHandler_ABC
	{
	public:
		virtual void     ApplyContamination              ( const MIL_ToxicEffectManipulator& contamination ) =0;
		virtual void     ApplyPoisonous                  ( const MIL_ToxicEffectManipulator& contamination ) =0;
	};
}

#endif /* __NBC_Toxic_Effect_Handler__ */
