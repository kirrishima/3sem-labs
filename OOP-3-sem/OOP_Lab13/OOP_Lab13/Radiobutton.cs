using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OOP_Lab04
{
    public sealed class  Radiobutton : ElemOfManage, IManagement
    {
        public void Show()
        {
            Console.WriteLine("Это Radiobutton");
        }
        public void Input()
        {
            if (Tap)
            {
                Console.WriteLine("Radiobutton отмечен ");
            }
            else
            {
                Console.WriteLine("Radiobutton не отмечен ");
            }

        }
    }
}
