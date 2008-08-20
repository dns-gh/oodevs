using System;
using System.Collections.Generic;
using ESRI.ArcGIS.Framework;
using ESRI.ArcGIS.esriSystem;
using ESRI.ArcGIS.Geodatabase;
using ESRI.ArcGIS.Carto;
using ESRI.ArcGIS.SystemUI;
using ESRI.ArcGIS.ArcMapUI;

namespace Sword
{
    namespace Crossbow
    {
        public interface IParameterFactory
        {
            void CreateParameters(IOrder order);
        }

        public class OrderFactory : IParameterFactory
        {
            private StaticModel m_model;

            public OrderFactory(StaticModel model)
            {
                m_model = model;
            }

            private bool IsUnitEngaged(IFeature selected, ref int type)
            {
                bool engaged = false;
                int oid = Tools.GetValue<int>(selected, "Parent_OID");
                ITable table = selected.Table;
                IDataset set = table as IDataset;
                
                IFeatureWorkspace wp = set.Workspace as IFeatureWorkspace;
                ITable formations = wp.OpenTable("Formations");
                IQueryFilter query = new QueryFilterClass();                
                query.WhereClause = "Public_OID = " + oid;
                ICursor cursor = formations.Search(query, true);
                IRow row = cursor.NextRow();
                type = Tools.GetValue<int>(selected, "Type");
                if (row != null)
                {                    
                    engaged = Tools.GetValue<short>(row, "Engaged") < 0;
                    if (engaged)
                        type = Tools.GetValue<int>(row, "Type");
                }
                return engaged;
            }

            public void BuildMissionContextMenu(MultiItemContextMenu menu, IFeature selected)
            {
                try
                {
                    int entityType = -1;
                    string entityName = Tools.GetValue<string>(selected, "Name");                    
//                    int entityType = Tools.GetValue<int>(selected, "Type");

                    EntityModel model = null;
                    if (entityName == null)
                        return;


                    if (IsUnitEngaged(selected, ref entityType)) // Engaged ?
                    {
                        if (m_model.AutomatTypes.ContainsKey(entityType))
                        {
                            string type = m_model.AutomatTypes[entityType];
                            if (m_model.AutomatModels.ContainsKey(type))
                                model = m_model.AutomatModels[type];
                        }
                    }
                    else
                    {
                        if (m_model.UnitTypes.ContainsKey(entityType))
                        {
                            string type = m_model.UnitTypes[entityType];
                            if (m_model.UnitModels.ContainsKey(type))
                                model = m_model.UnitModels[type];
                        }
                    }
                        
                    if (model == null)
                        return;
                    foreach (string mission in model)
                        menu.Add(mission, null);
                }
                catch( Exception ex )
                {
                }
            }

            public Order CreateOrder(string name, OrderHandler handler)
            {
                return new Order(name, handler, this);
            }

            public virtual void CreateParameters(IOrder order)
            {
                if (m_model.AutomatMissions.ContainsKey(order.Name))
                    foreach (KeyValuePair<string, string> definition in m_model.AutomatMissions[order.Name])
                        order.RegisterParameter(new OrderParameter(definition.Key.ToString(), definition.Value.ToString()));
                else if (m_model.UnitMissions.ContainsKey(order.Name))
                    foreach (KeyValuePair<string, string> definition in m_model.UnitMissions[order.Name])
                        order.RegisterParameter(new OrderParameter(definition.Key.ToString(), definition.Value.ToString()));
            }
        }
    }
}