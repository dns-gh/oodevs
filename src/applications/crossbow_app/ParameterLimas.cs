using System;
using ESRI.ArcGIS.Geodatabase;
using ESRI.ArcGIS.Framework;

namespace Sword
{
    namespace Crossbow
    {
        public sealed class ParameterLima : OrderParameter
        {
            public ParameterLima()
                : base("Phase line", ParameterTypeFactory.Create("Line"))
            {
            }
        }

        public sealed class ParameterLimas : IOrderParameter
        {
            private System.Collections.Generic.Dictionary<int, ParameterLima> m_limas = new System.Collections.Generic.Dictionary<int, ParameterLima>();
            private IFeature m_potential;

            public void Serialize(ITable table, int id)
            {
                foreach (System.Collections.Generic.KeyValuePair<int, ParameterLima> elt in m_limas)
                    elt.Value.Serialize(table, id);
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
                    m_limas[Tools.GetValue<int>(m_potential, "Public_OID")].SetValue(value);
            }

            public void NotifyUpdate(IMissionObserver observer)
            {
                foreach (System.Collections.Generic.KeyValuePair<int, ParameterLima> elt in m_limas)
                    observer.Update(this, elt.Key.ToString(), elt.Value.Value);
            }

            public string Name
            {
                get
                {
                    return "Phase Lines";
                }
            }
        }
    }
}