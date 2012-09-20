integration.getCommanderPosition = function( commander )
    return DEC_Automate_Position( commander.source )
end

integration.getSubordinateAgentsFromCommander = function( commander )
    return DEC_Automate_PionsDeAutomateAvecPC( commander.source )
end

integration.isCommanderMoving = function( commander )
    local subordinates = commander.source:DEC_Automate_PionsAvecPC()
    for _, subordinate in pairs( subordinates or emptyTable ) do
        if subordinate:DEC_Agent_IsMoving() then
            return true
        end
    end
    return false
end

integration.getSubordinatePosition = function( subordinate )
    return DEC_Automate_Position( subordinate.source )
end
