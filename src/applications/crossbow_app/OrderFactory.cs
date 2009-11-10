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

            public void BuildMissionContextMenu(MultiItemContextMenu menu, IFeature selected)
            {
                string entityName = Tools.GetValue<string>(selected, "Name");
                EntityModel model = null;
                if (entityName == null)
                    return;
                if (m_model.UnitTypes.ContainsKey(entityName))
                {
                    string type = m_model.UnitTypes[entityName];
                    if (m_model.UnitModels.ContainsKey(type))
                        model = m_model.UnitModels[type];
                }
                else if (m_model.AutomatTypes.ContainsKey(entityName))
                {
                    string type = m_model.AutomatTypes[entityName];
                    if (m_model.AutomatModels.ContainsKey(type))
                        model = m_model.AutomatModels[type];
                }
                if (model == null)
                    return;
                foreach (string mission in model)
                    menu.Add(mission, null);
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