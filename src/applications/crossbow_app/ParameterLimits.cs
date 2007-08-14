using System;
using ESRI.ArcGIS.Geodatabase;
using ESRI.ArcGIS.Framework;

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

        public void Serialize(ITable table, int id)
        {
            IRow row = table.CreateRow();
            Tools.SetValue<int>(row, "order_id", id);
            Tools.SetValue<string>(row, "name", "limits");
            Tools.SetValue<bool>(row, "context", true);
            Tools.SetValue<string>(row, "ParamValue", m_limits.first + ";" + m_limits.second);
            row.Store();
        }

        public void OnContextMenu(MultiItemSelectionMenu menu, int x, int y, IFeature selected)
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
                return "limits";
            }
        }
    }
}
