using System;
using ESRI.ArcGIS.Geodatabase;
using ESRI.ArcGIS.Framework;

namespace Sword
{
    namespace Crossbow
    {
        public sealed class ParameterLimits : IOrderParameter
        {
            struct Boundaries
            {
                public int first;
                public int second;
            };

            IFeature m_potential;
            Boundaries m_limits;

            private void SerializeLimit(ITable table, int order, string name, int limit)
            {
                IRow row = table.CreateRow();
                Tools.SetValue<int>(row, "order_id", order);
                Tools.SetValue<string>(row, "name", name);
                Tools.SetValue<string>(row, "ParamValue", limit.ToString());
                row.Store();
            }

            public void Serialize(ITable table, int id)
            {
                SerializeLimit(table, id, "Boundary limit 1", m_limits.first);
                SerializeLimit(table, id, "Boundary limit 2", m_limits.second);
            }

            public void OnContextMenu(MultiItemContextMenu menu, int x, int y, IFeature selected)
            {
                if (menu == null || selected == null || selected.Class.AliasName != "BoundaryLimits")
                    return;
                m_potential = selected;
                menu.Add("Set limit 1", this);
                menu.Add("Set limit 2", this);
            }

            public void SetValue(string value)
            {
                if (m_potential == null)
                    return;
                if (value == "Set limit 1")
                    m_limits.first = Tools.GetValue<int>(m_potential, "Public_OID");
                if (value == "Set limit 2")
                    m_limits.second = Tools.GetValue<int>(m_potential, "Public_OID");
            }

            public void NotifyUpdate(IMissionObserver observer)
            {
                if (observer != null)
                    observer.Update(this, m_limits.first.ToString(), m_limits.second.ToString());
            }

            public string Name
            {
                get
                {
                    return "Boundary limits";
                }
            }
        }
    }
}
