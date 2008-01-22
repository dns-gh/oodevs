using System;
using ESRI.ArcGIS.Geodatabase;
using ESRI.ArcGIS.Framework;

namespace Sword
{
    namespace Crossbow
    {
        public sealed class ParameterLimas : IOrderParameter
        {
            private System.Collections.Generic.Dictionary<int, string> m_limas = new System.Collections.Generic.Dictionary<int, string>();
            private IFeature m_potential;

            public void Serialize(ITable table, int id)
            {
                foreach (System.Collections.Generic.KeyValuePair<int, string> elt in m_limas)
                    SerializeLima(table, id, elt.Key + ";" + elt.Value);
            }

            public static void SerializeLima(ITable table, int id, string value)
            {
                IRow row = table.CreateRow();
                Tools.SetValue<int>(row, "order_id", id);
                Tools.SetValue<string>(row, "name", "lima");
                Tools.SetValue<bool>(row, "context", true);
                Tools.SetValue<string>(row, "ParamValue", value);
                row.Store();
            }

            public void OnContextMenu(MultiItemContextMenu menu, int x, int y, IFeature selected)
            {
                if (menu == null || selected == null || selected.Class.AliasName != "TacticalLines")
                    return;
                menu.Add("LD", this);
                menu.Add("LCA", this);
                menu.Add("LC", this);
                menu.Add("LI", this);
                menu.Add("LO", this);
                menu.Add("LCAR", this);
                menu.Add("LR", this);
                menu.Add("LDM", this);
                menu.Add("LFM", this);
                menu.Add("LIA", this);
                m_potential = selected;
            }

            public void SetValue(string value)
            {
                if (m_potential != null)
                    m_limas[Tools.GetValue<int>(m_potential, "Public_OID")] = value;
            }

            public void NotifyUpdate(IMissionObserver observer)
            {
                foreach (System.Collections.Generic.KeyValuePair<int, string> elt in m_limas)
                    observer.Update(this, elt.Key.ToString(), elt.Value);
            }

            public string Name
            {
                get
                {
                    return "limas";
                }
            }
        }
    }
}