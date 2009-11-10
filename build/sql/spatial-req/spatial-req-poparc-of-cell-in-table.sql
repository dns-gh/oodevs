-- see: http://www.postgis.fr/book/print/191#196196196

TRUNCATE TABLE sword.population_group_1

INSERT INTO sword.population_group_1 
SELECT sde.st_intersection(terrain_grid.shape, population.shape) AS intersection_geom, sde.st_length(population.shape) AS rd_orig_length, sword.population.*
FROM sword.population, sword.terrain_grid
WHERE sword.population.shape && sword.terrain_grid.shape
  AND sde.st_intersects(sword.terrain_grid.shape, sword.population.shape)
  AND terrain_grid.id = 1;


UPDATE sword.population SET cell_id = 1;
WHERE sword.population.shape && sword.terrain_grid.shape
  AND sde.st_intersects(sword.terrain_grid.shape, sword.population.shape)
  AND terrain_grid.id = 1;
  
-- create a table containing all ARCS that intersect polygon 1 of terrain_grid polygons


