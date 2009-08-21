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
			[ "ABC" ] = 7,
		}
	},

	-- Types from physical model
	types =
	{
		-- Unit types
		units =
		{
			[ "MBT platoon" ] = 68,
		},

		-- Mission types
		missions =
		{
			[ "move" ] = 133,
		}
	}
}
