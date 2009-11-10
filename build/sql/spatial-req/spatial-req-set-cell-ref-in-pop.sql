-- The crapouille way (clean way should be: add user id to handle multiple user)
DROP TABLE sword.relPopuPolygons;

SELECT   sword.population.id AS population_id, min(terrain_grid.id) AS terrain_grid_id
INTO     sword.relPopuPolygons -- temporary association between an arc and its polygon
FROM     sword.population, sword.terrain_grid
WHERE    sde.st_intersects(sword.terrain_grid.shape, sword.population.shape)
GROUP BY sword.population.id;

CREATE UNIQUE INDEX ix_relPopuPolygons_id ON sword.relPopuPolygons USING btree(population_id) WITH (FILLFACTOR=75);


-- update cell id of each arc
UPDATE     sword.population SET cell_id = tmp.terrain_grid_id
FROM       sword.relPopuPolygons AS tmp
INNER JOIN sword.population pop ON pop.id = tmp.population_id
WHERE      tmp.population_id = sword.population.id


-- sum of individuals in each cell
SELECT sum(sword.population.individuals) AS sum_individuals
FROM sword.population
GROUP BY sword.population.cell_id;

-- number of arcs per polygon v1
SELECT sword.relPopuPolygons.terrain_grid_id AS grid_id, count(sword.relPopuPolygons.population_id) AS sum_arcs
FROM sword.relPopuPolygons
GROUP BY sword.relPopuPolygons.terrain_grid_id
ORDER BY grid_id;


-- number of arcs per polygon v2 using LEFT OUTER JOIN -> empty polygons are removed
SELECT     sword.terrain_grid.id AS grid_id , count(sword.relPopuPolygons.population_id) AS arc_nb
FROM       sword.terrain_grid
LEFT OUTER JOIN sword.relPopuPolygons
ON 	   sword.terrain_grid.id = sword.relPopuPolygons.terrain_grid_id
GROUP BY   sword.terrain_grid.id
ORDER BY   grid_id;





-- number of arcs per polygon v3 using implicit join -> empty polygons are removed
SELECT     sword.terrain_grid.id AS grid_id , count(sword.relPopuPolygons.population_id) AS arc_nb
FROM       sword.terrain_grid, sword.relPopuPolygons
WHERE      sword.terrain_grid.id = sword.relPopuPolygons.terrain_grid_id
GROUP BY   sword.terrain_grid.id
ORDER BY   grid_id;