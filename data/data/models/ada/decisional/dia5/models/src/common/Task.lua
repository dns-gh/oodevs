return
{
    ponderateEffectsForCommandEfficiency = function( self, body, params, threat, effects )
        threat = threat or {}
        if effects.threatResistanceEffectLevel ~= nil then
            local resistance = 0
            for _, threateningElement in ipairs( threat ) do
                resistance = resistance + ( 100 - body:computeEstimateAttritionOnMe( threateningElement ) )
            end
            effects.threatResistanceEffectLevel = effects.threatResistanceEffectLevel * resistance / 100
        else
            error( "Impossible to compute resistance under threats capability for the task "..self.name, 2 )
        end
    end,

    sendItTo = function( self, entityToCommand, objectives, threat )
        entityToCommand:communicate( self, objectives, threat )
    end,
}
