using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Serialization;

namespace OOP_Lab04
{
    [Serializable]
    public abstract class Figure
    {
        public float pointX { get; set; }

        [NonSerialized]
        //[XmlIgnore]
        //public int NonSerialized = 1;
        public float pointY; 

        public override string ToString()
        {
            return $"pointX{pointX} \t pointY{pointY}";
        }

        public abstract void Show();
    }
}
