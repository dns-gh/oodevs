-- Actions

dofile "resources/scripts/actions.lua"

artillery =
{
    _Fire = function( firerId, target, dotation, iteration )
        local dotation  = dotation or orders.dotations["Obus 122 OE"]
        local iteration = iteration or 1
        orders.Issue(
            {
                fragorder = orders.fragOrders["Pion Appliquer feux"],
                target = firerId,
                parameters =
                {
                    { name = "Munitions", type = "DotationType", value = dotation },
                    { name = "Nb IT",     type = "Numeric",      value = iteration },
                    { name = "Cible",     type = "Point",      { orders.ParameterLocationPoint( target:GetPosition() ) } }
                }
            }
        )
    end,
    
    _IsIn = function( team, teamList )
        for _, v in ipairs( teamList ) do
            if v == team then return true end
        end
        return false
    end,
    
    DeclareFires = function( zones )
        local targeted = {}
        for _, zone in ipairs( zones ) do
            local event =
            {
                events.agents:KnowledgeEnters( drawings:Create( zone.name ):GetCoordinates() ),
                zone.states or {},
                function( k, target )
                    if k == nil or target == nil 
                       or not artillery._IsIn( target:GetTeam(), zone.targetTeams ) 
                       or not ( artillery._IsIn( k:GetOwnerTeam(), zone.firerTeams ) or #zone.firerTeams == 0 )
                            then return 
                    end

                    local firers = zone.firers
                    if #firers > 0 and not targeted[ target:GetIdentifier() ] then
                        local firer = firers[#firers]
                        table.remove(firers)
                        targeted[ target:GetIdentifier() ] = true
                        artillery._Fire( firer, target, zone.dotation, zone.iteration )
                    end
                end
            }
            DeclareEvents( event )
        end
    end
    
-- Exemple d'utilisation
-- artillery.DeclareFires(
--     {
--         { 
--             name = "ZoneTirArtillerieDroite",
--             firers = { 529, 530, 588 },
--             targetTeams = { "Bleu" },
--         },
--         { 
--             name = "ZoneTirArtillerieGauche",
--             firers = { 259, 260, 589 },
--             firerTeams = { "Rouge" },
--             targetTeams = { "Bleu" },
--             states = { "fire" }
--             iteration = 3
--         }
--     }
-- )

-- Chaque "firers" ne tire qu'une fois
-- Chaque nouvelle connaissance apparaissant dans la zone "name" n'est tirée qu'une fois

}
