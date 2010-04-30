-- Model

model =
{
	-- Entities from ORBAT
	entities =
	{
		-- Automats cannot be dynamically created, so we need to define one of each existing type
		-- in order to be able to create subordinate units
		-- TODO: see if each automat must have at least one subordinate (command post)
		automats =
		{
			[ "AMI_INF1" ] = 17,
			[ "AMI_INF2" ] = 48,
			[ "AMI_ABC1" ] = 23,
			[ "AMI_ABC2" ] = 43,			
			[ "ENI_INF1" ] = 0,
			[ "ENI_INF2" ] = 0,
			[ "ENI_ABC1" ] = 31,
			[ "ENI_ABC2" ] = 38,
		}
	},

	-- Types from physical model
	types =
	{
		-- Unit types
		units =
		{
			[ "INF1_PC_VAB" ] = 68,
			[ "INF1_VAB" ] = 19,
		},

		-- Mission types
		missions =
		{
			[ "Faire Mouvement" ] = 131,
		}
	}
}
