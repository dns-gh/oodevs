--CREATE UNIQUE CLUSTERED INDEX ix_population_id ON sword.population USING btree(id) WITH (FILLFACTOR=75);

--SELECT sde.st_register_spatial_column('sword', 'emergencies', 'shape', 'sword_auto_install', 4326);
--SELECT sde.st_register_spatial_column('sword', 'unitforces', 'shape', 'sword_auto_install', 4326);
--SELECT sde.st_register_spatial_column('sword', 'boundarylimits', 'shape', 'sword_auto_install', 4326);

--CREATE UNIQUE INDEX Sword_Emergencies_id ON sword.emergencies USING btree(id) WITH (FILLFACTOR=75);
--CREATE INDEX Sword_Emergencies_shape ON sword.emergencies USING gist(shape);

--CREATE INDEX Sword_UnitForces_shape ON sword.unitforces USING gist(shape);
--CREATE INDEX Sword_UnitForces_id ON sword.unitforces USING btree(id) WITH (FILLFACTOR=75);

--CREATE UNIQUE INDEX Sword_Orders_id ON sword.orders USING btree(id) WITH (FILLFACTOR=75);
--CREATE UNIQUE INDEX Sword_Orderparameters_order_id ON sword.orderparameters USING btree(order_id) WITH (FILLFACTOR=75);
--CREATE UNIQUE INDEX sword_orderparameters_area ON sword.orderparameters_area USING btree(parameter_id) WITH (FILLFACTOR=75);

--CREATE UNIQUE INDEX sword_orderparameters_line ON sword.orderparameters_line USING btree(parameter_id) WITH (FILLFACTOR=75);
--CREATE UNIQUE INDEX sword_orderparameters_point ON sword.orderparameters_point USING btree(parameter_id) WITH (FILLFACTOR=75);
--CREATE UNIQUE INDEX Sword_Reports_id ON sword.reports USING btree(id) WITH (FILLFACTOR=75);

--CREATE INDEX Sword_BoundaryLimits_shape ON sword.boundarylimits USING gist(shape);
--CREATE UNIQUE INDEX Sword_BoundaryLimits_publicOid ON sword.boundarylimits USING btree(public_oid) WITH (FILLFACTOR=75);
